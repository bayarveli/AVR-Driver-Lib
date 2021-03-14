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
	REPEATEDSTART,	/* Repeated Start */
	SLAVEADDRESS, /* Slave address */
	REGISTERADDRESS, /* Word address */
	DATA,
	STOP
}TransmitStateType;

void i2c_init()
{
	I2CTypeDef *pI2C = 0;
	pI2C = I2C;

	// 100 kHz
	pI2C->sTWBR = 18;
	pI2C->sTWSR |= (DIVIDED_BY_1 & 0x3);
}


uint8_t i2c_write(uint8_t aSlaveAddress, uint8_t *aRegisterAddress,
		uint8_t aRegLen, uint8_t *aDataToSent, uint8_t aDataLen)
{
	TransmitStateType currState = START;
	uint8_t dataIdx = 0;
	uint8_t regIdx = 0;
	I2CTypeDef *pI2C = 0;
	pI2C = I2C;

	while (1)
	{
		switch (currState)
		{
		case START:
			// Clear TWINT Bit; Writing 1 clears TWINT
			pI2C->sTWCR |= (1 << 7);
			// Set TWSTA Bit
			pI2C->sTWCR |= (1 << 5);
			// Set TWEN Enable Bit
			pI2C->sTWCR |= (1 << 2);

			/*
			 * Wait for TWINT Flag is set. This indicates
			 * that the START condition has been transmitted.
			 */
			while (!(pI2C->sTWCR & (1 << 7)));

			/*
			 * Check value of TWI Status Register. If status
			 * is different from START, then return error.
			 */
			if (((pI2C->sTWSR & 0xF8)) != START_OK)
			{
				return ((pI2C->sTWSR & 0xF8));
			}
			else
			{
				currState = SLAVEADDRESS;
			}
			break;
		case SLAVEADDRESS:
			pI2C->sTWDR = ((aSlaveAddress & 0x7F) << 1) | (0 & 0x1);
			pI2C->sTWCR &= ~(1 << 5);
			pI2C->sTWCR |= (1 << 7);
			pI2C->sTWCR |= (1 << 2);

			while (!(pI2C->sTWCR & (1 << 7)));

			if (((pI2C->sTWSR & 0xF8)) != SLAVE_WRITE_ACK_OK)
			{
				return ((pI2C->sTWSR & 0xF8));
			}
			else
			{
				currState = REGISTERADDRESS;
			}
			break;
		case REGISTERADDRESS:

			for (regIdx = 0; regIdx < aRegLen; regIdx++)
			{
				pI2C->sTWDR = aRegisterAddress[regIdx];
				pI2C->sTWCR |= (1 << 7);
				pI2C->sTWCR |= (1 << 2);

				while (!(pI2C->sTWCR & (1 << 7)));

				if (((pI2C->sTWSR & 0xF8)) != DATA_WRITE_ACK_OK)
				{
					return ((pI2C->sTWSR & 0xF8));
				}
			}

			currState = DATA;

			break;
		case DATA:

			if (aDataLen > 1)
			{
				for (dataIdx = 0; dataIdx < aDataLen; dataIdx++)
				{
					pI2C->sTWDR = aDataToSent[dataIdx];
					pI2C->sTWCR |= (1 << 7);
					pI2C->sTWCR |= (1 << 2);

					while (!(pI2C->sTWCR & (1 << 7)));

					if (((pI2C->sTWSR & 0xF8)) != DATA_WRITE_ACK_OK)
					{
						return ((pI2C->sTWSR & 0xF8));
					}
				}
			}
			else
			{
				pI2C->sTWDR = *aDataToSent;
				pI2C->sTWCR |= (1 << 7);
				pI2C->sTWCR |= (1 << 2);

				while (!(pI2C->sTWCR & (1 << 7)));

				if (((pI2C->sTWSR & 0xF8)) != DATA_WRITE_ACK_OK)
				{
					return ((pI2C->sTWSR & 0xF8));
				}
			}

			currState = STOP;
			break;
		case STOP:

			// Clear TWINT Bit; Writing 1 clears TWINT
			pI2C->sTWCR |= (1 << 7);
			// Set TWSTO Bit
			pI2C->sTWCR |= (1 << 4);
			// Set TWEN Enable Bit
			pI2C->sTWCR |= (1 << 2);

			return NO_ERROR;
			break;
		default:
			break;
		}
	}
}

