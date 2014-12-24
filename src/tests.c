/*
 * tests.c
 *
 *  Created on: Nov 20, 2014
 *      Author: anton
 */

#include <time.h>
#include "ims.h"
#include "rtc.h"

void signal_test(void)
{
	_gsm_quality_t signal;

	gsmOn();

	SETBIT(gsm_module_config, CFG_ECHO_MODE);

	while(1)
	{
		if (getGSMSignal(&signal))
		{
			_DEBUG("rssi - %d", signal.rssi);
			_DEBUG("ber - %d", signal.ber);
		}
		Delay_ms(1000);
	}
}


void sms_commands_test(void)
{
	char tbuf[100];
	uint8_t ids[SMS_SIM_NUMBER];
	uint8_t i;
	int ret, num;

	while(1)
	{

		/* default configuration */
//		setDefault();

		TaskQueue tqueue = createTaskQueue();
		TaskResultQueue trqueue = createTaskResultQueue();
		InceptionSMSStack smsstack = createInceptionSMSStack();
		TMapList tmap = createTMapList();

		_sms_t *sms;

//		sms = initSMS();
//
//		memcpy(sms->data, "A+,G", strlen("1234a+,g"));
//		sms->index = 1;
//		sms->date = 1234567;
//		sms->sz = strlen("1234a+,g");
//		memcpy(sms->oa, "+79266242473", 12);
//		memcpy(sms->da, "+79261112233", 12);
//
//		while (1)
//		{
//			uint8_t inception_id = smsstack.push(&smsstack, sms->oa, sms->date);
//			parseSmsCmd(&tqueue, sms, 0, &tmap);
//
//			smsstack.clean(&smsstack);
//
//			TaskResult *result;
//
//			while (tqueue.size > 0)
//			{
//				result = tqueue.run(&tqueue);
//					trqueue.add(&trqueue, result);
//				free(result->name);
//				free(result->result);
//				free(result);
//				tqueue.del(&tqueue);
//			}
//
//			while(tqueue.size > 0)
//				tqueue.del(&tqueue);
//			tmap.clean(&tmap);
//
//			trqueue.clean(&trqueue);
//			smsstack.clean(&smsstack);
//		}


		/* test sms handler */
		while (1)
		{
			ret = getSMSIdList(&ids[0], SMS_TYPE_REC_UNREAD);
			if (ret < 0)
			{
				_DEBUG("Error while get Id list - %d", ret);
			}
			else if (ret > 0)
			{
				_DEBUG("Numbers of new sms - %d", ret);

				num = ret;
				_sms_t *sms;

				for (i = 0; i < num; ++i)
				{
					sms = initSMS();

					ret = getSMSMessage(ids[i], sms);
					if (ret < 0)
					{
						_DEBUG("Error while get sms - %d", ret);
					}
					else
					{
						printdbgSMS(sms);

						uint8_t inception_id = smsstack.push(&smsstack, sms->oa, sms->date);
						parseSmsCmd(&tqueue, sms, inception_id, &tmap);
					}

					deinitSMS(sms);
				}

				tqueue.printdbg(&tqueue);
//				tqueue.merge(&tqueue);

				TaskResult *result;
//
				while (tqueue.size > 0)
				{
					result = tqueue.run(&tqueue);
//					trqueue.add(&trqueue, result);
					free(result);
					tqueue.del(&tqueue);
				}

				tqueue.printdbg(&tqueue);
				trqueue.printdbg(&trqueue);
			}
#if 0
			while (smsstack.size > 0)
			{
				uint8_t tid[MAX_TASK_ID_POOL];

				tmap.printdbg(&tmap);

				InceptionSMS inception;
				if (smsstack.pop(&smsstack, &inception) < 0)
					break;

				ret = tmap.getTaskIdByIncId(&tmap, inception.id, &tid[0]);
				if (ret > 0)
				{
					/* 	Main logic for multiple task for one inception.
					 * 	Before doing this logic we have to find and merge
					 * 	equal inceptions.
					 */

					/* send all results in one sms message */
					for (i = 0; i < ret; ++i)
					{
//						_DEBUG("i - %d", tid[i]);
						trqueue.printdbg(&trqueue);
						TaskResultNode *task_result = trqueue.getNodeById(&trqueue, tid[i]);
						_DEBUG("test - %s", inception.oa);
						_DEBUG("res - %s", task_result->result->name);
						_DEBUG("test", NULL);
						sendSMSMessageMock(task_result->result->result, inception.oa);
					}
				}
				else
				{
					_DEBUG("Did not found inc", NULL);
				}
			}
#endif

			smsstack.clean(&smsstack);
			smsstack.printdbg(&smsstack);
			tmap.clean(&tmap);
			tmap.printdbg(&tmap);
			trqueue.clean(&trqueue);
			trqueue.printdbg(&trqueue);
//			deleteAllSMSMessage();

			Delay_ms(100);
		}

//		turnWakeUpIntervalOn();
//		PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_SLEEPEntry_WFI);
//		turnWakeUpIntervalOff();
//		RestoreRCC();
	}
}


