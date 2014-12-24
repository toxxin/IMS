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

#include "embUnit/embUnit.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "include/ims.h"
#include "include/gsm.h"

#include "eeprom.h"

#if (ARCH == AVR)
//#include <util/delay.h>
//#include <avr/eeprom.h>
//#include <stddef.h>	/* size_t */
#endif


TestRef SELFTest_test(void);
static void setUp(void);
static void tearDown(void);

static void EEPROMTesting(void);

static void setUp(void)
{

}


static void tearDown(void)
{

}


static void SelfTesing(void)
{
	char test1[] = { 0x31, 0x31, 0x31, 0x31, 0x02, 0x00 };
	char test2[] = { 0x31, 0x31, 0x31, 0x31, 0x40, 0x00 };

	TEST_ASSERT_EQUAL_ARRAY(&test1[0], &test2[0], 4);
	TEST_ASSERT_EQUAL_ARRAY("123", "123", 2);
	TEST_ASSERT_EQUAL_STRING("123qwerty!!", "123qwerty!!");
	TEST_ASSERT_EQUAL_INT(1,1);
	TEST_ASSERT_EQUAL_INT(321,321);
	TEST_ASSERT_MESSAGE((0 < 1), "test fail");
}


static void EEPROMTesting(void)
{
	//TODO:: defining for AVR
//	uint8_t *addr = (uint8_t *)0x0001;
	uint32_t addr = 0x08080000;
	uint8_t test_value = 0x31;
	uint8_t read_value = 0x00;

	uint8_t test_block[10] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x34, 0x33};
	uint8_t read_block[72];

/* test byte */
	writeEEPROMByte(addr, test_value);
	read_value = readEEPROMByte(addr);

	TEST_ASSERT_EQUAL_INT(test_value, read_value);

	writeEEPROMByte(addr, 0x33);
	read_value = readEEPROMByte(addr);

	TEST_ASSERT_EQUAL_INT(0x33, read_value);


	writeEEPROMBlock(&test_block[0], addr, 7);
	readEEPROMBlock(&read_block[0], addr, 7);

	TEST_ASSERT_EQUAL_ARRAY(&test_block[0], &read_block[0], 7);

#if 0
/* test block */
	_dev_info_t myTestDeviceStruct;

	myTestDeviceStruct.device_id = 0x00000001;
	myTestDeviceStruct.hw_version = 0x0002;
	myTestDeviceStruct.sw_version = 0x0003;
	myTestDeviceStruct.state = 0x00;
	strcpy(myTestDeviceStruct.server1, "http://tracker.ics-technology.com\0");

	/* test random byte */
	writeEEPROMByte((uint8_t *)EEPROM_STATE_ADDR, myTestDeviceStruct.state);
	read_value = readEEPROMByte((uint8_t *)EEPROM_STATE_ADDR);
	TEST_ASSERT_EQUAL_INT(myTestDeviceStruct.state, read_value);

	/* test random block */
	writeEEPROMBlock((char *)(myTestDeviceStruct.phone_number1),
							(char *)(EEPROM_PHONE1_ADDR), PHONE1_LEN);
	readEEPROMBlock(&read_block[0], (char *)(EEPROM_PHONE1_ADDR),
														PHONE1_LEN);
	TEST_ASSERT_EQUAL_ARRAY((char *)(myTestDeviceStruct.phone_number1),
											&read_block[0], PHONE1_LEN);

	writeEEPROMBlock((char *)(myTestDeviceStruct.server1),
			(char *)(EEPROM_SERVER1_ADDR), strlen(myTestDeviceStruct.server1));
	readEEPROMBlock(&read_block[0], (char *)(EEPROM_SERVER1_ADDR),
											strlen(myTestDeviceStruct.server1));
	TEST_ASSERT_EQUAL_ARRAY((char *)myTestDeviceStruct.server1, &read_block[0],
											strlen(myTestDeviceStruct.server1));
#endif
}


TestRef SELFTest_test(void)
{
	EMB_UNIT_TESTFIXTURES(fixtures) {
		new_TestFixture("SelfTesting", SelfTesing),
		new_TestFixture("EEPROMTesting", EEPROMTesting),
		};

	EMB_UNIT_TESTCALLER(SELFTest_test,"SELFTest",setUp,tearDown,fixtures);

	return (TestRef)&SELFTest_test;
}
