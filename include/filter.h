/*
 * filter.h
 *
 *  Created on: 21.09.2013
 *      Author: Anton
 */

#ifndef FILTER_H_
#define FILTER_H_

#include "ims.h"

#define NO_FILTER				0

#define NO_ARGS					BIT(31)


#define ALLOWED_CHARS_MASK		0x80

typedef enum e_AllowedCharsType
{
	ONLY_CHARS_ARGS		= 1,	/* only chars */
	ONLY_NUMBER_ARGS	= 2,	/* only numbers */
	ONLY_PHONE_ARGS		= 3,	/* + - ( ) and numbers */
	ONLY_ON_OFF			= 4		/* - + ON OFF */
} AllowedCharsType;


#define MAX_ARGS_LEN_MASK	0xF0FFFFFF
#define MIN_ARGS_LEN_MASK	0xFF0FFFFF

typedef enum e_MaxArgsLenType
{
	MAX_ARGS_LEN_1		= 1,
	MAX_ARGS_LEN_2		= 2,
	MAX_ARGS_LEN_3		= 3,
	MAX_ARGS_LEN_4		= 4,
	MAX_ARGS_LEN_5		= 5,
	MAX_ARGS_LEN_6		= 6,
	MAX_ARGS_LEN_7		= 7,
	MAX_ARGS_LEN_8		= 8,
	MAX_ARGS_LEN_9		= 9,
	MAX_ARGS_LEN_10		= 10,
	MAX_ARGS_LEN_11		= 11,
	MAX_ARGS_LEN_12		= 12
} MaxArgsLenType;

typedef enum e_MinArgsLenType
{
	MIN_ARGS_LEN_1		= 1,
	MIN_ARGS_LEN_2		= 2,
	MIN_ARGS_LEN_3		= 3,
	MIN_ARGS_LEN_4		= 4,
	MIN_ARGS_LEN_5		= 5,
	MIN_ARGS_LEN_6		= 6,
	MIN_ARGS_LEN_7		= 7,
	MIN_ARGS_LEN_8		= 8,
	MIN_ARGS_LEN_9		= 9
} MinArgsLenType;


#endif /* FILTER_H_ */
