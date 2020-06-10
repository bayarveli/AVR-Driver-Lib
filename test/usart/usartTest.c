/*
 * usartTest.c
 *
 *  Created on: May 23, 2020
 *      Author: veli-
 */

#include "gpio.h"
#include "usart.h"

#ifdef CLOCK_FREQ
	#define F_CPU CLOCK_FREQ
#else
	#define F_CPU 16000000UL
#endif

#include <util/delay.h>
#include <string.h>

int main(void)
{

	// Baudrate = 115200 is not working.
	usart_init(USART_ASYNCH,
			   BAUDRATE_9600,
			   DATABIT_8,
			   PARITY_NONE,
			   STOPBIT_1,
			   RISING_EDGE,
			   NOMINAL_SPEED);

	usart_open();


	uint8_t receivedData = 0;
	uint8_t receivedDataCount = 0;
	uint8_t totalReceived = 0;

	uint8_t transmitBuffer[100] = { 0 };

	while(1)
	{
		usart_receive(&receivedData, &receivedDataCount);



		if (receivedDataCount != 0 )
		{
			totalReceived += receivedDataCount;

			usart_transmit(&receivedData, 0);

			receivedData = 0;

			itoa(totalReceived, transmitBuffer, 10);

			usart_transmit(transmitBuffer, 1);
		}

	}

	return 0;
}
