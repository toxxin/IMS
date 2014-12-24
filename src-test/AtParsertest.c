/**
 * 	\file
 *
 *	\brief XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
 *
 *	\page License
 *
 * THIS SOFTWARE IS PROVIDED BY OPEN SOURCE COMUNITY "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 *
 *  Created on: 07.07.2011
 *      Author: Anton Glukhov
 */

#include "ims.h"

#include "embUnit/embUnit.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

TestRef AtParserTest_test(void);
uint8_t splitString(uint8_t *str, size_t size, uint8_t **array);

static void setUp(void);
static void tearDown(void);

uint8_t clearFirstCharsets(uint8_t **str, size_t size);

static void setUp(void)
{
	SETBIT(gsm_module_config, CFG_ECHO_MODE);
}

static void tearDown(void)
{
	CLEARBIT(gsm_module_config, CFG_ECHO_MODE);
}

static void FirstSimbolsDropTesting(void)
{
	uint8_t tmp;

	uint8_t pattern[] = "\r\n\r\0AT+CMGM=1\r\n\r\n+CMS ERROR: 300\r\n\r\n";

	uint8_t *ptr = &pattern[0];
	uint8_t *clear_ptr = &pattern[4];

	/* pattern string */
	tmp = clearFirstCharsets((uint8_t**)&ptr, sizeof(pattern));
	TEST_ASSERT_EQUAL_INT(E_OK, tmp);

	/* clear string - without first (\r \n \0) symbols */
	tmp = clearFirstCharsets((uint8_t**)&clear_ptr, sizeof(pattern) - 4);
	TEST_ASSERT_EQUAL_INT(E_OK, tmp);

    TEST_ASSERT_EQUAL_STRING((char *)&pattern[4], (char *)ptr);
    TEST_ASSERT_EQUAL_STRING((char *)&pattern[4], (char *)clear_ptr);
}

//static void LastSimbolsDropTesting(void)
//{
//	char pattern[] = "\r\n\rAT+CMGM=1\r\n\r\n+CMS ERROR: 300\r\n\r\n";
//	uint8_t len = strlen(&pattern[0]);
//	uint8_t exp = len - 4;
//
//	clearLastCharsets(&pattern[0], &len);
//	TEST_ASSERT_EQUAL_INT(exp, len);
//}

//static void EmptyStringTesting(void)
//{
//	uint8_t pattern0[] = "AT+CMGM=1\r\n\r\nOK\r\n"; /* normal string */
//	uint8_t pattern1[] = "\r\n\r\n";
//	uint8_t *ptr0 = &pattern0[0];
//	uint8_t *ptr1 = &pattern1[0];
//
//	_error_t tmp;
//
//	/* normal string */
//	tmp = clearFirstCharsets((uint8_t**)&ptr0, sizeof(pattern0));
//		TEST_ASSERT_EQUAL_INT(ERR_NO_ERROR, tmp);
//
//	/* empty string */
//	tmp = clearFirstCharsets((uint8_t**)&ptr1, sizeof(pattern1));
//	TEST_ASSERT_EQUAL_INT(ERR_ARRAY_IS_EMPTY, tmp);
//
//	tmp = clearFirstCharsets((uint8_t**)&ptr1, 1);
//	TEST_ASSERT_EQUAL_INT(ERR_ARRAY_IS_EMPTY, tmp);
//}

//static void CountStringTesting(void)
//{
//	uint8_t pattern0[] = "AT+CMGM=1\r\n\r\nOK\r\n";
//	uint8_t pattern1[] = "AT+CMGM=1\r\n\r\n+CMS ERROR: 3001\r\n1";
//	uint8_t pattern2[] = "AT+CMGM=1";
//
//	TEST_ASSERT_EQUAL_INT(2, countString(&pattern0[0], sizeof(pattern0) - 1));	/* without null char */
//	TEST_ASSERT_EQUAL_INT(3, countString(&pattern1[0], sizeof(pattern1)));
//	TEST_ASSERT_EQUAL_INT(3, countString(&pattern1[0], sizeof(pattern1) - 1));	/* without null char */
//	TEST_ASSERT_EQUAL_INT(1, countString(&pattern2[0], sizeof(pattern2) - 1));	/* one string without any splits and null char */
//}

