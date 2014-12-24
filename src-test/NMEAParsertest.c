/*
 * NMEAParsertest.c
 *
 *  Created on: 24.01.2014
 *      Author: anton
 */

#include "embUnit/embUnit.h"
#include "ims.h"


static void setUp() {}


static void tearDown() {}


static void GPRMCSentenceTesting()
{
	const char buff[] = "$GPRMC,220516,A,5133.82,N,00042.24,W,173.8,231.8,130694,004.2,W*70\r\n";

	nmeaINFO info;
    nmeaPARSER parser;

    nmea_zero_INFO(&info);
    nmea_parser_init(&parser);

    nmea_parse(&parser, buff, (int)strlen(buff), &info);

    nmea_parser_destroy(&parser);

    TEST_ASSERT_EQUAL_INT(13, info.utc.day);
    TEST_ASSERT_EQUAL_INT(6 - 1, info.utc.mon); 	/* timeinfo.tm_mon = mon - 1; */
    TEST_ASSERT_EQUAL_INT(94, info.utc.year);

    TEST_ASSERT_EQUAL_INT(22, info.utc.hour);
    TEST_ASSERT_EQUAL_INT(05, info.utc.min);
    TEST_ASSERT_EQUAL_INT(16, info.utc.sec);

    TEST_ASSERT_EQUAL_INT(5133.82, info.lat);
    TEST_ASSERT_EQUAL_INT(-00042.24, info.lon);
}


static void GPGGASentenceTesting()
{
	const char buff[] = "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76\r\n";

    nmeaINFO info;
    nmeaPARSER parser;

    nmea_zero_INFO(&info);
    nmea_parser_init(&parser);

    nmea_parse(&parser, buff, (int)strlen(buff), &info);

    nmea_parser_destroy(&parser);

    TEST_ASSERT_EQUAL_INT(9, info.utc.hour);
    TEST_ASSERT_EQUAL_INT(27, info.utc.min);
    TEST_ASSERT_EQUAL_INT(50, info.utc.sec);

    TEST_ASSERT_EQUAL_INT(5321.6802, info.lat);
    TEST_ASSERT_EQUAL_INT(-00630.3372, info.lon);
    TEST_ASSERT_EQUAL_INT(61.7, info.elv);

    /* don't get info about satellites from this sentence */
    TEST_ASSERT_EQUAL_INT(1.03, info.HDOP);
}


static void GPGSVSentenceTesting()
{
	const char buff[] = "$GPGSV,3,1,11,03,03,111,00,04,15,270,00,06,01,010,00,13,06,292,00*74\r\n";

    nmeaINFO info;
    nmeaPARSER parser;

    nmea_zero_INFO(&info);
    nmea_parser_init(&parser);

    nmea_parse(&parser, buff, (int)strlen(buff), &info);

    TEST_ASSERT_EQUAL_INT(11, info.satinfo.inview);
    TEST_ASSERT_EQUAL_INT(0, info.satinfo.inuse);

    /* 3 satellites in sentence */
    TEST_ASSERT_EQUAL_INT(3, info.satinfo.sat[0].id);
    TEST_ASSERT_EQUAL_INT(3, info.satinfo.sat[0].elv);
    TEST_ASSERT_EQUAL_INT(111, info.satinfo.sat[0].azimuth);
    TEST_ASSERT_EQUAL_INT(0, info.satinfo.sat[0].sig);

    TEST_ASSERT_EQUAL_INT(4, info.satinfo.sat[1].id);
    TEST_ASSERT_EQUAL_INT(15, info.satinfo.sat[1].elv);
    TEST_ASSERT_EQUAL_INT(270, info.satinfo.sat[1].azimuth);
    TEST_ASSERT_EQUAL_INT(0, info.satinfo.sat[1].sig);

    TEST_ASSERT_EQUAL_INT(6, info.satinfo.sat[2].id);
    TEST_ASSERT_EQUAL_INT(1, info.satinfo.sat[2].elv);
    TEST_ASSERT_EQUAL_INT(10, info.satinfo.sat[2].azimuth);
    TEST_ASSERT_EQUAL_INT(0, info.satinfo.sat[2].sig);

    TEST_ASSERT_EQUAL_INT(13, info.satinfo.sat[3].id);
    TEST_ASSERT_EQUAL_INT(6, info.satinfo.sat[3].elv);
    TEST_ASSERT_EQUAL_INT(292, info.satinfo.sat[3].azimuth);
    TEST_ASSERT_EQUAL_INT(0, info.satinfo.sat[3].sig);

    /* add one more sentence to check how library add more info in info struct */
    const char buff1[] = "$GPGSV,3,2,11,14,25,170,00,16,57,208,39,18,67,296,40,19,40,246,00*74\r\n";

    nmea_parse(&parser, buff1, (int)strlen(buff1), &info);

    TEST_ASSERT_EQUAL_INT(14, info.satinfo.sat[4].id);
    TEST_ASSERT_EQUAL_INT(25, info.satinfo.sat[4].elv);
    TEST_ASSERT_EQUAL_INT(170, info.satinfo.sat[4].azimuth);
    TEST_ASSERT_EQUAL_INT(0, info.satinfo.sat[4].sig);

    TEST_ASSERT_EQUAL_INT(16, info.satinfo.sat[5].id);
    TEST_ASSERT_EQUAL_INT(57, info.satinfo.sat[5].elv);
    TEST_ASSERT_EQUAL_INT(208, info.satinfo.sat[5].azimuth);
    TEST_ASSERT_EQUAL_INT(39, info.satinfo.sat[5].sig);

    TEST_ASSERT_EQUAL_INT(18, info.satinfo.sat[6].id);
    TEST_ASSERT_EQUAL_INT(67, info.satinfo.sat[6].elv);
    TEST_ASSERT_EQUAL_INT(296, info.satinfo.sat[6].azimuth);
    TEST_ASSERT_EQUAL_INT(40, info.satinfo.sat[6].sig);

    TEST_ASSERT_EQUAL_INT(19, info.satinfo.sat[7].id);
    TEST_ASSERT_EQUAL_INT(40, info.satinfo.sat[7].elv);
    TEST_ASSERT_EQUAL_INT(246, info.satinfo.sat[7].azimuth);
    TEST_ASSERT_EQUAL_INT(0, info.satinfo.sat[7].sig);

    nmea_parser_destroy(&parser);
}


