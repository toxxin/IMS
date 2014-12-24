/*
 * common.c
 *
 *  Created on: 28.10.2013
 *      Author: Anton
 */

#include "ims.h"


int getLocation(char *location)
{
	int ret = E_OK;

	_DEBUG("Getting location...");

	strcpy(location, "54.324313,55.12323");
	ret = strlen(location);

	return ret;
}


int getLocationAdapter(char *result)
{
	int ret = E_OK;
	char tmp[30];

	_DEBUG("Getting location adapter...");

	ret = getLocation(&tmp[0]);
	if (ret < 0)
		strcpy(result, "Can't get location");
	else
	{
		strcpy(result, "Location: ");
		strcat(result, tmp);

		ret = strlen(result);
	}

	return ret;
}


int hdlrLocation(char *args, char *result)
{
	int ret = E_OK;
	char tmp[30];

	_DEBUG("Getting location handler...");

	if (args != NULL)
	{
		strcpy(result, "Too much arguments");
		return ERR_INCORRECT_CMD_ARGS;
	}

	ret = getLocation(&tmp[0]);
	if (ret < 0)
		strcpy(result, "Can't get location");
	else
	{
		strcpy(result, "Location: ");
		strcat(result, &tmp[0]);
	}

	return 0;
}


int getGSMLocation(char *location)
{
	int ret = E_OK;

	_DEBUG("Getting GSM location...");

	strcpy(location, "54.324313,55.12323");
	ret = strlen(location);

	return ret;
}


int getGSMLocationAdapter(char *result)
{
	int ret = E_OK;
	char tmp[30];

	_DEBUG("Getting gsm location adapter...");

	ret = getGSMLocation(&tmp[0]);
	if (ret < 0)
		strcpy(result, "Can't get location");
	else
	{
		strcpy(result, "Location: ");
		strcat(result, tmp);

		ret = strlen(result);
	}

	return ret;
}


int hdlrGSMLocation(char *args, char *result)
{
	int ret = E_OK;
	char tmp[30];

	_DEBUG("Getting gsm location handler...");

	if (args != NULL)
		return ERR_INCORRECT_CMD_ARGS;

	ret = getGSMLocation(&tmp[0]);
	if (ret < 0)
		strcpy(result, "Can't get location");
	else
	{
		strcpy(result, "Location: ");
		strcat(result, tmp);
	}

	return 0;
}


int getBattery(void)
{
	//TODO:: add ADC realisation

	return 40;
}


int getBatteryAdapter(char *result)
{
	int ret = E_OK;
	char battery[3];

	_DEBUG("Getting gsm location adapter...");

	ret = getBattery();
	if (ret < 0)
		_DEBUG("Can't get location");
	else
	{
		snprintf(result, 15, "Battery: %d\%", ret);
	}

	return ret;
}


int hdlrBattery(char *args, char *result)
{
	int ret = E_OK;

	_DEBUG("Getting battery handler...");

	if (args != NULL)
	{
		strcpy(result, "Too much arguments");
		return ERR_INCORRECT_CMD_ARGS;
	}

	ret = getBattery();
	if (ret < 0)
		_DEBUG("Can't get location");
	else
		snprintf(result, 15, "Battery: %d\%", ret);

	return 0;
}


int getNumber(char *number, uint8_t num)
{
	int ret = E_OK;

	switch (num)
	{
	case 1:
		readEEPROMBlock(number, EEPROM_PHONE1_ADDR, PHONE1_LEN);
		break;
	case 2:
		readEEPROMBlock(number, EEPROM_PHONE2_ADDR, PHONE2_LEN);
		break;
	default:
		ret = ERR_INCORRECT_NUMBER;
		break;
	}

	*(number + PHONE1_LEN) = '\0';

	_DEBUG("number: %s", number);

	return ret;
}


