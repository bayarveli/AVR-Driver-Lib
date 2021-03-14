/*
 * timer.c
 *
 *  Created on: May 30, 2020
 *      Author: veli-
 */

#include <avr/interrupt.h>
#include "timer.h"

void timer_configure(TimerType timer, TimerWaveformType waveform)
{
	switch (timer)
	{
	case TIMER_0:
		;
		Timer0TypeDef *pTimer0 = TIMER0;
		pTimer0->sTCCR0A |= (waveform & 0x3);
		pTimer0->sTCCR0B |= (((waveform & 0x4) >> 2) << 3);

		break;
	case TIMER_1:
		;
		Timer13TypeDef *pTimer1 = TIMER1;
		pTimer1->sTCCR13A |= (waveform & 0x3);
		pTimer1->sTCCR13B |= (((waveform & 0xc) >> 2) << 3);

		break;
	case TIMER_3:
		;
		Timer13TypeDef *pTimer3 = TIMER3;
		pTimer3->sTCCR13A |= (waveform & 0x3);
		pTimer3->sTCCR13B |= (((waveform & 0xc) >> 2) << 3);

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


void timer_clear(TimerType timer)
{
	switch (timer)
	{
	case TIMER_0:
		;
		Timer0TypeDef *pTimer0 = TIMER0;
		pTimer0->sTCNT0 = 0;

		break;
	case TIMER_1:
		;
		Timer13TypeDef *pTimer1 = TIMER1;
		pTimer1->sTCNT13H = 0;
		pTimer1->sTCNT13L = 0;
		break;
	case TIMER_3:
		;
		Timer13TypeDef *pTimer3 = TIMER3;
		pTimer3->sTCNT13H = 0;
		pTimer3->sTCNT13L = 0;
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
		pTimer0->sTCCR0B |= (clock & 0x7);

		break;
	case TIMER_1:
		;
		Timer13TypeDef *pTimer1 = TIMER1;
		pTimer1->sTCCR13B |= (clock & 0x7);

		break;
	case TIMER_3:
		;
		Timer13TypeDef *pTimer3 = TIMER3;
		pTimer3->sTCCR13B |= (clock & 0x7);

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
		pTimer0->sTCCR0B &= ~(0x7);
		break;
	case TIMER_1:
		;
		Timer13TypeDef *pTimer1 = TIMER1;
		pTimer1->sTCCR13B &= ~(0x7);
		break;
	case TIMER_3:
		;
		Timer13TypeDef *pTimer3 = TIMER3;
		pTimer3->sTCCR13B &= ~(0x7);
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
			pTimer0->sTCCR0A |= ((mode & 0x3) << 6);
		}
		else if (OCR0_B == outCompReg)
		{
			pTimer0->sTCCR0A |= ((mode & 0x3) << 4);
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
			pTimer1->sTCCR13A |= ((mode & 0x3) << 6);
		}
		else if (OCR1_B == outCompReg)
		{
			pTimer1->sTCCR13A |= ((mode & 0x3) << 4);
		}
		else if (OCR1_C == outCompReg)
		{
			pTimer1->sTCCR13A |= ((mode & 0x3) << 2);
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
			pTimer3->sTCCR13A |= ((mode & 0x3) << 6);
		}
		else if (OCR3_B == outCompReg)
		{
			pTimer3->sTCCR13A |= ((mode & 0x3) << 4);
		}
		else if (OCR3_C == outCompReg)
		{
			pTimer3->sTCCR13A |= ((mode & 0x3) << 2);
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
			pTimer0->sOCR0A = (outCompRegValue & 0xFF);
		}
		else if (OCR0_B == outCompReg)
		{
			pTimer0->sOCR0B = (outCompRegValue & 0xFF);
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
			pTimer1->sOCR13AH = ((outCompRegValue & 0xFF00) >> 8);
			pTimer1->sOCR13AL = (outCompRegValue & 0xFF);
		}
		else if (OCR1_B == outCompReg)
		{
			pTimer1->sOCR13BH = ((outCompRegValue & 0xFF00) >> 8);
			pTimer1->sOCR13BL = (outCompRegValue & 0xFF);
		}
		else if (OCR1_C == outCompReg)
		{
			pTimer1->sOCR13CH = ((outCompRegValue & 0xFF00) >> 8);
			pTimer1->sOCR13CL = (outCompRegValue & 0xFF);
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
			pTimer3->sOCR13AH = ((outCompRegValue & 0xFF00) >> 8);
			pTimer3->sOCR13AL = (outCompRegValue & 0xFF);
		}
		else if (OCR3_B == outCompReg)
		{
			pTimer3->sOCR13BH = ((outCompRegValue & 0xFF00) >> 8);
			pTimer3->sOCR13BL = (outCompRegValue & 0xFF);
		}
		else if (OCR3_C == outCompReg)
		{
			pTimer3->sOCR13CH = ((outCompRegValue & 0xFF00) >> 8);
			pTimer3->sOCR13CL = (outCompRegValue & 0xFF);
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

void timer_setCallbackTimer3OCRAMatch(void (*ptr)())
{
	TimerIntMaskTypeDef *pTimerInt = 0;
	pTimerInt = TIMERINTMASK;

	timer3OCRAMatchCallback = ptr;

	pTimerInt->sTIMSK3 |= (1 << 1);
}

ISR(TIMER3_COMPA_vect)
{
	timer3OCRAMatchCallback();
}

