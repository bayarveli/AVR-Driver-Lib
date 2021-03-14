/*
 * usart.c
 *
 *  Created on: Apr 21, 2020
 *      Author: veli-
 */

#include <avr/interrupt.h>
#include "usart.h"


static void usart_calculateBaudrate(BaudrateSelect baudrate, uint8_t isDoubleSpeedEnabled);

static void usart_calculateBaudrate(BaudrateSelect baudrate, uint8_t isDoubleSpeedEnabled)
{
	UsartTypeDef *pUsart = 0;
	uint32_t baudrateValue = 9600;
	uint32_t ubrrValue = 0;

	pUsart = USART1;

	switch (baudrate)
	{
	case BAUDRATE_9600:
		baudrateValue = 9600;
		break;
	case BAUDRATE_19200:
		baudrateValue = 19200;
		break;
	case BAUDRATE_38400:
		baudrateValue = 38400;
		break;
	case BAUDRATE_115200:
		baudrateValue = 115200;
		break;
	default:
		baudrateValue = 9600;
		break;
	}

	if (isDoubleSpeedEnabled == 0)
	{
		ubrrValue = ((CLOCK_FREQ / (16 * baudrateValue)) - 1);
	}
	else // Double speed is only enabled in asynchronous mode
	{
		ubrrValue = ((CLOCK_FREQ / (8 * baudrateValue)) - 1);
	}

	pUsart->sUBRRL = ubrrValue & 0xFF;
	pUsart->sUBRRH = ubrrValue >> 8;
}


void usart_init(UartModeSelect uartMode,
				BaudrateSelect baudrate,
				DataBitSelect dataBit,
				ParityModeSelect parityMode,
				StopBitSelect stopBit,
				PolaritySelect polarity,
				SpeedSelect speed)
{
	UsartTypeDef *pUsart = 0;
	uint8_t controlAndStatusRegC = 0U;
	uint8_t controlAndStatusRegB = 0U;
	uint8_t controlAndStatusRegA = 0U;

	pUsart = USART1;

	switch (uartMode)
	{
	case USART_ASYNCH:
		controlAndStatusRegC |= uartMode << 6;
		controlAndStatusRegC |= parityMode << 4;
		controlAndStatusRegC |= stopBit << 3;
		controlAndStatusRegC |= (dataBit & 0x7) << 1;
		controlAndStatusRegB |= ((dataBit & 0x4) >> 2) << 2;
		controlAndStatusRegC |= 0; // Clock polarity - Write 0 in asynchronous mode
		controlAndStatusRegA |= speed << 1;

		if (speed == DOUBLE_SPEED)
		{
			usart_calculateBaudrate(baudrate, 1U);
		}
		else
		{
			usart_calculateBaudrate(baudrate, 0U);
		}

		break;
	case USART_SYNC:
		controlAndStatusRegC |= uartMode << 6;
		controlAndStatusRegC |= parityMode << 4;
		controlAndStatusRegC |= stopBit << 3;
		controlAndStatusRegC |= (dataBit & 0x2) << 1;
		controlAndStatusRegB |= ((dataBit & 0x4) >> 2) << 2;
		controlAndStatusRegC |= polarity;
		controlAndStatusRegA |= 0 << 1;

		usart_calculateBaudrate(baudrate, 0U);

		break;
	case USART_SPI:
		// Not implemented
		break;
	}

	pUsart->sUCSRA = controlAndStatusRegA;
	pUsart->sUCSRB = controlAndStatusRegB;
	pUsart->sUCSRC = controlAndStatusRegC;
}

void usart_open()
{
	UsartTypeDef *pUsart = 0;
	pUsart = USART1;

	pUsart->sUCSRB |= (0x3 << 3);
}

void usart_transmit(uint8_t * dataToTransmit, uint8_t isString)
{
	UsartTypeDef *pUsart = 0;
	pUsart = USART1;

	if (isString == 0U)
	{
		if ((pUsart->sUCSRA & 0x20) >> 5)
		{
			pUsart->sUDR = *dataToTransmit;
		}
	}
	else
	{
		while(*(dataToTransmit) != '\0')
		{
			if ((pUsart->sUCSRA & 0x20) >> 5)
			{
				pUsart->sUDR = *(dataToTransmit++);
			}
		}
	}
}

void usart_receive(uint8_t * dataReceived, uint8_t *receivedDataCount)
{
	UsartTypeDef *pUsart = 0;
	pUsart = USART1;

//	while (!(pUsart->UCSRA & (1 << 7)));
//
//	*dataReceived = pUsart->UDR;
//	*receivedDataCount = 1;
	if ((pUsart->sUCSRA & (1 << 7)))
	{
		*dataReceived = pUsart->sUDR;
		*receivedDataCount = 1;
	}
	else
	{
		*receivedDataCount = 0;
	}
}

void usart_close()
{
	UsartTypeDef *pUsart = 0;
	pUsart = USART1;

	pUsart->sUCSRB |= (0x0 << 3);
}

void usart_flush()
{

}

void usart_setCallback(void (*ptr)())
{
	UsartTypeDef *pUsart = 0;
	pUsart = USART1;

	usartRxCallback = ptr;

	pUsart->sUCSRB |= (1 << 7); // RXCIE1 enable rx interrupt
}

ISR(USART1_RX_vect)
{
	 usartRxCallback();
}

