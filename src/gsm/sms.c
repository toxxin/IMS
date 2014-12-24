/*
 * sms.c
 *
 *  Created on: 08.09.2013
 *      Author: Anton
 */

#include <stdio.h>
#include <time.h>

#include "ims.h"


#if 0
int getBalance(char *msg)
{
	/* looking for first number */
	/* +CUSD: 2,"Balans: 253,83 rub. Gde druz'ya sejchas? Radar: NET SEKRETOV! *566*5# 2r/sut",1 */
	return E_OK;
}
#endif


int setMessageFormat(uint8_t mode)
{
	int ret;

	char *buf = (mode == SMS_TEXT_MODE) ? "AT+CMGF=1\r" : "AT+CMGF=0\r";

	ret = setModuleData(&buf[0]);
	if (ret == E_OK)
		mode ? SETBIT(gsm_module_config, CFG_TEXT_MODE) : CLEARBIT(gsm_module_config, CFG_TEXT_MODE);

	return ret;
}


int showTextModeParams(FunctionalState cmd)
{
	int ret = ERR_UNKNOWN;

	char *buf = (cmd == ENABLE) ? "AT+CSDH=1\r" : "AT+CSDH=0\r";

	ret = setModuleData(&buf[0]);

	return ret;
}


int setPreferStore(const char *store)
{
	int ret = ERR_UNKNOWN;

	return ret;
}


int sendSMSMessage(const char *message, const char *number)
{
	uint8_t i;
	int ret;
	_gsm_reg_t reg;
	char buf[30];
	char cmd[30];

	char sendbuf[SMS_MESSAGE_SIZE];

	ret = setMessageFormat(SMS_TEXT_MODE);
	if (ret != E_OK)
		return ret;

	snprintf(&cmd[0], 30, "AT+CMGS=\"%s\"\r", number);
	//TODO:: make number as a structure with country code + code + number

	_DEBUG("cmd - %s", cmd);

	ret = getModuleData(&cmd[0], (char *)&buf[0]);

	if (memcmp(buf[0], '>', 1) == 0)
	{
		/* typing message and add <CTRL-Z> -> 0x1a */
		memset(&buf[0], '\0', 30);
		_DEBUG("message...");
		USART_STR_TX(message, GSM_USART_PORT);
		ret = getModuleDataTimeout((char *)"\x1a", &buf[0]);

		_DEBUG("sms ret - %d", ret);
		_DEBUG("result - %s", &buf[0]);
	}
	else
	{
		_DEBUG("Can't get '>' to send message");
		return ERR_GSM_ERROR;
	}

	return E_OK;
}


int sendSMSMessageMock(const char *message, const char *number)
{
	_DEBUG("SMS for %s, with text: %s", number, message);

	return 0;
}


/* make as little bytes as possible */
//TODO:: refactoring - get list + parameter type of messages: new, readed, deleted, etc
int getSMSIdList(uint8_t *id, _sms_type_t sms_type)
{
	int ret = ERR_UNKNOWN;
	uint8_t data[MAX_STRING_LEN];
	int8_t num = 0;

	/* set pdu mode */
	ret = setMessageFormat(SMS_PDU_MODE);
	if (ret < 0)
	{
		_DEBUG("Can't setup SMS PDU mode", NULL);
		return ret;
	}

	/* with out additional data */
	ret = showTextModeParams(DISABLE);
	if (ret < 0)
	{
		_DEBUG("Can't setup show mode off for SMS", ret);
		return ret;
	}

	switch(sms_type)
	{
	case SMS_TYPE_REC_UNREAD:
		ret = getModuleData("AT+CMGL=0\r", (char*)&data[0]);
		break;
	case SMS_TYPE_REC_READ:
		ret = getModuleData("AT+CMGL=1\r", (char*)&data[0]);
		break;
	case SMS_TYPE_STO_UNSENT:
		ret = getModuleData("AT+CMGL=2\r", (char*)&data[0]);
		break;
	case SMS_TYPE_STO_SENT:
		ret = getModuleData("AT+CMGL=2\r", (char*)&data[0]);
		break;
	case SMS_TYPE_ALL:
		ret = getModuleData("AT+CMGL=2\r", (char*)&data[0]);
		break;
	default:
		return ERR_UNKNOWN_SMS_TYPE;
	}

	if (ret < 0)
	{
		if (ret == 0)
			return 0;
		_DEBUG("Can't get message - error: %d", ret);
	}
	else
	{
		uint8_t *p = &data[0];
		uint8_t *ptr;
		uint8_t *t = &data[ret - 1];

		while ((p = memmem(p, (t - p + 1), "+CMGL: ", strlen("+CMGL: "))) != NULL)
		{
			p += 7;
			ptr = memchr(p, ',', t- p + 1);
			*ptr = '\0';
			*id = atoi(p);
			id++;
			num++;
		}

		ret = num;
	}

	return ret;
}


