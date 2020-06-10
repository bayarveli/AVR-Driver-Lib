/*
 * atmega32u4.h
 *
 *  Created on: Dec 22, 2019
 *      Author: veli-
 */

#ifndef INCLUDE_ATMEGA32U4_H_
#define INCLUDE_ATMEGA32U4_H_

// userconfig.h gibi bir dosyaya taşınacak.
#define CLOCK_FREQ 			(16000000UL)

#define GPIOB_BASE			(0x23)
#define GPIOC_BASE			(0x26)
#define GPIOD_BASE			(0x29)
#define GPIOE_BASE			(0x2C)
#define GPIOF_BASE			(0x2F)

#define USART1_BASE			(0xC8)

#define TIMERINTFLAG_BASE 	(0x15)
#define TIMERINTMASK_BASE 	(0x6E)
#define TIMER0_BASE			(0x44)
#define TIMER1_BASE			(0x80)
#define TIMER3_BASE			(0x90)
#define TIMER4A_BASE		(0xBE)
#define TIMER4B_BASE		(0xCF)

/*------------- General Purpose Input/Output (GPIO) --------------------------*/
typedef struct
{
	uint8_t PIN;
	uint8_t DDR;
	uint8_t PORT;
} GpioTypeDef;

/*------------- USART --------------------------*/
typedef struct
{
	uint8_t UCSRA;
	uint8_t UCSRB;
	uint8_t UCSRC;
	uint8_t UCSRD;
	uint8_t UBRRL;
	uint8_t UBRRH;
	uint8_t UDR;
} UsartTypeDef;

/*------------- TIMERS --------------------------*/

typedef struct
{
	uint8_t TIFR0;
	uint8_t TIFR1;
	uint8_t Reserved;
	uint8_t TIFR3;
	uint8_t TIFR4;
} TimerIntFlagTypeDef;

typedef struct
{
	uint8_t TIMSK0;
	uint8_t TIMSK1;
	uint8_t Reserved;
	uint8_t TIMSK3;
	uint8_t TIMSK4;
} TimerIntMaskTypeDef;

typedef struct
{
	uint8_t TCCR0A;
	uint8_t TCCR0B;
	uint8_t TCNT0;
	uint8_t OCR0A;
	uint8_t OCR0B;
} Timer0TypeDef;

typedef struct
{
	uint8_t TCCR13A;
	uint8_t TCCR13B;
	uint8_t TCCR13C;
	uint8_t Reserved;
	uint8_t TCNT13L;
	uint8_t TCNT13H;
	uint8_t ICR13L;
	uint8_t ICR13H;
	uint8_t OCR13AL;
	uint8_t OCR13AH;
	uint8_t OCR13BL;
	uint8_t OCR13BH;
	uint8_t OCR13CL;
	uint8_t OCR13CH;
} Timer13TypeDef;


typedef struct
{
	uint8_t TCNT4;
	uint8_t TC4H;
	uint8_t TCCR4A;
	uint8_t TCCR4B;
	uint8_t TCCR4C;
	uint8_t TCCR4D;
	uint8_t TCCR4E;
} Timer4aTypeDef;

typedef struct
{
	uint8_t OCR4A;
	uint8_t OCR4B;
	uint8_t OCR4C;
	uint8_t OCR4D;
	uint8_t Reserved;
	uint8_t DT4;
} Timer4bTypeDef;

#define GPIOB	((GpioTypeDef *) GPIOB_BASE)
#define GPIOC	((GpioTypeDef *) GPIOC_BASE)
#define GPIOD	((GpioTypeDef *) GPIOD_BASE)
#define GPIOE	((GpioTypeDef *) GPIOE_BASE)
#define GPIOF	((GpioTypeDef *) GPIOF_BASE)

#define USART1	((UsartTypeDef *) USART1_BASE)

#define TIMERINTFLAG 	((TimerIntFlagTypeDef *) TIMERINTFLAG_BASE)
#define TIMERINTMASK 	((TimerIntMaskTypeDef *) TIMERINTMASK_BASE)
#define TIMER0			((Timer0TypeDef *) TIMER0_BASE)
#define TIMER1			((Timer13TypeDef *) TIMER1_BASE)
#define TIMER3			((Timer13TypeDef *) TIMER3_BASE)
#define TIMER4A			((Timer4aTypeDef *) TIMER4A_BASE)
#define TIMER4B			((Timer4bTypeDef *) TIMER4B_BASE)

#endif /* INCLUDE_ATMEGA32U4_H_ */
