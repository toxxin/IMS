/*
 * sms_parser.c
 *
 *  Created on: 21.09.2013
 *      Author: Anton
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "ims.h"
#include "common.h"

#include "sms_cmd_parser.h"


	createCmdList();


uint8_t dropChar(uint8_t *buf, size_t size, char c)
{
	uint8_t *head = buf;
	uint8_t *tail = buf + size - 1;
	uint8_t drop_count = 0;

	while (buf <= tail)
	{
		if (*buf == c)
		{
			if (buf < tail)	/* condition if last charset in string is space */
				memmove(buf, buf + 1, tail - buf);
			tail--; drop_count++;
		}
		/* protection from sequence more than 1 char */
		/* don't increase if next or now current char is space */
		if (*buf != c)
			buf++;
	}

	buf = head;

	return drop_count;
}


uint8_t cleanBuf(uint8_t *buf, size_t size, uint8_t chars)
{
	uint8_t drop_cout = 0;

	if (CHKBIT_REG(chars, CHAR_SPACE))
		drop_cout += dropChar(buf, size, ' ');

	if (CHKBIT_REG(chars, CHAR_NEW_LINE))
		drop_cout += dropChar(buf, size, '\n');

	if (CHKBIT_REG(chars, CHAR_NEW_CARRIAGE))
		drop_cout += dropChar(buf, size, '\r');

	if (CHKBIT_REG(chars, CHAR_NULL_TERMINATE))
		drop_cout += dropChar(buf, size, '\0');

	return drop_cout;
}


int checkSMSPass(const uint8_t *message, const size_t size)
{
	int ret = E_OK;
	char pass[SMS_PASS_LEN];

	ret = getPass(&pass[0]);
	if (ret < 0)
	{
		_DEBUG("Can't read sms pass from EEPROM");
		return ret;
	}

	uint8_t *ppass = memmem(message, size, pass, SMS_PASS_LEN);
	if ((ppass == NULL) || (ppass != message))
		ret = ERR_INCORRECT_PASS;
	else
		ret = SMS_PASS_LEN;

	return ret;
}


static int checkTrustNumber(uint8_t *number)
{
	return 0;
}


/* Warning! MUST be null-terminated string */
int splitCommand(uint8_t *buf, const size_t size, const char dlm,  uint8_t *array[])
{
	uint8_t *tail = buf + size - 1;
	int substr_count = 0;

	if (size < 2)
		return -1;

	if (*tail != '\0')
		_DEBUG("WARNING!!! String is not null-terminated");

	if (*buf != dlm)
		array[substr_count++] = buf;

	while ((tail - buf) > 0)
	{
		if (*buf == dlm)
		{
			*buf++ = '\0';

			while ((*buf == dlm) && ((tail - buf) > 0))
				buf++;

			if ((tail - buf) <= 0)
				return substr_count;
			else
				array[substr_count++] = buf;
		}

		buf++;
	}

	return substr_count;
}


void convertToUpper(uint8_t *str)
{
	for(; *str != '\0'; str++)
		*str = toupper(*str);
}


_cmd_t *getCmdBySmsName(uint8_t *cmd)
{
	uint8_t i;

	size_t cmd_list_size = CMD_NUMBERS;

	for (i = 0; i < cmd_list_size; ++i)
	{
		if (memcmp(cmd_list[i].sms, cmd, strlen((char *)cmd_list[i].sms)) == 0)
			return &cmd_list[i];
	}

	return NULL;
}


_cmd_t *getCmdByWebName(_cmd_t *cmd_list, size_t array_size, uint8_t *cmd)
{
	/* code here -> copy getCmdBySmsName() */
	return E_OK;
}


int parseSmsCmd(TaskQueue *queue, _sms_t *sms, uint8_t inception_id, TMapList *tmap)
{
	uint8_t i;
	int ret = E_OK;
	_cmd_t *cmd = NULL;
	int(*handler)(void *, void *);
	uint8_t *args = NULL;
	size_t size = (sms->sz) + 1; /* size+1 for additional \0 during parsing */

	uint8_t *origin_buf = malloc(sizeof(uint8_t) * size); /* size+1 for additional \0 during parsing */
	if(origin_buf == NULL)
		return ERR_MEM_AVAILABLE;

	uint8_t *buf = origin_buf;

	memcpy(buf, sms->data, sms->sz);
	*(buf + size - 1) = '\0'; /* size+1 for additional \0 during parsing */

	size -= cleanBuf(buf, size, CHAR_SPACE | CHAR_NEW_LINE | CHAR_NEW_CARRIAGE);
	_DEBUG("Incoming commands: %s", buf);

	if (size < 1)
	{
		ret = ERR_FAIL_MESSAGE;
		goto buf_free;
	}

	/* check pass and trust number */
	ret = checkSMSPass(buf, size);
	if (ret < 0)
	{
		if (checkTrustNumber(sms->oa))
		{
			_DEBUG("Can't identify pass.");
			goto buf_free;
		}
	}
	else
	{
		buf = buf + SMS_PASS_LEN;
		size -= SMS_PASS_LEN;
	}


	uint8_t *pcmds[SMS_MAX_CMD];
	ret = splitCommand(buf, size, SMS_CMD_DELIMITER, &pcmds[0]);
	if (ret < 0)
		goto buf_free;

	uint8_t cmd_count = ret;

	for (i = 0; i < cmd_count; ++i)
	{
		handler = NULL;
		args = NULL;

		convertToUpper(pcmds[i]);

		if ((cmd = getCmdBySmsName(pcmds[i])) == NULL)
		{
			_DEBUG("Unknown command: %s", buf);
			ret = ERR_UNK_PACKAGE;
			goto err_cmd;
		}

		if (strlen(pcmds[i] + cmd->cmd_len) != 0)
		{
			args = (char *) realloc(args, strlen(pcmds[i] + cmd->cmd_len) + 1);
			strcpy(args, pcmds[i] + cmd->cmd_len);
		}

		handler = cmd->handler;

		/* return new task id */
		ret = queue->add(queue, cmd->name, cmd->id, sms->date, handler, args);
		if (ret < 0) /* ret is task id -> add this to some map table */
			_DEBUG("some problems with adding - %d", ret);
		else
			tmap->add(tmap, inception_id, (uint8_t)ret);

		free(args);
	}

	goto buf_free;

err_cmd:	/* clear all command for current sms */
	tmap->delByIncId(tmap, inception_id);

buf_free:
	free(origin_buf);

	return ret;
}
