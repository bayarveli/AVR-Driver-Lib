/*
 * timer.c
 *
 *  Created on: May 30, 2020
 *      Author: veli-
 */

#include "timer.h"

void timer_configure(TimerType timer, TimerWaveformType waveform)
{
	switch (timer)
	{
	case TIMER_0:
		;
		Timer0TypeDef *pTimer0 = TIMER0;
		pTimer0->TCCR0A |= (waveform & 0x3);
		pTimer0->TCCR0B |= (((waveform & 0x4) >> 2) << 3);

		break;
	case TIMER_1:
		;
		Timer13TypeDef *pTimer1 = TIMER1;

		break;
	case TIMER_3:
		;
		Timer13TypeDef *pTimer3 = TIMER3;

		break;
	case TIMER_4:
		;
		Timer4aTypeDef *pTimer4 = TIMER4A;

		break;
	default:
		// defensive
		break;
	}
}

void timer_start(TimerType timer, TimerClockSelect clock)
{
	switch (timer)
	{
	case TIMER_0:
		;
		Timer0TypeDef *pTimer0 = TIMER0;
		pTimer0->TCCR0B |= (clock & 0x7);

		break;
	case TIMER_1:
		;
		Timer13TypeDef *pTimer1 = TIMER1;



		break;
	case TIMER_3:
		;
		Timer13TypeDef *pTimer3 = TIMER3;

		break;
	case TIMER_4:
		;
		Timer4aTypeDef *pTimer4 = TIMER4A;

		break;
	default:
		// defensive
		break;
	}
}

void timer_stop(TimerType timer)
{
	switch (timer)
	{
	case TIMER_0:
		;
		Timer0TypeDef *pTimer0 = TIMER0;
		pTimer0->TCCR0B &= ~(0x7);
		break;
	case TIMER_1:
		;
		Timer13TypeDef *pTimer1 = TIMER1;



		break;
	case TIMER_3:
		;
		Timer13TypeDef *pTimer3 = TIMER3;

		break;
	case TIMER_4:
		;
		Timer4aTypeDef *pTimer4 = TIMER4A;

		break;
	default:
		// defensive
		break;
	}
}



void timer_ocr_configure(TimerType timer, OutputCompareType outCompReg, OutputCompareMode mode)
{
	switch (timer)
	{
	case TIMER_0:
		;
		Timer0TypeDef *pTimer0 = TIMER0;

		if (OCR0_A == outCompReg)
		{
			pTimer0->TCCR0A |= ((mode & 0x3) << 6);
		}
		else if (OCR0_B == outCompReg)
		{
			pTimer0->TCCR0A |= ((mode & 0x3) << 4);
		}
		else
		{
			// defensive
		}

		break;
	case TIMER_1:
		;
		Timer13TypeDef *pTimer1 = TIMER1;

		if (OCR1_A == outCompReg)
		{

		}
		else if (OCR1_B == outCompReg)
		{

		}
		else if (OCR1_C == outCompReg)
		{

		}
		else
		{
			// defensive
		}

		break;
	case TIMER_3:
		;
		Timer13TypeDef *pTimer3 = TIMER3;

		if (OCR3_A == outCompReg)
		{

		}
		else if (OCR3_B == outCompReg)
		{

		}
		else if (OCR3_C == outCompReg)
		{

		}
		else
		{
			// defensive
		}
		break;
	case TIMER_4:
		;
		Timer4aTypeDef *pTimer4 = TIMER4A;

		// TODO: To be implemented

		break;
	default:
		// defensive
		break;
	}
}


void timer_set_ocr_value(TimerType timer, OutputCompareType outCompReg, uint16_t outCompRegValue)
{
	switch (timer)
	{
	case TIMER_0:
		;
		Timer0TypeDef *pTimer0 = TIMER0;

		if (OCR0_A == outCompReg)
		{
			pTimer0->OCR0A = (outCompRegValue & 0xFF);
		}
		else if (OCR0_B == outCompReg)
		{
			pTimer0->OCR0B = (outCompRegValue & 0xFF);
		}
		else
		{
			// defensive
		}

		break;
	case TIMER_1:
		;
		Timer13TypeDef *pTimer1 = TIMER1;

		if (OCR1_A == outCompReg)
		{
			pTimer1->OCR13AL = (outCompRegValue & 0xFF);
			pTimer1->OCR13AH = ((outCompRegValue & 0xFF00) >> 8);
		}
		else if (OCR1_B == outCompReg)
		{
			pTimer1->OCR13BL = (outCompRegValue & 0xFF);
			pTimer1->OCR13BH = ((outCompRegValue & 0xFF00) >> 8);
		}
		else if (OCR1_C == outCompReg)
		{
			pTimer1->OCR13CL = (outCompRegValue & 0xFF);
			pTimer1->OCR13CH = ((outCompRegValue & 0xFF00) >> 8);
		}
		else
		{
			// defensive
		}

		break;
	case TIMER_3:
		;
		Timer13TypeDef *pTimer3 = TIMER3;

		if (OCR3_A == outCompReg)
		{
			pTimer3->OCR13AL = (outCompRegValue & 0xFF);
			pTimer3->OCR13AH = ((outCompRegValue & 0xFF00) >> 8);
		}
		else if (OCR3_B == outCompReg)
		{
			pTimer3->OCR13BL = (outCompRegValue & 0xFF);
			pTimer3->OCR13BH = ((outCompRegValue & 0xFF00) >> 8);
		}
		else if (OCR3_C == outCompReg)
		{
			pTimer3->OCR13CL = (outCompRegValue & 0xFF);
			pTimer3->OCR13CH = ((outCompRegValue & 0xFF00) >> 8);
		}
		else
		{
			// defensive
		}
		break;
	case TIMER_4:
		;
		Timer4aTypeDef *pTimer4 = TIMER4A;

		// TODO: To be implemented

		break;
	default:
		// defensive
		break;
	}
}
