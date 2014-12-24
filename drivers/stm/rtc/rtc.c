/**
 *	\file
 *
 *	\brief RTC management
 *
 *	\page License
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
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
 *  Created on: 09.06.2011
 *      Author: Anton Glukhov
 */

#include <time.h>
#include "include/ims.h"
#include "rtc.h"


void RTC_Configuration(void)
{
	_DEBUG("RTC Configuration...");

	RTC_InitTypeDef RTC_InitStructure;
	RTC_TimeTypeDef  RTC_TimeStruct;

	/* Enable the PWR clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Allow access to RTC */
	PWR_RTCAccessCmd(ENABLE);

	/* Reset RTC Domain */
	RCC_RTCResetCmd(ENABLE);
	RCC_RTCResetCmd(DISABLE);

	/* Enable the LSE OSC */
	RCC_LSEConfig(RCC_LSE_ON);

	/* Wait till LSE is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

	/* Select the RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	/* Enable the RTC Clock */
	RCC_RTCCLKCmd(ENABLE);

	/* Wait for RTC APB registers synchronisation */
	RTC_WaitForSynchro();

	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);

	/* Configure the RTC data register and RTC prescaler - 1Hz */
	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
	RTC_InitStructure.RTC_SynchPrediv  = 0xFF;
	RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;
	int error = RTC_Init(&RTC_InitStructure);
	if (error == 0)
		_DEBUG("ERROR: init RTC");

	/* Set the time to 00h 00mn 00s AM */
	RTC_TimeStruct.RTC_H12     = RTC_H12_AM;
	RTC_TimeStruct.RTC_Hours   = 0x00;
	RTC_TimeStruct.RTC_Minutes = 0x00;
	RTC_TimeStruct.RTC_Seconds = 0x00;
	int err = RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);
	if (err == 0)
		_DEBUG("ERROR: setup RTC");

	RTC_TimeTypeDef RTC_TimeStruct1;

	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct1);
	_DEBUG("Time - %.2d:%.2d:%.2d", RTC_TimeStruct1.RTC_Hours,
			  	  	  	  	  	 RTC_TimeStruct1.RTC_Minutes,
			  	  	  	  	  	 RTC_TimeStruct1.RTC_Seconds);

	_DEBUG("Done.");
}


void RestoreRCC(void)
{
#ifdef DISCOVERY
	//TODO:: Add customize reinitialization without RTC reset
	RCC_Configuration();
#else
	RCC_HSEConfig(ENABLE);

	while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);

	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);
#endif
}


void RTC_Alarm_Configuration(void)
{
	_DEBUG("RTC alarm Configuration...");

	RTC_AlarmTypeDef RTC_AlarmStructure;

	/* Set the alarmA Masks */
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay | RTC_AlarmMask_Hours | RTC_AlarmMask_Minutes;
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = 0x00;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0x00;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0x00;

	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);

	_DEBUG("Done.");
}


void setDate(date_tt Date, time_tt Time)
{
	_DEBUG("Setup date and time...");

	RTC_DateTypeDef		Local_DateStruct;
	RTC_TimeTypeDef  	Local_TimeStruct;

#ifdef DEBUG
	RTC_DateTypeDef		RTC_DateStruct;
	RTC_TimeTypeDef 	RTC_TimeStruct;
#endif

	Local_DateStruct.RTC_Year		= Date.Date_Year;
	Local_DateStruct.RTC_Month		= Date.Date_Month;
	Local_DateStruct.RTC_Date		= Date.Date_Day;

	RTC_SetDate(RTC_Format_BIN, &Local_DateStruct);

	Local_TimeStruct.RTC_H12		= RTC_H12_AM;
	Local_TimeStruct.RTC_Hours		= Time.Time_Hours;
	Local_TimeStruct.RTC_Minutes	= Time.Time_Minutes;
	Local_TimeStruct.RTC_Seconds	= Time.Time_Seconds;

	RTC_SetTime(RTC_Format_BIN, &Local_TimeStruct);

#ifdef DEBUG
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
	_DEBUG("Date - %.4d:%.2d:%.2d", RTC_DateStruct.RTC_Year,
								RTC_DateStruct.RTC_Month,
								RTC_DateStruct.RTC_Date);

	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
	_DEBUG("Time - %.2d:%.2d:%.2d", RTC_TimeStruct.RTC_Hours,
										RTC_TimeStruct.RTC_Minutes,
										RTC_TimeStruct.RTC_Seconds);
#endif

	_DEBUG("Done.");
}

void setAlarmInterval(uint8_t TimeInterval)
{
	_DEBUG("Setup alarm...");
	/* not sure about this function - maybe every day or week events */
	_DEBUG("Done.");
}


void turnAlarmOn()
{
	/* Enable AlarmA interrupt */
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);

	/* Enable the alarmA */
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
}


void turnAlarmOff()
{
	/* Enable AlarmA interrupt */
	RTC_ITConfig(RTC_IT_ALRA, DISABLE);

	/* Enable the alarmA */
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
}


void setUpWakeUpInterval(time_interval_t TimeInterval)
{
	_DEBUG("Setup wakeup intervals...");

	RTC_WakeUpCmd(DISABLE);

//	RTC_SetWakeUpCounter(10);
	RTC_SetWakeUpCounter(TimeInterval * 60 - 1);

	RTC_WakeUpCmd(ENABLE);

	_DEBUG("Done.");
}

//TODO:: instead two functions On and Off -> one with ENABLE|DISABLE argument
void turnWakeUpIntervalOn(void)
{
//	RTC_ClearITPendingBit(RTC_IT_WUT);
	RTC_ITConfig(RTC_IT_WUT, ENABLE);
	RTC_WakeUpCmd(ENABLE);
}

void turnWakeUpIntervalOff(void)
{
//	RTC_ClearITPendingBit(RTC_IT_WUT);
	RTC_ITConfig(RTC_IT_WUT, DISABLE);
	RTC_WakeUpCmd(DISABLE);
}


time_t Now(void)
{
	RTC_DateTypeDef		RTC_DateStruct;
	RTC_TimeTypeDef 	RTC_TimeStruct;

	struct tm timeinfo;

	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);

	timeinfo.tm_hour = RTC_TimeStruct.RTC_Hours;
	timeinfo.tm_min = RTC_TimeStruct.RTC_Minutes;
	timeinfo.tm_sec = RTC_TimeStruct.RTC_Seconds;
	timeinfo.tm_year = 2014 - 1900;
	timeinfo.tm_mon = RTC_DateStruct.RTC_Month;
	timeinfo.tm_mday = RTC_DateStruct.RTC_Date;

	return mktime(&timeinfo);

}