int setNumber(char *number, uint8_t num)
{
	int ret = E_OK;

	switch (num)
	{
	case 1:
		writeEEPROMBlock(number, EEPROM_PHONE1_ADDR, PHONE1_LEN);
		break;
	case 2:
		writeEEPROMBlock(number, EEPROM_PHONE2_ADDR, PHONE2_LEN);
		break;
	default:
		ret = ERR_INCORRECT_NUMBER;
		break;
	}

	return ret;
}


int getNumberAdapter(char *var)
{
	int ret = E_OK;
	uint8_t result[50];
	uint8_t buf1[PHONE1_LEN + 1];	// null-terminated string
	uint8_t buf2[PHONE2_LEN + 1];	// null-terminated string

	_DEBUG("getting number adapter - %s", var);

	ret = getNumber((char *)&buf1[0], 1);
	if (ret < 0)
		strcpy(var, "Can't get emergency number1.");

	ret = getNumber((char *)&buf2[0], 2);
	if (ret < 0)
		strcpy(var, "Can't get emergency number2.");

	strcpy((char *)result, "Your emergency number is ");
	strcat((char *)result, (char *)buf1);
	strcat((char *)result, ", ");
	strcat((char *)result, (char *)buf2);

	strcpy(var, (char *)result);

	ret = strlen((char *)var);

	return ret;
}


int setNumberAdapter(char *number, char *result)
{
	int ret = E_OK;
	uint8_t tmp[PHONE1_LEN + 1];

	_DEBUG("new number adapter - %s", number);

	// filter for '-', '(', ')' had to be worked before

	setNumber(number, 1);

	strcpy(result, "New number is ");

	getNumber(tmp, 1);
	tmp[PHONE1_LEN] = '\0';
	strcat(result, tmp);

	ret = strlen(result);

	return ret;
}


int hdlrNumber(char *args, char *result)
{
	int ret = E_OK;
	uint8_t buf1[PHONE1_LEN + 1];	// null-terminated string
	uint8_t buf2[PHONE2_LEN + 1];	// null-terminated string

	_DEBUG("Number handler...");

	if (args == NULL)
	{
		ret = getNumber((char *)&buf1[0], 1);
		if (ret < 0)
			goto err;
		ret = getNumber((char *)&buf2[0], 2);
		if (ret < 0)
			goto err;

		strcpy((char *)result, "N1:+");
		strcat((char *)result, (char *)buf1);
		strcat((char *)result, "\r\n");
		strcat((char *)result, "N2:+");
		strcat((char *)result, (char *)buf2);
	}
	else if ((*args == '1') && (strlen(args) == 12))
	{
		setNumber(args + 1, 1);
		strcpy((char *)result, "New number was set.");
	}
	else if ((*args == '2') && (strlen(args) == 12))
	{
		setNumber(args + 1, 2);
		strcpy((char *)result, "New number was set.");
	}
	else
	{
		strcpy(result, "Incorrect value.");
		return ERR_INCORRECT_CMD_ARGS;
	}

	return 0;

err:
	strcpy((char *)result, "Can't get number!");
	//TODO:: change error number
	return -1;
}


/*
 *  Accelerometer update
 */
int setAccelAdapter(char *stat, char *result)
{
	int ret = E_OK;

	_DEBUG("changing accel adapter - %s", stat);

	if ((memcmp(stat, "OFF", 3) == 0) || *stat == '-')
	{
		strcpy(result, "Accelerometer off.");
		ret = strlen(result) + 1;
	}
	else if ((memcmp(stat, "ON", 2) == 0) || *stat == '+')
	{
		strcpy(result, "Accelerometer on.");
		ret = strlen(result) + 1;
	}
	else
	{
		ret = ERR_INCORRECT_VALUE;
	}

	return ret;
}


