/*
 * AddSmsCmdInQueue.c
 *
 *  Created on: 21.12.2013
 *      Author: anton
 */

#include <stdlib.h>

#include "ims.h"

#include "embUnit/embUnit.h"

static void setUp(void);
static void tearDown(void);

static void NormalSmsCmdtest(void);


static void setUp(void) {}


static void tearDown(void) {}


static void NormalSmsCmdtest(void)
{
	TaskQueue tqueue = createTaskQueue();
	InceptionSMSStack smsstack = createInceptionSMSStack();
	TMapList tmap = createTMapList();
	uint8_t inception_id;

	_sms_t *sms = (_sms_t *)malloc(sizeof(_sms_t));
	sms->data = (uint8_t *)malloc(sizeof(uint8_t) * SMS_MESSAGE_SIZE);

	memcpy(sms->data, "1234a+,g", strlen("1234a+,g"));
	sms->index = 1;
	sms->date = 1234567;
	sms->sz = strlen("1234a+,g");
	memcpy(sms->oa, "+79266242473", 12);
	memcpy(sms->da, "+79261112233", 12);

	inception_id = smsstack.push(&smsstack, sms->oa, sms->date);
	parseSmsCmd(&tqueue, sms, 0, &tmap);

	/* tqueue checking */
	TEST_ASSERT_EQUAL_INT(2, tqueue.size);
	TEST_ASSERT_EQUAL_INT(1234567, tqueue.getNodeById(&tqueue, 1)->date);
	TEST_ASSERT_EQUAL_INT(CMD_ACCEL, tqueue.getNodeById(&tqueue, 0)->cmd_id);
	TEST_ASSERT_EQUAL_INT(CMD_GPS_LOCATION, tqueue.getNodeById(&tqueue, 1)->cmd_id);
	TEST_ASSERT_EQUAL_STRING("+", tqueue.getNodeById(&tqueue, 0)->args);
	TEST_ASSERT_NULL(tqueue.getNodeById(&tqueue, 1)->args);

	/* tmap checking */
	TEST_ASSERT_EQUAL_INT(2, tmap.size);
	TEST_ASSERT_EQUAL_INT(0, tmap.getTMapById(&tmap, 0)->id);
	TEST_ASSERT_EQUAL_INT(1, tmap.getTMapById(&tmap, 1)->id);

	TEST_ASSERT_EQUAL_INT(0, tmap.getTMapById(&tmap, 0)->iid);
	TEST_ASSERT_EQUAL_INT(0, tmap.getTMapById(&tmap, 0)->tid);

	TEST_ASSERT_EQUAL_INT(0, tmap.getTMapById(&tmap, 1)->iid);
	TEST_ASSERT_EQUAL_INT(1, tmap.getTMapById(&tmap, 1)->tid);


	/* add one more sms */
	memcpy(sms->data, "1234C, N1+79266242473", strlen("1234C, N1+79266242473"));
	sms->index = 2;
	sms->date = 6543210;
	sms->sz = strlen("1234C, N1+79266242473");
	memcpy("+79266242477", sms->oa, 12);
	memcpy("+79261112211", sms->da, 12);

	inception_id = smsstack.push(&smsstack, sms->oa, sms->date);
	parseSmsCmd(&tqueue, sms, 1, &tmap);

	TEST_ASSERT_EQUAL_INT(4, tqueue.size);
	TEST_ASSERT_EQUAL_INT(6543210, tqueue.getNodeById(&tqueue, 2)->date);
	TEST_ASSERT_EQUAL_INT(6543210, tqueue.getNodeById(&tqueue, 3)->date);
	TEST_ASSERT_EQUAL_INT(CMD_GSM_LOCATION, tqueue.getNodeById(&tqueue, 2)->cmd_id);
	TEST_ASSERT_EQUAL_INT(CMD_NUMBER, tqueue.getNodeById(&tqueue, 3)->cmd_id);
	TEST_ASSERT_NULL(tqueue.getNodeById(&tqueue, 2)->args);
	TEST_ASSERT_EQUAL_STRING("1+79266242473", tqueue.getNodeById(&tqueue, 3)->args);


	/* tqueue checking */


	free(sms->data);
	free(sms);
}


static void DoubleCommandInTheSameSMS(void)
{
	TaskQueue tqueue = createTaskQueue();
	TaskResultQueue trqueue = createTaskResultQueue();
	InceptionSMSStack smsstack = createInceptionSMSStack();
	TMapList tmap = createTMapList();
}


static void AddEmptySms(void)
{
	_sms_t *sms = (_sms_t *)malloc(sizeof(_sms_t));
	sms->data = (uint8_t *)malloc(sizeof(uint8_t) * SMS_MESSAGE_SIZE);

	free(sms->data);
	free(sms);
}


TestRef SmsCmdInQueueTest_test(void)
{
	EMB_UNIT_TESTFIXTURES(fixtures) {
		new_TestFixture("AddNormalSmsCmdtest", NormalSmsCmdtest),
		new_TestFixture("AddEmptySms", AddEmptySms)
	};

	EMB_UNIT_TESTCALLER(SmsCmdInQueueTest,"SmsCmdInQueueTest",setUp,tearDown,fixtures);

	return (TestRef)&SmsCmdInQueueTest;
}
