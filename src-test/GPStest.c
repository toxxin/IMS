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
 *  Created on: 18.07.2011
 *      Author: Anton Glukhov
 */

#include "embUnit/embUnit.h"

#include "include/ims.h"
#include "include/gps.h"

TestRef GPSTest_test(void);
static void setUp(void);
static void tearDown(void);

static void setUp(void)
{

}

static void tearDown(void)
{

}

static void GPS_StringParser(void)
{
	char good_string[83] =
"$GPGGA,121314.15,9876.4321,N,65535.7448,E,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh\r\n";
	char bad_string[83]  =
"$GPALM,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh\r\n";

	_str_locate_t myTestGPSStruct;

	TEST_ASSERT_EQUAL_INT(ERR_UNK_PACKAGE,
			ParseString((char *)&bad_string[0], &myTestGPSStruct));

	TEST_ASSERT_EQUAL_INT(ERR_NO_ERROR,
			ParseString((char *)&good_string[0], &myTestGPSStruct));

	TEST_ASSERT_EQUAL_INT(myTestGPSStruct.latitude_d, 9876);
	TEST_ASSERT_EQUAL_INT(myTestGPSStruct.latitude_m, 4321);
	TEST_ASSERT_EQUAL_INT(myTestGPSStruct.longitude_d, 65535);
	TEST_ASSERT_EQUAL_INT(myTestGPSStruct.longitude_m, 7448);
	TEST_ASSERT_EQUAL_ARRAY((char *)myTestGPSStruct.ns, (char *)'N', 1);
	TEST_ASSERT_EQUAL_ARRAY((char *)myTestGPSStruct.ew, (char *)'E', 1);
}

TestRef GPSTest_test(void)
{
	EMB_UNIT_TESTFIXTURES(fixtures) {
		new_TestFixture("GPS_StringParser", GPS_StringParser),
		};

	EMB_UNIT_TESTCALLER(GPSTest,"GPSTest",setUp,tearDown,fixtures);

	return (TestRef)&GPSTest;
}
