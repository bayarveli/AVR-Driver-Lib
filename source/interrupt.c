/*
 * interrupt.c
 *
 *  Created on: May 25, 2020
 *      Author: veli-
 */


#include "interrupt.h"

void interrupt_enableGlobalInterrupt()
{
	sei();
}


void interrupt_disableGlobalInterrupt()
{
	cli();
}

void interrupt_setCallback();