static void  DevideStringTesting(void)
{
	uint8_t pattern0[] = "AT+CMGM=1\r\n\r\nOK\r\n";
	uint8_t pattern1[] = "AT+CMGM=1\r\n\r\n+CMS ERROR: 300\r\n\r\n";
	uint8_t pattern2[] = "AT+CMGM=1\r\n";
	uint8_t pattern3[] = "AT+CMGM=1";

	uint8_t **array0 = (uint8_t **)calloc(sizeof(uint8_t*) * 100, 0);	//TODO::add MAX_LEN definition

	splitString(&pattern0[0], sizeof(pattern0) - 1, array0);

	TEST_ASSERT_EQUAL_STRING("AT+CMGM=1", (char *)array0[0]);
	TEST_ASSERT_EQUAL_STRING("OK", (char *)array0[1]);

	uint8_t **array1 = (uint8_t **)calloc(sizeof(uint8_t*) * 100, 0); //TODO::add MAX_LEN definition

	splitString(&pattern1[0], sizeof(pattern1) - 1, array1);

	TEST_ASSERT_EQUAL_STRING("AT+CMGM=1", (char *)array1[0]);
	TEST_ASSERT_EQUAL_STRING("+CMS ERROR: 300", (char *)array1[1]);

	uint8_t **array2 = (uint8_t **)calloc(sizeof(uint8_t*) * 100, 0); //TODO::add MAX_LEN definition

	splitString(&pattern2[0], sizeof(pattern2) - 1, array2);

	TEST_ASSERT_EQUAL_STRING("AT+CMGM=1", (char *)array2[0]);
//	TEST_ASSERT_EQUAL_STRING(NULL, array2[1]); /* can't compare null and null ;) */

	uint8_t **array3 = (uint8_t **)calloc(sizeof(uint8_t*) * 100, 0); //TODO::add MAX_LEN definition

	splitString(&pattern3[0], sizeof(pattern3) - 1, array3);

	TEST_ASSERT_EQUAL_STRING("AT+CMGM=1", (char *)array3[0]);
//	TEST_ASSERT_EQUAL_STRING(NULL, array3[1]); /* can't compare null and null ;) */
}


static void AtCommandParseTesting(void)
{
	uint8_t pattern0[] = "\r\nAT+CMGM=1\r\n\r\nOK\r\n";
	uint8_t resp;

	resp = parseAtResponseOk(&pattern0[0], sizeof(pattern0) - 1);
	TEST_ASSERT_EQUAL_INT(E_OK, resp);

	resp = parseAtResponseOk(&pattern0[2], sizeof(pattern0) - 3);
	TEST_ASSERT_EQUAL_INT(E_OK, resp);
}


static void MultipleMemmemTesting(void)
{
	uint8_t pattern0[] = "\r\n+CMGL: 1,0,\"\",24\r\n07234536546OD\r\n+CMGL: 22,0,\"\",12,45235\r";
	int8_t *array[20];
	uint8_t num = 0;

	num = multipleMemmem(&pattern0[0], strlen(pattern0), "+CMGL:", &array[0]);

	TEST_ASSERT_EQUAL_INT(2, num);
	TEST_ASSERT_EQUAL_ARRAY("+CMGL: 1,0,", array[0], 11);
	TEST_ASSERT_EQUAL_ARRAY("+CMGL: 22,0", array[1], 11);
}


static void GetSmsListTesting()
{
	uint8_t pattern0[] = "\r\n+CMGL: 1,0,\"\",24\r\n07234536546OD\r\n+CMGL: 22,0,\"\",12,45235\r";
	uint8_t *array[20];
	uint8_t indexes[20];
	uint8_t i, num = 0;

	num = multipleMemmem(&pattern0[0], strlen(pattern0), "+CMGL: ", &array[0]);

	for (i = 0; i < num; ++i)
	{
		uint8_t *ptr = array[i] + 7;
		uint8_t index;
		if (*ptr + 1 == ',')
			*(ptr + 1) = '\0';
		else
			*(ptr + 2) = '\0';
		indexes[i] = atoi(ptr);

	}
	TEST_ASSERT_EQUAL_INT(1, indexes[0]);
	TEST_ASSERT_EQUAL_INT(22, indexes[1]);
}


TestRef AtParserTest_test(void)
{
	EMB_UNIT_TESTFIXTURES(fixtures) {
//		new_TestFixture("EmptyString", EmptyStringTesting),
		new_TestFixture("FirstSimbolsDrop", FirstSimbolsDropTesting),
//		new_TestFixture("FirstSimbolsDrop", LastSimbolsDropTesting),
//		new_TestFixture("CountStrings", CountStringTesting),
		new_TestFixture("StringDevide", DevideStringTesting),
		new_TestFixture("AtCommandParser", AtCommandParseTesting),
		new_TestFixture("MultipleMemmem", MultipleMemmemTesting),
		new_TestFixture("GetSmsListTesting", GetSmsListTesting),
		};

	EMB_UNIT_TESTCALLER(AtParserTest,"AtParserTest",setUp,tearDown,fixtures);

	return (TestRef)&AtParserTest;
}

