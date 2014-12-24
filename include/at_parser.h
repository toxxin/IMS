/*
 * at_parser.h
 *
 *  Created on: 08.09.2013
 *      Author: Anton
 */

#ifndef AT_PARSER_H_
#define AT_PARSER_H_


/**
 * 	\brief	Remove first chars in buffer.
 *
 * 	This function remove first chars from buffer, such as \0 \r \n.
 * 	Don't forget change size, cause array was changed!
 *
 * 	\retval _error_t
 */
int clearFirstCharsets(uint8_t **str, size_t size);


/**
 * 	\brief	Remove last chars in buffer.
 *
 * 	This function remove last chars from buffer, such as \0 \r \n.
 * 	Don't forget change size, cause array was changed!
 *
 * 	\retval _error_t
 */
//static void clearLastCharsets(const uint8_t *str, uint8_t *size);


/**
 * 	\brief	Count strings in buffer.
 *
 * 	This function count strings in buffer.
 * 	All strings divided by any \r \n \0 char or combinations of their!
 *
 * 	\param str		- Received string
 *	\param size		- String size
 *
 * 	\retval count
 */
//int countString(uint8_t *str, size_t size);


/**
 * 	\brief	Split received buffer to strings.
 *
 * 	This function splits received buffer into separated strings.
 *
 * 	\param buf		- Received buffer
 *	\param size		- Buffer size
 *	\param array	- Double pointer to goal strings
 *
 * 	\retval _error_t
 */
int splitString(uint8_t *buf, size_t size, uint8_t **array);

#endif /* AT_PARSER_H_ */
