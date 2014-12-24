/*
 * usart.h
 *
 *  Created on: 03.05.2013
 *      Author: Anton
 */

#ifndef USART_H_
#define USART_H_

/**
 * 	\brief	USART configuration
 *
 * 	This function initialize USART module.
 *
 *	\return void
 */
void USART_Configuration(void);


/**
 * 	\brief	USART string sending
 *
 * 	This function sends a null-terminated string by USART module.
 *
 *	\return void
 */
uint8_t USART_STR_TX(unsigned char *data, USART_TypeDef* USARTx);


/**
 * 	\brief	USART buf sending
 *
 * 	This function sends a buffer by USART module.
 *
 *	\return void
 */
void USART_BUF_TX(uint8_t *data, size_t len, USART_TypeDef* USARTx);


/**
 * 	\brief	USART getBuf
 *
 * 	This function receive a buffer from USART channel.
 *
 * 	\param buf	- Pointer to the allocated buffer
 * 	\param sz	- Size of the allocated buffer
 * 	\param num	- USART channel number
 * 	\param bt	- Timeout before receiving first char
 * 	\param at	- Timeout after receiving last char
 *
 *	\return The number of bytes received or error
 */
int getBuf(uint8_t *buf, size_t sz, USART_TypeDef* num, uint16_t bt, uint16_t at);


/**
 * 	\brief	USART lines status
 *
 * 	This function prints USART lines states.
 *
 *	\return void
 */
void USART_LINES_STATUS(void);

#endif /* USART_H_ */
