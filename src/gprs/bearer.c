/*
 * bear.c
 *
 *  Created on: 29.01.2014
 *      Author: anton
 */

#include <stdlib.h>
#include "ims.h"


//static int isBearerStart(Bearer *bearer)
//{
//
//}
//
//
//static int isBearerOpen(Bearer *bearer)
//{
//	//TODO:: static function for chechinkg
//	ret = getModuleData("AT+WIPBR?\r", tmpdata[20]);
//	if ((ret => 0) || (ret < 4))	// no data means no open bearers
//
//}


/*
	several bearer can be opened at the same time, but only one bearer can
	be started.
 */
static int openBearer(Bearer *bearer)
{
	int ret = E_OK;
	char tmpdata[20];
	char cmd[30];

	_DEBUG("Open bearer...");

	snprintf(&cmd[0], 30, "AT+WIPBR=%d,%d\r", GPRS_BEARER_CMD_OPEN, bearer->bid);

	ret = getModuleData(cmd, &tmpdata[0]);
	if (ret != 0)
		return -1;

	return ret;
}


int closeBearer(Bearer *bearer)
{
	int ret = E_OK;
	char tmpdata[20];
	char cmd[30];

	_DEBUG("Close bearer...");

	snprintf(&cmd[0], 30, "AT+WIPBR=%d,%d\r", GPRS_BEARER_CMD_CLOSE, bearer->bid);

	ret = getModuleData(cmd, &tmpdata[0]);
	if (ret != 0)
		return -1;

	return ret;
}


int startBearer(Bearer *bearer)
{
	int ret = E_OK;
	char tmpdata[40];
	char cmd[30];

	_DEBUG("Start bearer...");

	snprintf(&cmd[0], 30, "AT+WIPBR=%d,%d,%d\r", GPRS_BEARER_CMD_START, bearer->bid, 0);
	ret = getModuleDataTimeout(cmd, &tmpdata[0]);
	if (ret != 0)
		return -1;

	return ret;
}


int stopBearer(Bearer *bearer)
{
	int ret = E_OK;
	char tmpdata[20];
	char cmd[30];

	_DEBUG("Stop bearer...");

	snprintf(&cmd[0], 30, "AT+WIPBR=%d,%d\r", GPRS_BEARER_CMD_STOP, bearer->bid);
	ret = getModuleData(cmd, &tmpdata[0]);
	if (ret != 0)
		return -1;

	return ret;
}


int getBearerOption(Bearer *bearer, uint8_t option)
{
	int ret = E_OK;
	char tmpdata[20];
	char cmd[30];

	_DEBUG("Get bearer option...");

	snprintf(&cmd[0], 30, "AT+WIPBR=%d,%d,%d\r", GPRS_BEARER_CMD_GET, bearer->bid, option);
	ret = getModuleData(cmd, &tmpdata[0]);
	if (ret != 0)
		return -1;

	return ret;
}


int setBearerOption(Bearer *bearer, uint8_t option, char *value)
{
	int ret = E_OK;
	char tmpdata[20];
	char cmd[30];

	_DEBUG("Set bearer option...");

	snprintf(&cmd[0], 30, "AT+WIPBR=%d,%d,%d,\"%s\"\r", GPRS_BEARER_CMD_SET, bearer->bid, option, value);
	ret = getModuleData(&cmd[0], &tmpdata[0]);
	if (ret != 0)
		return -1;
//	else //mean 0 chars was received exept "OK" -> success!

	return ret;
}


// save options ?? -> move in setBearerOption
int saveBearer(Bearer *bearer)
{
	return 0;
}


void BearerInit(Bearer *bearer, Bearer_InitTypeDef *Bearer_InitStruct)
{
	//TODO:: asserts!

	strcpy(bearer->login, Bearer_InitStruct->login);
	strcpy(bearer->password, Bearer_InitStruct->password);
	strcpy(bearer->apn, Bearer_InitStruct->apn);
	bearer->bid = Bearer_InitStruct->type;

	bearer->open = openBearer;
	bearer->close = closeBearer;
	bearer->start = startBearer;
	bearer->stop = stopBearer;
	bearer->get = getBearerOption;
	bearer->set = setBearerOption;
}


int BearerDefInit(Bearer *bearer)
{
	int ret = E_OK;

	strcpy(bearer->login, GPRS_DEFAULT_LOGIN);
	strcpy(bearer->password, GPRS_DEFAULT_PASSWORD);
	strcpy(bearer->apn, GPRS_DEFAULT_APN);
	bearer->bid = GPRS_DEFAULT_TYPE;

	//TODO:: check ip stack
	//TODO:: check if already opened

	bearer->open = openBearer;
	bearer->close = closeBearer;
	bearer->start = startBearer;
	bearer->stop = stopBearer;
	bearer->get = getBearerOption;
	bearer->set = setBearerOption;

	ret = bearer->open(bearer);
	if (ret < 0)
	{
		_DEBUG("Can't open bearer!");
		return ret;
	}
	ret = bearer->set(bearer, GPRS_BEARER_OPT_LOGIN, bearer->login);
	ret = bearer->set(bearer, GPRS_BEARER_OPT_PASSWORD, bearer->password);
	ret = bearer->set(bearer, GPRS_BEARER_OPT_APN, bearer->apn);

	return ret;
}

