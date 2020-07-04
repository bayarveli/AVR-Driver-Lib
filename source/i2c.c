/*
 * i2c.c
 *
 *  Created on: Jun 10, 2020
 *      Author: veli-
 */


#include "i2c.h"

typedef enum
{
	DIVIDED_BY_1 = 1,
	DIVIDED_BY_4 = 4,
	DIVIDED_BY_16 = 16,
	DIVIDED_BY_64 = 64
}PrescalerBitsType;

typedef enum
{
	NO_ERROR = 0x0,
	START_OK = 0x08,
	REPEATED_START_OK = 0x10,
	SLAVE_WRITE_ACK_OK = 0x18,
	SLAVE_WRITE_ACK_NOT_OK = 0x20,
	DATA_WRITE_ACK_OK = 0x28,
	DATA_WRITE_ACK_NOT_OK = 0x30,
	ARBITRATION_LOST = 0x38,
	SLAVE_READ_ACK_OK = 0x40,
	SLAVE_READ_ACK_NOT_OK = 0x48,
	DATA_READ_ACK_OK = 0x50,
	DATA_READ_ACK_NOT_OK = 0x58
}MasterModeStatusCode;

// TODO: Receiver Mode Status Codes will be added later.

typedef enum
{
	START,
	RSTART,	/* Repeated Start */
	SADDRESS, /* Slave address */
	WADDRESS, /* Word address */
	DATA,
	STOP
}TransmitStateType;

void i2c_init()
{
	I2CTypeDef *pI2C = 0;
	pI2C = I2C;

	// 100 kHz
	pI2C->TWBR = 18;
	pI2C->TWSR |= (DIVIDED_BY_1 & 0x3);
}


uint8_t i2c_write(uint8_t slaveAddress, uint8_t wordAddress, uint8_t *dataToSent, uint8_t dataLen)
{
	TransmitStateType currState = START;
	I2CTypeDef *pI2C = 0;
	pI2C = I2C;

	while (1)
	{
		switch (currState)
		{
		case START:
			// Clear TWINT Bit; Writing 1 clears TWINT
			pI2C->TWCR |= (1 << 7);
			// Set TWSTA Bit
			pI2C->TWCR |= (1 << 5);
			// Set TWEN Enable Bit
			pI2C->TWCR |= (1 << 2);

			/*
			 * Wait for TWINT Flag is set. This indicates
			 * that the START condition has been transmitted.
			 */
			while (!(pI2C->TWCR & (1 << 7)));

			/*
			 * Check value of TWI Status Register. If status
			 * is different from START, then return error.
			 */
			if (((pI2C->TWSR & 0xF8)) != START_OK) {
				return ((pI2C->TWSR & 0xF8));
			} else {
				currState = SADDRESS;
			}
			break;
		case SADDRESS:
			pI2C->TWDR = ((slaveAddress & 0x7F) << 1) | (0 & 0x1);
			pI2C->TWCR &= ~(1 << 5);
			pI2C->TWCR |= (1 << 7);
			pI2C->TWCR |= (1 << 2);

			while (!(pI2C->TWCR & (1 << 7)));

			if (((pI2C->TWSR & 0xF8)) != SLAVE_WRITE_ACK_OK)
			{
				return ((pI2C->TWSR & 0xF8));
			}
			else
			{
				currState = WADDRESS;
			}
			break;
		case WADDRESS:
			pI2C->TWDR = wordAddress;
			pI2C->TWCR |= (1 << 7);
			pI2C->TWCR |= (1 << 2);

			while (!(pI2C->TWCR & (1 << 7)));

			if (((pI2C->TWSR & 0xF8)) != DATA_WRITE_ACK_OK)
			{
				return ((pI2C->TWSR & 0xF8));
			}
			else
			{
				currState = DATA;
			}
			break;
		case DATA:
			pI2C->TWDR = *(dataToSent++);
			pI2C->TWCR |= (1 << 7);
			pI2C->TWCR |= (1 << 2);

			while (!(pI2C->TWCR & (1 << 7)));

			if (((pI2C->TWSR & 0xF8)) != DATA_WRITE_ACK_OK)
			{
				return ((pI2C->TWSR & 0xF8));
			}
			else
			{
				currState = STOP;
			}

			break;
		case STOP:

			// Clear TWINT Bit; Writing 1 clears TWINT
			pI2C->TWCR |= (1 << 7);
			// Set TWSTO Bit
			pI2C->TWCR |= (1 << 4);
			// Set TWEN Enable Bit
			pI2C->TWCR |= (1 << 2);

			return NO_ERROR;
			break;
		default:
			break;
		}
	}
}

