/*
 * interrupt.h
 *
 *  Created on: May 25, 2020
 *      Author: veli-
 */

#ifndef INCLUDE_INTERRUPT_H_
#define INCLUDE_INTERRUPT_H_

#include <stdint.h>
#include <avr/interrupt.h>

void interrupt_enableGlobalInterrupt();
void interrupt_disableGlobalInterrupt();
void interrupt_setCallback();

#endif /* INCLUDE_INTERRUPT_H_ */