int hdlrAccel(char *args, char *result)
{
	int ret = E_OK;

	_DEBUG("Accelerometer handler...");

	if (memcmp(args, "ON", 2) || memcmp(args, "+", 1))
	{
		//TODO:: insert command
		strcpy(result, "Accelerometer on.");
	}
	else if (memcmp(args, "OFF", 3) || memcmp(args, "-", 1))
	{
		//TODO:: insert command
		strcpy(result, "Accelerometer off.");
	}
	else
	{
		ret = ERR_INCORRECT_CMD_ARGS;
		strcpy(result, "Incorrect command for accelerometer.");
	}

	return ret;
}

/*
 *  Time interval update
 */
int getTimeUpdateAdapter(char *time)
{
	int ret = E_OK;

	// get update time

	strcpy(time, "Update time: 10 min.");

	return ret;
}


int setTimeUpdateAdapter(char *var, char *result)
{
	int ret = E_OK;

	_DEBUG("changing time adapter - %s", var);

	strcpy(result, "Time interval was updated.");
	ret = strlen(result) + 1;

	//parsing new time

	return ret;
}


int hdlrTimeUpdate(char *args, char *result)
{
	int ret = E_OK;

	_DEBUG("Time update handler...");

	if (args == NULL)
	{
		//TODO:: add function getTimeUpdate()
		strcpy(result, "Update time: 10 min.");
	}
	else
	{
		// Update time
		strcpy(result, "Time interval was updated.");
	}

	return ret;
}


int getPass(char *pass)
{
	int ret = E_OK;

	readEEPROMBlock(pass, EEPROM_SMS_PASS_ADDR, SMS_PASS_LEN);

	return ret;
}


int setPass(char *pass)
{
	int ret = E_OK;

	writeEEPROMBlock(pass, EEPROM_SMS_PASS_ADDR, SMS_PASS_LEN);

	return ret;
}


int setPassAdapter(char *pass)
{
	_DEBUG("set new pass mock - %s", pass);
	return 0;
}


int hdlrPass(char *args, char *result)
{
	int ret = E_OK;
	char pass[SMS_PASS_LEN];

	_DEBUG("Pass handler...");

	if (args == NULL)
	{
		getPass(&pass[0]);
		strcpy(result, "Password:");
		strncpy(result, &pass[0], SMS_PASS_LEN);
	}
	else if(strlen(args) == SMS_PASS_LEN)
	{

	}
	else
	{
		strcpy(result, "Incorrect pass length");
		ret = ERR_INCORRECT_CMD_ARGS;
	}

	return ret;
}


int setDefault(void)
{
	int ret = E_OK;

	writeEEPROMBlock(DEFAULT_SMS_PASS, EEPROM_SMS_PASS_ADDR, SMS_PASS_LEN);
	writeEEPROMBlock(DEFAULT_SERVER1, EEPROM_SERVER1_ADDR, SERVER1_LEN);
	writeEEPROMBlock("++++++++++++", EEPROM_PHONE1_ADDR, PHONE1_LEN);
	writeEEPROMBlock("++++++++++++", EEPROM_PHONE2_ADDR, PHONE2_LEN);

	return ret;
}


int setDefaultAdapter(char *result)
{
	int ret = E_OK;

	_DEBUG("set default mock.");

	strcpy(result, "Default settings was set up.");
	ret = strlen(result) + 1;

	return ret;
}


int hdlrDefault(char *args, char *result)
{
	int ret = E_OK;

	_DEBUG("Set default handler...");

	if (args == NULL)
	{
		//TODO:: fix mock
		strcpy(result, "Default settings was set up.");
	}
	else
	{
		strcpy(result, "Incorrect arguments for default command.");
		ret = ERR_INCORRECT_CMD_ARGS;
	}

	return ret;
}


int setEchoAdapter(char *var)
{
	_DEBUG("echo cmd mock - %s", var);
	sendSMSMessage(var, "+79266242473");

	return 0;
}


int hdlrEcho(char *args, char *result)
{
	int ret = E_OK;

	_DEBUG("Echo handler...");

	if (args == NULL)
	{
		strcpy(result, "Echo response.");
	}
	else
	{
		strcpy(result, args);
	}

	return ret;
}