uint8_t i2c_read_after_write(uint8_t slaveAddress, uint8_t wordAddress, uint8_t *dataToReceive)
{
	TransmitStateType currState = START;
	TransmitStateType previousState = START;
	uint8_t dataIdx = 0;
	I2CTypeDef *pI2C = 0;

	pI2C = I2C;

	while(1)
	{
		switch(currState)
		{
		case START:
			// Set TWSTA Bit
			pI2C->TWCR |= (1 << 5);
			// Clear TWINT Bit; Writing 1 clears TWINT
			pI2C->TWCR |= (1 << 7);
			// Set TWEN Enable Bit
			pI2C->TWCR |= (1 << 2);

			/*
			 * Wait for TWINT Flag is set. This indicates
			 * that the START condition has been transmitted.
			 */
			while (!(pI2C->TWCR & (1 << 7)));

			/*
			 * Check value of TWI Status Register. If status
			 * is different from START, then return error.
			 */
			if (((pI2C->TWSR & 0xF8) == START_OK) || ((pI2C->TWSR & 0xF8) == REPEATED_START_OK))
			{
				currState = SADDRESS;
			}
			else
			{
				return ((pI2C->TWSR & 0xF8) >> 3);
			}

			break;
		case SADDRESS:
			if (previousState == START)
			{
				pI2C->TWDR = ((slaveAddress & 0x7F) << 1) | (0 & 0x1);
			}
			else if (previousState == SADDRESS)
			{
				pI2C->TWDR = ((slaveAddress & 0x7F) << 1) | (1 & 0x1);
			}

			// Set TWSTA Bit
			pI2C->TWCR &= ~(1 << 5);
			pI2C->TWCR |= (1 << 7);
			pI2C->TWCR |= (1 << 2);

			while (!(pI2C->TWCR & (1 << 7)));

			if (((pI2C->TWSR & 0xF8) == SLAVE_WRITE_ACK_OK) || ((pI2C->TWSR & 0xF8) == SLAVE_READ_ACK_OK))
			{

				if (previousState == START)
				{
					currState = WADDRESS;
				}
				else if (previousState == SADDRESS)
				{
					currState = DATA;
					previousState = START;
				}
			}
			else
			{
				return ((pI2C->TWSR & 0xF8) >> 3);
			}

			break;
		case WADDRESS:
			pI2C->TWDR = wordAddress;
			pI2C->TWCR |= (1 << 7);
			pI2C->TWCR |= (1 << 2);

			while (!(pI2C->TWCR & (1 << 7)));

			if (((pI2C->TWSR & 0xF8)) != DATA_WRITE_ACK_OK)
			{
				return ((pI2C->TWSR & 0xF8) >> 3);
			}
			else
			{
				currState = START;
				previousState = SADDRESS;
			}

			break;
		case DATA:


			pI2C->TWCR |= (1 << 7);
			pI2C->TWCR |= (1 << 2);

			while (!(pI2C->TWCR & (1 << 7)));

			*(dataToReceive++) = pI2C->TWDR;

			if (((pI2C->TWSR & 0xF8)) == DATA_READ_ACK_OK)
			{

				currState = DATA;
				previousState = SADDRESS;
			}
			else if ((((pI2C->TWSR & 0xF8)) == DATA_READ_ACK_NOT_OK))
			{
				currState = STOP;
				previousState = DATA;
			}
			else
			{
				return ((pI2C->TWSR & 0xF8) >> 3);
			}

			break;
		case STOP:
			// Set TWSTO Bit
			pI2C->TWCR |= (1 << 4);
			// Clear TWINT Bit; Writing 1 clears TWINT
			pI2C->TWCR |= (1 << 7);
			// Set TWEN Enable Bit
			pI2C->TWCR |= (1 << 2);


			return NO_ERROR;

			break;
		default:
			break;
		}
	}
}


void i2c_deinit()
{

}
