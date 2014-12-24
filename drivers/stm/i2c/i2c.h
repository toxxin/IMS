/*
 * i2c.h
 *
 *  Created on: 26.09.2013
 *      Author: Anton
 */

#ifndef I2C_H_
#define I2C_H_

#include "ims.h"

#define I2CTimeout ((uint32_t)0x10000)

/* Maximum Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will
   not remain stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#define I2CTimeout				((uint32_t)0x1000)
#define LONG_I2CTimeout			((uint32_t)(10 * I2CTimeout))

void I2C_Configuration(void);

int I2C_read(I2C_TypeDef*, uint8_t, uint8_t, uint8_t*);

int I2C_write(I2C_TypeDef*, uint8_t, uint8_t, uint8_t);

#endif /* I2C_H_ */
