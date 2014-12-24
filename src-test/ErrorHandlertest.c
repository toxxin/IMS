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

#include "errors.h"

#define uint8_t char

static void setUp(void);
static void tearDown(void);
static void UnknownErrorTesting(void);


static void setUp(void)
{

}

static void tearDown(void)
{

}


static void NormalErrorMessage(void)
{
	char *ptr = getErrorMessageByNum(ERR_INIT_FAIL);

	TEST_ASSERT_EQUAL_STRING("Initialization fail", ptr);
}


static void UnknownErrorTesting(void)
{
	char *ptr = getErrorMessageByNum(30);

	TEST_ASSERT_EQUAL_STRING("Unknown error", ptr);
}


TestRef ErrorHandlerTest_test(void)
{
	EMB_UNIT_TESTFIXTURES(fixtures) {
		new_TestFixture("Normal error case", NormalErrorMessage),
		new_TestFixture("Unexpected error", UnknownErrorTesting),
		};

	EMB_UNIT_TESTCALLER(SELFTest_test,"SELFTest",setUp,tearDown,fixtures);

	return (TestRef)&SELFTest_test;
}

