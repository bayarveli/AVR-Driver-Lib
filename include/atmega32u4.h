/*
 * atmega32u4.h
 *
 *  Created on: Dec 22, 2019
 *      Author: veli-
 */

#ifndef INCLUDE_ATMEGA32U4_H_
#define INCLUDE_ATMEGA32U4_H_

#define GPIOB_BASE	(0x23)
#define GPIOC_BASE	(0x26)
#define GPIOD_BASE	(0x29)
#define GPIOE_BASE	(0x2C)
#define GPIOF_BASE	(0x2F)

/*------------- General Purpose Input/Output (GPIO) --------------------------*/
typedef struct
{
	uint8_t PORT;
	uint8_t DDR;
	uint8_t PIN;
} GpioTypeDef;


#define GPIOB	((GpioTypeDef *) GPIOB_BASE)
#define GPIOC	((GpioTypeDef *) GPIOC_BASE)
#define GPIOD	((GpioTypeDef *) GPIOD_BASE)
#define GPIOE	((GpioTypeDef *) GPIOE_BASE)
#define GPIOF	((GpioTypeDef *) GPIOF_BASE)


#endif /* INCLUDE_ATMEGA32U4_H_ */
