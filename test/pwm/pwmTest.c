/*
 * test.c
 *
 *  Created on: Dec 22, 2019
 *      Author: veli-
 */

#include "gpio.h"
#include "usart.h"
#include "timer.h"


#ifdef CLOCK_FREQ
	#define F_CPU CLOCK_FREQ
#else
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

int main(void)
{
	// usart is used for debugging purposes
	usart_init(USART_ASYNCH,
			   BAUDRATE_9600,
			   DATABIT_8,
			   PARITY_NONE,
			   STOPBIT_1,
			   RISING_EDGE,
			   NOMINAL_SPEED);


	// Set PWM output pins as output
	gpio_setDir(PORT_B, PIN_7, OUT); // OCR0A
	gpio_setDir(PORT_B, PIN_6, OUT); // Dummy to check voltage

	gpio_setDir(PORT_D, PIN_0, OUT); // OCR0B
	gpio_setDir(PORT_D, PIN_1, OUT); // Dummy to check voltage

	gpio_setValue(PORT_D, PIN_1);
	gpio_setValue(PORT_B, PIN_6);

	// open usart
	usart_open();

	timer_configure(TIMER_0, (TimerWaveformType)TIMER_0_FAST_PWM_TOP);
	timer_ocr_configure(TIMER_0, OCR0_A, NOT_INVERTED);
	timer_ocr_configure(TIMER_0, OCR0_B, NOT_INVERTED);
	timer_set_ocr_value(TIMER_0, OCR0_A, 127);
	timer_set_ocr_value(TIMER_0, OCR0_B, 127);
	timer_start(TIMER_0, CLK_IO_DIV_BY_1024);


	// timer - pwm settings
//	TCCR0A |= (_BV(WGM01) | _BV(WGM00));
//	TCCR0A |= (_BV(COM0B1) | _BV(COM0B0));
//	TCCR0A &= ~(_BV(COM0B0));
//	TCCR0A |= (_BV(COM0A1) | _BV(COM0A0));
//	TCCR0A &= ~(_BV(COM0A0));
//
//	TCCR0B |= (_BV(CS02) | _BV(CS01) | _BV(CS00));
//	TCCR0B &= ~(_BV(CS01));
//
//	OCR0A = 127; // Duty Cycle %50
//	OCR0B = 127; // Duty Cycle %50

	uint8_t txData[8] = { 0 };

	txData[0] = 0xAA;
	txData[1] = TCCR0A;
	txData[2] = TCCR0B;
	txData[3] = OCR0A;
	txData[4] = OCR0B;
	txData[5] = 0x13;
	txData[6] = 0x10;
	txData[7] = '\0';

	while(1)
	{
		usart_transmit(txData, 1);
	    _delay_ms(1000);
	}

	return 0;
}
