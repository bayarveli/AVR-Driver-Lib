/*
 * test.c
 *
 *  Created on: Dec 22, 2019
 *      Author: veli-
 */
#define F_CPU 16000000UL
#define BAUD 9600UL


#include "gpio.h"
#include "usart.h"
#include "i2c.h"

#include "atmega32u4.h"

// #include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>
// #include <avr/interrupt.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


uint8_t usartTxBuffer[100];
const char startString[10] = "IIC TEST\n";
const char stopString[10] = "IIC END\n";
const char debugString[10] = "HERE! \n";
const char timeTemplate[64] = "Time >> %2d:%2d:%2d \t Date >> %2d-%2d-%2d Day: %2d \n";

uint16_t bcdToInt(uint8_t bcdNumber)
{
	return (((bcdNumber & 0x70) >> 4) * 10 + (bcdNumber & 0x0F));
}

uint16_t bcdToInt2(uint8_t bcdNumber)
{
	return (((bcdNumber & 0xF0) >> 4) * 10 + (bcdNumber & 0x0F));
}

#define DELAY_TIME (100)

int main(void)
{

	usart_init(USART_ASYNCH,
			   BAUDRATE_9600,
			   DATABIT_8,
			   PARITY_NONE,
			   STOPBIT_1,
			   RISING_EDGE,
			   NOMINAL_SPEED);

	gpio_setDir(PORT_D, PIN_0, OUT);
	gpio_setDir(PORT_D, PIN_1, OUT);

	gpio_setValue(PORT_D, PIN_0);
	gpio_setValue(PORT_D, PIN_1);

	usart_open();

	uint8_t slaveAddress = 0x68;
	uint8_t wordAddress = 0x00;
	uint8_t second = 0x0;
	uint8_t minute = 0x0;
	uint8_t hour = 0x0;
	uint8_t dayOfWeek = 0x0;
	uint8_t dayOfMonth = 0x0;
	uint8_t month = 0x0;
	uint8_t year = 0x0;

	uint8_t setSecond = 0x0;
	uint8_t setMinute = 0x10;
	uint8_t setHour = 0x15;
	uint8_t setDay = 0x04;
	uint8_t setMonth = 0x07;
	uint8_t setYear = 0x20;

	uint8_t status = 0x0;

	i2c_init();

	_delay_us(DELAY_TIME);
	// Set the second
	status = i2c_write(slaveAddress, 0x00, &setSecond, 1);
	if (status != 0)
	{
		usartTxBuffer[0] = status;
		usartTxBuffer[1] = '\0';
		usart_transmit(usartTxBuffer, 1);
	}

	_delay_us(DELAY_TIME);

	status = i2c_write(slaveAddress, 0x01, &setMinute, 1);
	if (status != 0)
	{
		usartTxBuffer[0] = status;
		usartTxBuffer[1] = '\0';
		usart_transmit(usartTxBuffer, 1);
	}

	_delay_us(DELAY_TIME);

	status = i2c_write(slaveAddress, 0x02, &setHour, 1);
	if (status != 0)
	{
		usartTxBuffer[0] = status;
		usartTxBuffer[1] = '\0';
		usart_transmit(usartTxBuffer, 1);
	}

	_delay_us(DELAY_TIME);

	status = i2c_write(slaveAddress, 0x04, &setDay, 1);
	if (status != 0) {
		usartTxBuffer[0] = status;
		usartTxBuffer[1] = '\0';
		usart_transmit(usartTxBuffer, 1);
	}

	_delay_us(DELAY_TIME);

	status = i2c_write(slaveAddress, 0x05, &setMonth, 1);
	if (status != 0) {
		usartTxBuffer[0] = status;
		usartTxBuffer[1] = '\0';
		usart_transmit(usartTxBuffer, 1);
	}

	_delay_us(DELAY_TIME);

	status = i2c_write(slaveAddress, 0x06, &setYear, 1);
	if (status != 0) {
		usartTxBuffer[0] = status;
		usartTxBuffer[1] = '\0';
		usart_transmit(usartTxBuffer, 1);
	}

	_delay_us(DELAY_TIME);

	while(1)
	{
		status = i2c_read_after_write(slaveAddress, 0x00, &second);
		if (status != 0)
		{
			usartTxBuffer[0] = status;
			usartTxBuffer[1] = '\0';
			usart_transmit(usartTxBuffer, 1);
		}

		_delay_us(DELAY_TIME);

		status = i2c_read_after_write(slaveAddress, 0x01, &minute);
		if (status != 0)
		{
			usartTxBuffer[0] = status;
			usartTxBuffer[1] = '\0';
			usart_transmit(usartTxBuffer, 1);
		}
		_delay_us(DELAY_TIME);

		status = i2c_read_after_write(slaveAddress, 0x02, &hour);
		if (status != 0)
		{
			usartTxBuffer[0] = status;
			usartTxBuffer[1] = '\0';
			usart_transmit(usartTxBuffer, 1);
		}
		_delay_us(DELAY_TIME);
		status = i2c_read_after_write(slaveAddress, 0x03, &dayOfWeek);
		if (status != 0)
		{
			usartTxBuffer[0] = status;
			usartTxBuffer[1] = '\0';
			usart_transmit(usartTxBuffer, 1);
		}
		_delay_us(DELAY_TIME);
		status = i2c_read_after_write(slaveAddress, 0x04, &dayOfMonth);
		if (status != 0)
		{
			usartTxBuffer[0] = status;
			usartTxBuffer[1] = '\0';
			usart_transmit(usartTxBuffer, 1);
		}
		_delay_us(DELAY_TIME);
		status = i2c_read_after_write(slaveAddress, 0x05, &month);
		if (status != 0)
		{
			usartTxBuffer[0] = status;
			usartTxBuffer[1] = '\0';
			usart_transmit(usartTxBuffer, 1);
		}
		_delay_us(DELAY_TIME);
		i2c_read_after_write(slaveAddress, 0x06, &year);
		if (status != 0)
		{
			usartTxBuffer[0] = status;
			usartTxBuffer[1] = '\0';
			usart_transmit(usartTxBuffer, 1);
		}
		_delay_us(DELAY_TIME);
		snprintf(usartTxBuffer, 100, timeTemplate, bcdToInt(hour),
				bcdToInt(minute), bcdToInt(second), bcdToInt(dayOfMonth), bcdToInt(month), bcdToInt2(year), bcdToInt2(dayOfWeek));

		usart_transmit(usartTxBuffer, 1);

		_delay_ms(1000);
	}



//	strcpy(usartTxBuffer, stopString);
//	usart_transmit(usartTxBuffer, 1);

	return 0;
}