//TODO:: carry this code out here
uint8_t multipleMemmem(const char *buf, uint8_t size, const char *pattern, uint8_t *array[])
{
	char *p = buf;
	char *t = buf + size - 1;
	uint8_t num = 0;
	size_t pattern_len = strlen(pattern);

	while ((p = memmem(p, (t - p + 1), pattern, pattern_len)) != NULL)
	{
		array[num++] = p;
		p += pattern_len;
	}

	return num;
}


_sms_t* initSMS(void)
{
	_sms_t *sms;

	sms = (_sms_t *)malloc(sizeof(_sms_t));
	sms->data = (uint8_t *)malloc(sizeof(uint8_t) * SMS_MESSAGE_SIZE);

	return sms;
}


void deinitSMS(_sms_t *sms)
{
	free(sms->data);
	free(sms);
}


void printdbgSMS(_sms_t *sms)
{
	_DEBUG("Got sms:", NULL);
	_DEBUG("     date - %d", sms->date);
	_DEBUG("     oa - %s", sms->oa);
	_DEBUG("     data - %s", sms->data);
}


int getSMSMessage(uint8_t index, _sms_t *sms)
{
	int ret = ERR_UNKNOWN;
	char cmd[12];
	char data[MAX_STRING_LEN];
	uint8_t *ptr1, *ptr2;

	/* set pdu mode */
	ret = setMessageFormat(SMS_TEXT_MODE);
	if (ret < 0)
	{
		_DEBUG("Can't setup SMS text mode", NULL);
		return ret;
	}

	/* with out additional data */
	ret = showTextModeParams(ENABLE);
	if (ret < 0)
	{
		_DEBUG("Can't setup show mode on for SMS", NULL);
		return ret;
	}

	sprintf(&cmd[0], "AT+CMGR=%d\r", index);

	ret = getModuleData(&cmd[0], &data[0]);
	if (ret < 0)
	{
		_DEBUG("Can't get message - error: %d", ret);
	}
	else
	{
		/* parseng sms */
		_DEBUG("parse sms - %s", &data[0]);

		ptr1 = memmem(&data[0], ret, "+CMGR: ", strlen("+CMGR: "));
		if (ptr1 == NULL)
			return ERR_READ_MESSAGE;

		ptr1 += 7;
		//TODO:: MUST change ret as range of searching
	/* origin number */
		ptr2 = memchr(ptr1, ',', ret);
		ptr2 += 2;

		ptr1 = memchr(ptr2, ',', ret);
		*(ptr1 - 1) = '\0';

		memcpy(sms->oa, ptr2, ptr1 - ptr2);

	/* date */
		ptr1 += 5;
		ptr2 = memchr(ptr1, '"', ret);

		*ptr2 = '\0';

		uint8_t *timestr = ptr1;

		struct tm timeptr;

		int hh, mm, ss;
		int y, m, d;

		sscanf(timestr, "%d/%d/%d,%d:%d:%d+", &y, &m, &d, &hh, &mm, &ss);

		timeptr.tm_year = y + 100;
		timeptr.tm_mon = m - 1;
		timeptr.tm_mday = d;
		timeptr.tm_hour = hh;
		timeptr.tm_min = mm;
		timeptr.tm_sec = ss;

		sms->date = mktime(&timeptr);

	/* data */
		ptr1 = memchr(ptr2, '\r', ret);
		while ((*ptr1 == '\r') || (*ptr1 == '\n'))
			ptr1++;

		strcpy(sms->data, ptr1);
		sms->sz = strlen(ptr1);
	}

	return ret;
}


//TODO:: add delete message by index
#if 0
static uint8_t deleteSMSMessage(uint8_t index)
{
	/* code here */
}
#endif


int deleteAllSMSMessage(void)
{
	char data[20];
	char cmd[12];
	uint8_t i;

	_DEBUG("Deleting all sms...");

	for(i = 0; i < SMS_SIM_NUMBER; ++i)
	{
		sprintf(&cmd[0], "AT+CMGD=%d\r", i);
		getModuleData(&cmd[0], &data[0]);
	}

	return E_OK;
}