void freertos_test(void)
{
#if 0
	if( xTaskCreate( Task1, "Task1", 200, NULL, 1, NULL) != pdTRUE ) {
		_DEBUG("Can't\r\n");
	}

	if( xTaskCreate( vUARTReport, "vUARTReport", 200, NULL, 2, NULL) != pdTRUE ) {
		_DEBUG("Can't\r\n");
	}

	_DEBUG("Scheduler starting...\r\n");
	vTaskStartScheduler();
#endif
}


void gps_test(void)
{
	gpsOn();

	Delay_ms(3000);

	uint32_t char_count = 0;
	uint8_t *buf;
	buf = (uint8_t *)malloc(sizeof(uint8_t) * MAX_STRING_LEN);
	if (buf == NULL)
		_DEBUG("can't allocate memory");

//	SETBIT(FLAG, WAIT_FLAG);
//	TIM_SetCounter(TIM2, 100);
//	TimerOn(TIM2);
//	USART_ITConfig(GPS_USART_PORT, USART_IT_RXNE, ENABLE);
//	while(!CHKBIT_REG(FLAG, WAIT_FLAG)) {}

	char_count = test_getBuf(buf, MAX_STRING_LEN, GPS_USART_PORT);
	*(buf + char_count) = '\0';
	_DEBUG("chars - %d", (uint32_t)char_count);
	_DEBUG("buf - %s", buf);

	while(1);
}


void rtc_test(void)
{
	RTC_DateTypeDef		RTC_DateStruct;
	RTC_TimeTypeDef 	RTC_TimeStruct;

	uint8_t i = 2;

	while (i > 0)
	{
		RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
			_DEBUG("Time - %.2d:%.2d:%.2d:%.2d", RTC_DateStruct.RTC_Year,
								RTC_DateStruct.RTC_Month,
								RTC_DateStruct.RTC_Date,
								RTC_DateStruct.RTC_WeekDay);

		RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
			_DEBUG("Time - %.2d:%.2d:%.2d", RTC_TimeStruct.RTC_Hours,
										RTC_TimeStruct.RTC_Minutes,
										RTC_TimeStruct.RTC_Seconds);
		i--;
		Delay_ms(1000);
	}

	date_tt Date;
	time_tt Time;

	Date.Date_Year = 2014 - 2000;
	Date.Date_Month = 2;
	Date.Date_Day = 12;

	Time.Time_Hours = 2;
	Time.Time_Minutes = 23;
	Time.Time_Seconds = 30;

	setDate(Date, Time);

	time_t t;
	struct tm timeinfo;

	while (1)
	{
		RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
			_DEBUG("Time - %d:%d:%d", RTC_DateStruct.RTC_Year,
								RTC_DateStruct.RTC_Month,
								RTC_DateStruct.RTC_Date);

		RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
			_DEBUG("Time - %.2d:%.2d:%.2d", RTC_TimeStruct.RTC_Hours,
										RTC_TimeStruct.RTC_Minutes,
										RTC_TimeStruct.RTC_Seconds);

		timeinfo.tm_hour = RTC_TimeStruct.RTC_Hours;
		timeinfo.tm_min = RTC_TimeStruct.RTC_Minutes;
		timeinfo.tm_sec = RTC_TimeStruct.RTC_Seconds;
		timeinfo.tm_year = 2014 - 1900;
		timeinfo.tm_mon = RTC_DateStruct.RTC_Month;
		timeinfo.tm_mday = RTC_DateStruct.RTC_Date;

		t = mktime(&timeinfo);
		memset(&timeinfo, 0, sizeof(timeinfo));

		_DEBUG("unix time - %d", t);
		t = 0;
		Delay_ms(1000);
	}
}


void rtc_wakeup_test(void)
{
	setUpWakeUpInterval(INTERVAL1M);
	turnWakeUpIntervalOn();

	RTC_TimeTypeDef RTC_TimeStruct;
	while(1)
	{
		_DEBUG("ss", NULL);

		RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
		_DEBUG("Time before - %.2d:%.2d", RTC_TimeStruct.RTC_Minutes,
											  RTC_TimeStruct.RTC_Seconds);
		Delay_ms(1000);

		turnWakeUpIntervalOn();
		PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_SLEEPEntry_WFI);
		turnWakeUpIntervalOff();
		RestoreRCC();


		RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
		_DEBUG("Time after - %.2d:%.2d", RTC_TimeStruct.RTC_Minutes,
											  RTC_TimeStruct.RTC_Seconds);

		_DEBUG("c");
		Delay_ms(10000);
		_DEBUG("test");
	}
}


