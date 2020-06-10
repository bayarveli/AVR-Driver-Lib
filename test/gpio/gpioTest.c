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

#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>
#include <avr/interrupt.h>
#include <string.h>



typedef enum
{
	WRITE = 0,
	READ = 1
}UsartState;


int main(void)
{
	usart_init(USART_ASYNCH,
			   BAUDRATE_9600,
			   DATABIT_8,
			   PARITY_NONE,
			   STOPBIT_1,
			   RISING_EDGE,
			   NOMINAL_SPEED);
//
//	gpio_setDir(PORTB, PIN0, OUT);
//	gpio_setDir(PORTD, PIN5, OUT);
//	gpio_setDir(PORTC, PIN7, OUT);
//
//	gpio_setValue(PORTB, PIN0);
//	gpio_setValue(PORTD, PIN5);
//	gpio_setValue(PORTC, PIN7);
//
//
	usart_open();

	UsartTypeDef *pUsart = 0;
	pUsart = USART1;


	uint8_t usartData[3];
	usartData[0] = 'A';
	usartData[1] = 'B';
	usartData[2] = '\n';

	int idx = 0;

	while(1)
	{
//	    loop_until_bit_is_set(UCSR1A, UDRE1); /* Wait until data register empty. */
//	    UDR1 = ax;
		usart_transmit(&usartData[idx++], 0);


	    if (idx > 2)
	    {
	    	idx = 0;
	    }
	    _delay_ms(2000);
	}

	return 0;
}
