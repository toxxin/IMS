/**
 * 	\file
 *	
 *	\brief Unit tests for GSM source.
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

TestRef GSMTest_test(void);
static void setUp(void);
static void tearDown(void);
static void GSM_SwitchBuffer(void);
static void GSM_checkSIMCard(void);


static void setUp(void) {}

static void tearDown(void) {}


static void GSM_ASCIItoGSMConverter(void)
{
	uint8_t res = 0;
	char buf[5] = "123\r";

	/* Length error */
	res = ascii_to_gsm(&buf[0], 0);
	TEST_ASSERT_MESSAGE((res != ERR_NO_ERROR), "Expected error");

	/* Without converts */
	strcpy(&buf[0], "123\r");
	res = ascii_to_gsm(&buf[0], 3);
	TEST_ASSERT_EQUAL_ARRAY(&buf[0], "123\r", 4);

	/* With converts */
	strcpy(&buf[0], "000\r");
	res = ascii_to_gsm(&buf[0], 3);
	TEST_ASSERT_EQUAL_ARRAY(&buf[0], "000\r", 4);

	strcpy(&buf[0], "111\r");
	res = ascii_to_gsm(&buf[0], 3);
	TEST_ASSERT_EQUAL_ARRAY(&buf[0], "111\r", 4);

}


#define TEST_BUF_SIZE 32

static void GSM_SwitchBuffer(void)
{
	char buffer[TEST_BUF_SIZE] = "Hello world\r";

	ShiftMemory(&buffer[2], &buffer[5], 2);

	buffer[2] = '1';
	buffer[3] = '1';

	TEST_ASSERT_EQUAL_ARRAY("He11llo rld\r\0", &buffer[0], 8);

	memset(buffer, 1, TEST_BUF_SIZE);

	char buffer1[TEST_BUF_SIZE] = "Hello world\r";

	ShiftMemory(&buffer1[0], &buffer1[12], 3);

	for (uint8_t i = 0; i < 3; ++i) buffer1[i] = '1';

	TEST_ASSERT_EQUAL_ARRAY("111Hello world\r\0", &buffer1[0], 4);
}


static void GSM_InternetConnectionSetupProfile(void)
{

}


static void GSM_checkSIMCard(void)
{
//	TEST_ASSERT_EQUAL_INT(ERR_NO_ERROR, checkSIMCard());

//	TEST_ASSERT_EQUAL_INT(ERR_NO_SIM, checkSIMCard());
}


TestRef GSMTest_test(void)
{
	EMB_UNIT_TESTFIXTURES(fixtures) {
//		new_TestFixture("GSM_ASCIItoGSMConverter", GSM_ASCIItoGSMConverter),
//		new_TestFixture("GSM_SwitchBuffer", GSM_SwitchBuffer),
//		new_TestFixture("GSM_checkSIMCard", GSM_checkSIMCard),

//		new_TestFixture("GSM_InternetConnectionSetupProfile", GSM_InternetConnectionSetupProfile),
		};

	EMB_UNIT_TESTCALLER(GSMTest_test,"GSMTest",setUp,tearDown,fixtures);

	return (TestRef)&GSMTest_test;
}
