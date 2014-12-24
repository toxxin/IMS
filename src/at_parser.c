/*
 * at_parser.c
 *
 *  Created on: Jul 30, 2013
 *      Author: anton
 */

#include <stddef.h>
#include <stdlib.h>
#include  <string.h>

#include "include/ims.h"
#include "include/at_parser.h"

#define ERR_SOME_ERROR	48
#define ERR_NO_MEMORY	51


#define MIN_BUF_LEN			2
#define DROP_TERMINATED_STRING	/* don't parse if string without \r \n or \0 */


int clearFirstCharsets(uint8_t **str, const size_t size)
{
	uint8_t *tail = *str + size - 1;	/* pointer to last char */

	if (size <= MIN_BUF_LEN)
		return ERR_ARRAY_IS_EMPTY;

    while ((**str == '\r') || (**str == '\n') || (**str == '\0'))
    {
    	(*str)++;
    	if (tail <= *str)
    		return ERR_ARRAY_IS_EMPTY;
    }

    return E_OK;
}

#if 0
static void clearLastCharsets(const uint8_t *str, const size_t size)
{
	uint8_t *tail = str + size - 1;

	while ((*tail == '\r') || (*tail == '\n') || (*tail == '\0'))
	{
		*tail = '\0';
		tail--;
	}
}



int countString(uint8_t *str, size_t size)
{
	uint8_t *ptr = str;
	uint8_t *tail = ptr + size - 1;

	uint8_t count = 1;

	while (ptr < tail)
	{
		if ((*ptr == '\r' || *ptr == '\n' || *ptr == '\0')
			&& *(ptr + 1) != '\r' && *(ptr + 1) != '\n' && *(ptr + 1) != '\0')
		{
			count++;
		}
		ptr++;
	}

	return count;
}
#endif

int splitString(uint8_t *buf, size_t size, uint8_t **array)
{
	uint8_t *ptr = buf;
	uint8_t *tail = ptr + size - 1;

	uint8_t tmp;

	/* skip first null chars */
	tmp = clearFirstCharsets((uint8_t**)&ptr, size);
	if (tmp != E_OK)
		return tmp;
	size -= (ptr - buf); /* drop first chars -> changed size */

	/* first char */
	*(array++) = ptr;

	while(ptr <= tail)
	{
		while (*ptr != '\r' && *ptr != '\n' && *ptr != '\0')
		{
			ptr++;
			if (ptr > tail) return E_OK;
		}

		*ptr = '\0'; // mark end of line - substitution first \r\n char to \0
		while (*ptr == '\n' || *ptr == '\r' || *ptr == '\0')
		{
			ptr++;
			if (ptr > tail) return E_OK;
		}

		*(array++) = ptr;
	}

	return E_OK;
}

//TODO:: change this function to memmem()
uint8_t parseAtResponseOk(uint8_t *str, size_t size)
{
	uint8_t *ptr = str;

	uint8_t **array = (uint8_t **)malloc(sizeof(uint8_t*) * MAX_STRING_LEN);
	if (array == NULL)
		return ERR_MEM_AVAILABLE;

	splitString(ptr, size, array);

	if (CHKBIT_REG(gsm_module_config, CFG_ECHO_MODE))
	{
		if (!memcmp(array[1], "OK", 2))
			return E_OK;
		else
			return ERR_SOME_ERROR;
	}
	else
	{
		if (!memcmp(array[0], "OK", 2))
			return E_OK;
		else
			return ERR_SOME_ERROR;
	}

	return E_OK;
}
