/*
 * interrupt.c
 *
 *  Created on: May 25, 2020
 *      Author: veli-
 */


#include <x_interrupt.h>

void interrupt_enableGlobalInterrupt()
{
	sei();
}


void interrupt_disableGlobalInterrupt()
{
	cli();
}