/*
 *   Sample from nmealib source code
 */
static void BaseNMEASentences()
{
    const char *buff[] = {
        "$GPRMC,173843,A,3349.896,N,11808.521,W,000.0,360.0,230108,013.4,E*69\r\n",
        "$GPGGA,111609.14,5001.27,N,3613.06,E,3,08,0.0,10.2,M,0.0,M,0.0,0000*70\r\n",
        "$GPGSV,2,1,08,01,05,005,80,02,05,050,80,03,05,095,80,04,05,140,80*7f\r\n",
        "$GPGSV,2,2,08,05,05,185,80,06,05,230,80,07,05,275,80,08,05,320,80*71\r\n",
        "$GPGSA,A,3,01,02,03,04,05,06,07,08,00,00,00,00,0.0,0.0,0.0*3a\r\n",
        "$GPRMC,111609.14,A,5001.27,N,3613.06,E,11.2,0.0,261206,0.0,E*50\r\n",
        "$GPVTG,217.5,T,208.8,M,000.00,N,000.01,K*4C\r\n"
    };

    nmeaINFO info;
    nmeaPARSER parser;
    int it;

    nmea_zero_INFO(&info);
    nmea_parser_init(&parser);

    for(it = 0; it < 6; ++it)
    	nmea_parse(&parser, buff[it], (int)strlen(buff[it]), &info);

    nmea_parser_destroy(&parser);

    TEST_ASSERT_EQUAL_INT(11, info.utc.hour);
    TEST_ASSERT_EQUAL_INT(16, info.utc.min);
    TEST_ASSERT_EQUAL_INT(9, info.utc.sec);

    TEST_ASSERT_EQUAL_INT(0, info.direction);
//    TEST_ASSERT_EQUAL_INT(0, info.speed);
//    TEST_ASSERT_EQUAL_INT(9, info.declination);
}


/*
 *   Sample getting from ML8088S GPS module
 */
