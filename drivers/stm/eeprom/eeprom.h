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

#ifndef EEPROM_H_
#define EEPROM_H_


void writeEEPROMByte(uint32_t Address, uint8_t Data);
uint8_t readEEPROMByte(uint32_t Address);

void writeEEPROMBlock(uint8_t *src, uint32_t Address, size_t n);
void readEEPROMBlock(void *dst, uint32_t Address, size_t n);

#endif /* EEPROM_H_ */
