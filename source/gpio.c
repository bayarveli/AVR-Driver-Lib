/**
 * @file gpio.c
 * @author Veli Bayar
 * @date 29 Dec 2019
 * @brief A template of GPIO library for AVR - Atmega32U4
 */

#include "gpio.h"

static GpioTypeDef *gpio_getPointer(uint8_t portNum);

static GpioTypeDef *gpio_getPointer(uint8_t portNum)
{
	GpioTypeDef *pGPIO = 0;

	switch (portNum) {
		case 0:
			// PORTA is not available for 32U4
			break;
		case 1:
			pGPIO = GPIOB;
			break;
		case 2:
			pGPIO = GPIOC;
			break;
		case 3:
			pGPIO = GPIOD;
			break;
		case 4:
			pGPIO = GPIOE;
			break;
		case 5:
			pGPIO = GPIOF;
			break;
		default:
			break;
	}
	return pGPIO;
}


void gpio_setDir(GpioPortType portType, GpioPinNumber pinNo, GpioPinDir pinDir)
{
	GpioTypeDef *pGpio = gpio_getPointer(portType);

	if (pGpio != 0)
	{
		// Enable output
		if (pinDir)
		{
			pGpio->sDDR |= (1 << pinNo);
		}
		else
		{
			pGpio->sDDR &= ~(1 << pinNo);
		}
	}
}

void gpio_setValue(GpioPortType portType, GpioPinNumber pinNo)
{
	GpioTypeDef *pGpio = gpio_getPointer(portType);

	if (pGpio != 0)
	{
		pGpio->sPORT |= (1 << pinNo);
	}
}

void gpio_clearValue(GpioPortType portType, GpioPinNumber pinNo)
{
	GpioTypeDef *pGpio = gpio_getPointer(portType);

	if (pGpio != 0)
	{
		pGpio->sPORT &= ~(1 << pinNo);
	}

}

uint8_t gpio_readValue(GpioPortType portType, GpioPinNumber pinNo)
{
	GpioTypeDef *pGpio = gpio_getPointer(portType);
	uint8_t returnVal;

	if (pGpio != 0)
	{
		returnVal = ((pGpio->sPORT) & (1 << pinNo)) >> pinNo;
	}

	return returnVal;
}

