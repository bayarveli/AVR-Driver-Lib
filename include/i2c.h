/*
 * i2c.h
 *
 *  Created on: Jun 10, 2020
 *      Author: veli-
 */

#ifndef INCLUDE_I2C_H_
#define INCLUDE_I2C_H_

#include <stdint.h>
#include "atmega32u4.h"

void i2c_init();

/*
 * Transfer data is address info for read
 * Transfer data is address info
 */
uint8_t i2c_write(uint8_t slaveAddress, uint8_t wordAddress, uint8_t *dataToSent, uint8_t dataLen);
uint8_t i2c_read_after_write(uint8_t slaveAddress, uint8_t wordAddress, uint8_t *dataToReceive);

void i2c_deinit();


#endif /* INCLUDE_I2C_H_ */
