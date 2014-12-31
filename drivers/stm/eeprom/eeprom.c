/**
 * 	\file
 *
 *	\brief EEPROM software driver interface.
 *
 *	\page License
 *
 *  Created on: 20.11.2011
 *      Author: Anton Glukhov
 */

#include "include/ims.h"
#include "eeprom.h"


char *DBGAddr = (char *)EEPROM_DBG_ADDR;
char *DBGAddrDec = (char *)EEPROM_DBG_ADDR;	/* p to every string in EEPROM */


void writeEEPROMByte(uint32_t Address, uint8_t Data)
{
	FLASH_Status FlashStatus;

	if (IS_FLASH_DATA_ADDRESS(Address))
	{
		DATA_EEPROM_Unlock();

//		FlashStatus = DATA_EEPROM_ProgramByte(Address, Data);
		FlashStatus = DATA_EEPROM_FastProgramByte(Address, Data);


		DATA_EEPROM_Lock();
	}
}


uint8_t readEEPROMByte(uint32_t Address)
{
	uint8_t res = 0;

	if (IS_FLASH_DATA_ADDRESS(Address))
	{
		DATA_EEPROM_Unlock();

		res = *(uint32_t *)(Address);

		DATA_EEPROM_Lock();
	}

	return res;
}


void writeEEPROMBlock(uint8_t *src, uint32_t Address, size_t n)
{
	/* Code here */
	/* Probable not supported */
	uint8_t i;
	FLASH_Status FlashStatus;

	if (IS_FLASH_DATA_ADDRESS(Address))
	{
		DATA_EEPROM_Unlock();

		for(i = 0; i < n; ++i)
			FlashStatus = DATA_EEPROM_FastProgramByte(Address++, *src++);

		DATA_EEPROM_Lock();
	}
}


void readEEPROMBlock(void *dst, uint32_t Address, size_t n)
{
	/* Code here */
	/* Probable not supported */
	uint8_t i;
	uint8_t buf[n];

	if (IS_FLASH_DATA_ADDRESS(Address))
	{
		DATA_EEPROM_Unlock();

		for (i = 0; i < n; ++i)
			buf[i] = *(uint32_t *)(Address++);

		DATA_EEPROM_Lock();
	}

	memcpy(dst, &buf[0], n);
}


int eeprom_putchar(uint8_t byte, FILE *stream)
{
	if ((byte != '\n') && (byte != '\r'))
	{
		writeEEPROMByte((uint8_t *)DBGAddr, byte);
		DBGAddr++;
	}
	else
	{
		DBGAddrDec += 0x0010;
		DBGAddr = DBGAddrDec;
	}

	return 0;
}
