/*
 * SmsCmdParsertest.h
 *
 *  Created on: 21.09.2013
 *      Author: Anton
 */
#include "embUnit/embUnit.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ims.h"


typedef struct
{
	uint8_t expected[30];
	uint8_t actual[30];
} _test_vars_t;


static void setUp()
{
	writeEEPROMBlock("1234", EEPROM_SMS_PASS_ADDR, SMS_PASS_LEN);
}


static void tearDown() {}


static void DropSpacesTesting()
{
	uint8_t i = 0;

	_test_vars_t test_vars[] = {
		{ "12345N1+79992223344", "12345N1+79992223344" },
		{ "12345N1+79992223344", "1234 5N1+7999 222 33 44" },
		{ "12345N1+79992223344", "12345N1+79992223344 " },
		{ "12345N1+79992223344", " 12345N1+79992223344" },
		{ "12345N1+79992223344", " 12345N1+79992223344 " },
		{ "12345N1+79992223344", "  12345N1+79992223344" },
		{ "12345N1+79992223344", "12345N1+79992223344  " },
		{ "12345N1+79992223344", "  12345N1+   79992223344" },
		{ "12345AT+,G", "123 45 AT +,  G" },
		{ "12345AT+,G\r\n", "123 45 AT +,  G   \r\n " },
	};

	const size_t test_vars_size = sizeof(test_vars)/sizeof(test_vars[0]);

	for (i = 0; i < test_vars_size; ++i)
	{
		uint8_t *tmp = test_vars[i].actual;
		size_t len = strlen(test_vars[i].actual);
		size_t new_len = len - dropChar(tmp, len, ' ');
		TEST_ASSERT_EQUAL_ARRAY((char *)&(test_vars[i].expected), tmp, new_len);
		TEST_ASSERT_EQUAL_INT(strlen(test_vars[i].expected), new_len);
	}
}


static void CleanBufTesting()
{
	uint8_t i = 0;

	_test_vars_t test_vars[] = {
		{ "12345N1+79992223344", "12345N1+79992223344  " },
		{ "12345N1+79992223344", "  12345N1+   79992223344" },
		{ "12345AT+,G", "123 45 AT +,  G" },
		{ "12345AT+,G", "123 45 AT +,  G   \r\n " },
		{ "12345AT+,G\0\0", "123 45 AT +,  G   \r\0\0" },
	};

	const size_t test_vars_size = sizeof(test_vars)/sizeof(test_vars[0]);

	for (i = 0; i < test_vars_size; ++i)
	{
		uint8_t *tmp = test_vars[i].actual;
		uint8_t len = strlen(test_vars[i].actual);
		//TODO:: check null-terminated char - '\0'
		uint8_t new_len = len - cleanBuf(tmp, len, CHAR_SPACE | CHAR_NEW_LINE | CHAR_NEW_CARRIAGE);
		TEST_ASSERT_EQUAL_ARRAY((char *)&(test_vars[i].expected), tmp, new_len);
	}
}