uint8_t i2c_read_after_write(uint8_t aSlaveAddress, uint8_t *aRegisterAddress,
		uint8_t aRegLen, uint8_t *aDataToReceive, uint8_t aDataLen)
{
	TransmitStateType currState = START;
	TransmitStateType previousState = START;
	uint8_t dataIdx = 0;
	uint8_t regIdx = 0;
	I2CTypeDef *pI2C = 0;

	pI2C = I2C;

	while(1)
	{
		switch(currState)
		{
		case START:
			// Set TWSTA Bit
			pI2C->sTWCR |= (1 << 5);
			// Clear TWINT Bit; Writing 1 clears TWINT
			pI2C->sTWCR |= (1 << 7);
			// Set TWEN Enable Bit
			pI2C->sTWCR |= (1 << 2);

			/*
			 * Wait for TWINT Flag is set. This indicates
			 * that the START condition has been transmitted.
			 */
			while (!(pI2C->sTWCR & (1 << 7)));

			/*
			 * Check value of TWI Status Register. If status
			 * is different from START, then return error.
			 */
			if (((pI2C->sTWSR & 0xF8) == START_OK) || ((pI2C->sTWSR & 0xF8) == REPEATED_START_OK))
			{
				currState = SLAVEADDRESS;
			}
			else
			{
				return ((pI2C->sTWSR & 0xF8));
			}

			break;
		case SLAVEADDRESS:
			if (previousState == START)
			{
				pI2C->sTWDR = ((aSlaveAddress & 0x7F) << 1) | (0 & 0x1);
			}
			else if (previousState == SLAVEADDRESS)
			{
				pI2C->sTWDR = ((aSlaveAddress & 0x7F) << 1) | (1 & 0x1);
			}

			// Set TWSTA Bit
			pI2C->sTWCR &= ~(1 << 5);
			pI2C->sTWCR |= (1 << 7);
			pI2C->sTWCR |= (1 << 2);

			while (!(pI2C->sTWCR & (1 << 7)));

			if (((pI2C->sTWSR & 0xF8) == SLAVE_WRITE_ACK_OK) || ((pI2C->sTWSR & 0xF8) == SLAVE_READ_ACK_OK))
			{

				if (previousState == START)
				{
					currState = REGISTERADDRESS;
				}
				else if (previousState == SLAVEADDRESS)
				{
					currState = DATA;
					previousState = START;
				}
			}
			else
			{
				return ((pI2C->sTWSR & 0xF8));
			}

			break;
		case REGISTERADDRESS:

			for (regIdx = 0; regIdx < aRegLen;  regIdx++)
			{
				pI2C->sTWDR = aRegisterAddress[regIdx];
				pI2C->sTWCR |= (1 << 7);
				pI2C->sTWCR |= (1 << 2);

				while (!(pI2C->sTWCR & (1 << 7)));

				if (((pI2C->sTWSR & 0xF8)) != DATA_WRITE_ACK_OK)
				{
					return ((pI2C->sTWSR & 0xF8));
				}
			}

			currState = START;
			previousState = SLAVEADDRESS;
			break;
		case DATA:

			for (dataIdx = 0; dataIdx < aDataLen; dataIdx++)
			{
				pI2C->sTWCR |= (1 << 7);
				pI2C->sTWCR |= (1 << 2);

				while (!(pI2C->sTWCR & (1 << 7)));

				aDataToReceive[dataIdx] = pI2C->sTWDR;

//				if (((pI2C->sTWSR & 0xF8)) == DATA_READ_ACK_OK)
//				{
//
//					currState = DATA;
//					previousState = SLAVEADDRESS;
//				}
//				else if ((((pI2C->sTWSR & 0xF8)) == DATA_READ_ACK_NOT_OK))
//				{
//					currState = STOP;
//					previousState = DATA;
//				}
//				else
//				{
//					return ((pI2C->sTWSR & 0xF8) >> 3);
//				}
				if (((pI2C->sTWSR & 0xF8)) == DATA_READ_ACK_OK)
				{

					currState = DATA;
					previousState = SLAVEADDRESS;
				}
				else if ((((pI2C->sTWSR & 0xF8)) == DATA_READ_ACK_NOT_OK))
				{
					currState = STOP;
					previousState = DATA;
				}
				else
				{
					return ((pI2C->sTWSR & 0xF8));
				}
			}

			break;
		case STOP:
			// Set TWSTO Bit
			pI2C->sTWCR |= (1 << 4);
			// Clear TWINT Bit; Writing 1 clears TWINT
			pI2C->sTWCR |= (1 << 7);
			// Set TWEN Enable Bit
			pI2C->sTWCR |= (1 << 2);
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