void USSD_test(void)
{
	gsmOn();

	USART_STR_TX("AT&F\r", GSM_USART_PORT);
	Delay_ms(5000);

	checkGSMNetwork();

	/* get sms */

//	deleteAllSMSMessage();

	int ret;
	uint8_t ids[SMS_SIM_NUMBER];
//loop:
	ret = getSMSIdList(&ids[0], SMS_TYPE_REC_UNREAD);

	_DEBUG("sms = %d", ret);
	_DEBUG("id1 - %d", ids[0]);
	_DEBUG("id2 = %d", ids[1]);

	uint8_t i = 0;
	_sms_t *sms;

	for (i = 0; i < ret; ++i)
	{
		sms = initSMS();

		ret = getSMSMessage(ids[i], sms);
		if (ret < 0)
		{
			_DEBUG("Error while get sms - %d", ret);
		}
		else
		{
			printdbgSMS(sms);
		}

		deinitSMS(sms);
	}
	Delay_ms(2000);
//	goto loop;

	sendSMSMessage("test", "+79266242473");


	Delay_ms(1000);
	getUSDMode();
	Delay_ms(1000);
	setUSDMode(1);
	Delay_ms(1000);
	getUSDMode();
	Delay_ms(1000);
	sendUSSD("*100#");

	waitURC("*100#", 65000);

	Delay_ms(10000);

	gsmOff();
}


void gprs_test(void)
{
	uint8_t ret = E_OK;
	char tmpdata[100];
	Bearer bearer;
	uint16_t cnt = 0;

	_DEBUG("GPRS testing...");

Again:
	ret = getModuleData("AT+WIPCFG=1\r", &tmpdata[0]);
	if (ret < 0)
		_DEBUG("Error stack init");

	BearerDefInit(&bearer);
	Delay_ms(500);
	bearer.start(&bearer);
	Delay_ms(500);
	Delay_ms(5000);
	bearer.get(&bearer, GPRS_BEARER_OPT_LOGIN);
	bearer.get(&bearer, GPRS_BEARER_OPT_PASSWORD);
	bearer.get(&bearer, GPRS_BEARER_OPT_DNS1);
	bearer.get(&bearer, GPRS_BEARER_OPT_IP_ADDR);
	bearer.get(&bearer, GPRS_BEARER_OPT_IP_DST_ADDR);

	Socket *socket;

	if (createSocket(socket, 1, "109.173.124.250", 5683) < 0)
		_DEBUG("Can't init socket!");

	Delay_ms(5000);

	if (setGPRSConnection(socket) == E_OK)
		_DEBUG("Connected");
	else
		_DEBUG("Not connected");

	USART_STR_TX("First\x03", GSM_USART_PORT);
	cnt = getBuf(&tmpdata[0], 20, GSM_USART_PORT, 2000, GSM_LAST_CHAR_TIMEOUT_MS);
	_DEBUG("%d, data - %s", cnt, &tmpdata[0]);
	Delay_ms(1000);

	USART_STR_TX("Second\x03", GSM_USART_PORT);
	cnt = getBuf(&tmpdata[0], 20, GSM_USART_PORT, 2000, GSM_LAST_CHAR_TIMEOUT_MS);
	_DEBUG("%d, data - %s", cnt, &tmpdata[0]);
	Delay_ms(1000);

	USART_STR_TX("Third\x03", GSM_USART_PORT);
	cnt = getBuf(&tmpdata[0], 20, GSM_USART_PORT, 2000, GSM_LAST_CHAR_TIMEOUT_MS);
	_DEBUG("%d, data - %s", cnt, &tmpdata[0]);
	Delay_ms(1000);

	_DEBUG("close connection");

	/* stop sequence 1sec before and after */
	Delay_ms(2000);
	USART_STR_TX("\x2B\x2B\x2B", GSM_USART_PORT);
	Delay_ms(2000);

	_DEBUG("close socket");
	getModuleData("AT+WIPCLOSE=1,1\r", &tmpdata[0]);
	Delay_ms(2000);

	bearer.stop(&bearer);
	bearer.close(&bearer);

	/* cleaning up stack */
	getModuleData("AT+WIPCFG=0\r", &tmpdata[0]);

	_DEBUG("create again socket");
	goto Again;
}


void accel_test(void)
{
	AccAxesRaw_t tmp;

	while(1)
	{
#ifdef LIS331DLM
		GetAccAxesRaw(&tmp);
		/* format for Python visualization */
		fprintf(stderr, "%d,%d,%d\n", tmp.AXIS_X, tmp.AXIS_Y, tmp.AXIS_Z);
//		_DEBUG("%d,%d,%d", tmp.AXIS_X, tmp.AXIS_Y, tmp.AXIS_Z);
		Delay_ms(100);
#elif LIS331DLH
		GetAccAxesRaw(&tmp);
		/* format for Python visualization */
		fprintf(stderr, "%d,%d,%d\n", tmp.AXIS_X, tmp.AXIS_Y, tmp.AXIS_Z);
//		_DEBUG("%d,%d,%d", tmp.AXIS_X, tmp.AXIS_Y, tmp.AXIS_Z);
		Delay_ms(100);
#endif
	}
}