static void SplitCommandTesting()
{
	int8_t ret;
	uint8_t *array[20];

	uint8_t message[] = "N+79266242473,A-";
	uint8_t sz = sizeof(message); // +\0 cause splitCommand needs null-terminated

	ret = splitCommand(&message[0], sz, SMS_CMD_DELIMITER, &array[0]);
	TEST_ASSERT_EQUAL_INT(2, ret);
	TEST_ASSERT_EQUAL_STRING("N+79266242473", array[0]);
	TEST_ASSERT_EQUAL_STRING("A-", array[1]);

	/* double delimiter */
	uint8_t message1[] = "N+79266242473,,A-";

	ret = splitCommand(&message1[0], sizeof(message1), SMS_CMD_DELIMITER, &array[0]);
	TEST_ASSERT_EQUAL_INT(2, ret);
	TEST_ASSERT_EQUAL_STRING("N+79266242473", array[0]);
	TEST_ASSERT_EQUAL_STRING("A-", array[1]);
	
	/* triple delimiter */
	uint8_t message2[] = "N+79266242473,,,A-";

	ret = splitCommand(&message2[0], sizeof(message2), SMS_CMD_DELIMITER, &array[0]);
	TEST_ASSERT_EQUAL_INT(2, ret);
	TEST_ASSERT_EQUAL_STRING("N+79266242473", array[0]);
	TEST_ASSERT_EQUAL_STRING("A-", array[1]);

	/* last char */
	uint8_t message3[] = "N+79266242473,A-,";

	ret = splitCommand(&message3[0], sizeof(message3), SMS_CMD_DELIMITER, &array[0]);
	TEST_ASSERT_EQUAL_INT(2, ret);
	TEST_ASSERT_EQUAL_STRING("N+79266242473", array[0]);
	TEST_ASSERT_EQUAL_STRING("A-", array[1]);
	
	/* brocken case */
	uint8_t message4[] = "A-,G";

	ret = splitCommand(&message4[0], sizeof(message4), SMS_CMD_DELIMITER, &array[0]);
	TEST_ASSERT_EQUAL_INT(2, ret);
	TEST_ASSERT_EQUAL_STRING("A-", array[0]);
	TEST_ASSERT_EQUAL_STRING("G", array[1]);

	/* first delimiters */
	uint8_t message5[] = ",,A-,G";

	ret = splitCommand(&message5[0], sizeof(message5), SMS_CMD_DELIMITER, &array[0]);
	TEST_ASSERT_EQUAL_INT(2, ret);
	TEST_ASSERT_EQUAL_STRING("A-", array[0]);
	TEST_ASSERT_EQUAL_STRING("G", array[1]);

	/* last delimiters */
	uint8_t message6[] = "A-,G,,";

	ret = splitCommand(&message6[0], sizeof(message6), SMS_CMD_DELIMITER, &array[0]);
	TEST_ASSERT_EQUAL_INT(2, ret);
	TEST_ASSERT_EQUAL_STRING("A-", array[0]);
	TEST_ASSERT_EQUAL_STRING("G", array[1]);

	/* only delimiters */
	uint8_t message7[] = ",,,,";

	ret = splitCommand(&message7[0], sizeof(message7), SMS_CMD_DELIMITER, &array[0]);
	TEST_ASSERT_EQUAL_INT(0, ret);
	TEST_ASSERT_EQUAL_STRING("A-", array[0]);
	TEST_ASSERT_EQUAL_STRING("G", array[1]);
}


static void UpperCaseTesting()
{
	char plain_test[] = "testTest";
	char symbols[] = "teSt>-(),TesT";

	convertToUpper(&plain_test[0]);
	TEST_ASSERT_EQUAL_STRING("TESTTEST", &plain_test[0]);

	convertToUpper(&symbols[0]);
	TEST_ASSERT_EQUAL_STRING("TEST>-(),TEST", &symbols[0]);
}


static void GetCmdByNameTesting()
{
	TEST_ASSERT_EQUAL_INT(4, checkSMSPass("1234", 4));
	TEST_ASSERT_EQUAL_INT(4, checkSMSPass("12344A+10N1", 11));
	TEST_ASSERT_EQUAL_INT(ERR_INCORRECT_PASS, checkSMSPass("0000", 4));
}


TestRef SmsParserTest_test(void)
{
	EMB_UNIT_TESTFIXTURES(fixtures) {
		new_TestFixture("DropSpacesTesting", DropSpacesTesting),
		new_TestFixture("CleanBufTesting", CleanBufTesting),
		new_TestFixture("SplitCommandTesting", SplitCommandTesting),
		new_TestFixture("UpperCaseTesting", UpperCaseTesting),
		new_TestFixture("GetCmdByNameTesting", GetCmdByNameTesting),
		};

	EMB_UNIT_TESTCALLER(SELFTest_test,"SELFTest",setUp,tearDown,fixtures);

	return (TestRef)&SELFTest_test;
}