static void ML8088SNMEASentences()
{
    const char *buff[] = {
        "$GPRMC,065803.000,V,5558.66287,N,03709.85870,E,0.0,0.0,160114,,,N*72\r\n",
        "$GPGGA,065803.000,5558.66287,N,03709.85870,E,0,00,99.0,0263.0,M,14.9,M,,*51\r\n",
        "$GPVTG,0.0,T,,M,0.0,N,0.0,K,N*02\r\n",
        "$GNGSA,A,1,,,,,,,,,,,,,99.0,99.0,99.0*1E\r\n",
        "$GNGSA,A,1,,,,,,,,,,,,,99.0,99.0,99.0*1E\r\n",
        "$GPGSV,4,1,14,03,55,242,,06,50,201,,08,00,325,,11,16,292,*7C\r\n",
        "$GPGSV,4,2,14,14,23,155,,15,16,038,,16,06,223,,18,49,072,*74\r\n",
        "$GPGSV,4,3,14,19,51,289,,21,21,106,,22,77,141,,24,06,072,*72\r\n",
        "$GPGSV,4,4,14,27,59,215,,28,07,348,,,,,,,,,*71\r\n",
        "$GLGSV,3,1,10,67,18,358,,68,27,134,,84,29,146,,78,15,025,*65\r\n",
        "$GLGSV,3,2,10,70,75,190,,86,45,315,,77,15,335,,69,43,046,*6B\r\n",
        "$GLGSV,3,3,10,85,77,176,,71,23,215,,,,,,,,,*68\r\n",
        "$GPGLL,5558.66287,N,03709.85870,E,065803.000,V*24\r\n"
    };

    nmeaINFO info;
    nmeaPARSER parser;
    int it;

    nmea_zero_INFO(&info);
    nmea_parser_init(&parser);

    for (it = 0; it < 13; ++it)
    	nmea_parse(&parser, buff[it], (int)strlen(buff[it]), &info);

    nmea_parser_destroy(&parser);

    /* time */
    TEST_ASSERT_EQUAL_INT(6, info.utc.hour);
    TEST_ASSERT_EQUAL_INT(58, info.utc.min);
    TEST_ASSERT_EQUAL_INT(3, info.utc.sec);

    /* date */
    TEST_ASSERT_EQUAL_INT(16, info.utc.day);
    TEST_ASSERT_EQUAL_INT(1 - 1, info.utc.mon);
    TEST_ASSERT_EQUAL_INT(114, info.utc.year);

    /* lat lon */
    TEST_ASSERT_EQUAL_INT(5558.66287, info.lat);
    TEST_ASSERT_EQUAL_INT(3709.85870, info.lon);
    TEST_ASSERT_EQUAL_INT(263, info.elv);

    TEST_ASSERT_EQUAL_INT(0, info.declination);
    TEST_ASSERT_EQUAL_INT(99, info.HDOP);

    /* satellites */
    TEST_ASSERT_EQUAL_INT(0, info.satinfo.inuse);
    TEST_ASSERT_EQUAL_INT(14, info.satinfo.inview);

    TEST_ASSERT_EQUAL_INT(3, info.satinfo.sat[0].id);
    TEST_ASSERT_EQUAL_INT(55, info.satinfo.sat[0].elv);
    TEST_ASSERT_EQUAL_INT(242, info.satinfo.sat[0].azimuth);
    TEST_ASSERT_EQUAL_INT(0, info.satinfo.sat[0].sig);

    TEST_ASSERT_EQUAL_INT(6, info.satinfo.sat[1].id);
    TEST_ASSERT_EQUAL_INT(50, info.satinfo.sat[1].elv);
    TEST_ASSERT_EQUAL_INT(201, info.satinfo.sat[1].azimuth);
    TEST_ASSERT_EQUAL_INT(0, info.satinfo.sat[1].sig);

    TEST_ASSERT_EQUAL_INT(28, info.satinfo.sat[13].id);
    TEST_ASSERT_EQUAL_INT(7, info.satinfo.sat[13].elv);
    TEST_ASSERT_EQUAL_INT(348, info.satinfo.sat[13].azimuth);
    TEST_ASSERT_EQUAL_INT(0, info.satinfo.sat[13].sig);
}


TestRef NMEAParserTest_test(void)
{
	EMB_UNIT_TESTFIXTURES(fixtures) {
		new_TestFixture("GPRMCSentenceTesting", GPRMCSentenceTesting),
		new_TestFixture("GPGGASentenceTesting", GPGGASentenceTesting),
		new_TestFixture("GPGSVSentenceTesting", GPGSVSentenceTesting),
		new_TestFixture("BaseNMEASentences", BaseNMEASentences),
		new_TestFixture("ML8088SNMEASentences", ML8088SNMEASentences),
		};

	EMB_UNIT_TESTCALLER(NMEAParser_test,"NMEAParser_test",setUp,tearDown,fixtures);

	return (TestRef)&NMEAParser_test;
}
