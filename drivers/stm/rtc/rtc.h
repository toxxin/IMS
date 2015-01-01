/**
 *	\file
 *
 *	\brief RTC management
 *
 *	\page License
 *
 *  Created on: 09.06.2011
 *      Author: Anton Glukhov
 */

#ifndef RTC_H_
#define RTC_H_

/** @name Time structure */
typedef struct
{
	uint8_t Time_Hours;
	uint8_t Time_Minutes;
	uint8_t Time_Seconds;
} time_tt;
/** @} */


/** @name Date structure */
typedef struct
{
	uint8_t Date_Year;	/* 1970 - start point */
	uint8_t Date_Month;
	uint8_t Date_Day;
} date_tt;
/** @} */


/** @name Time intervals */
typedef enum
{
	INTERVAL0M = 0,
	INTERVAL1M = 1,
	INTERVAL5M = 5,
	INTERVAL10M = 10,
	INTERVAL15M = 15,
	INTERVAL20M = 20,
	INTERVAL25M = 25,
	INTERVAL30M = 30,
	INTERVAL35M = 35,
	INTERVAL40M = 40,
	INTERVAL45M = 45,
	INTERVAL50M  =50,
	INTERVAL55M = 55,
	INTERVAL60M = 60,

	INTERVAL1H = 60,		/* 1 hour 		*/
	INTERVAL1H5 = 90,		/* 1,5 hours 	*/
	INTERVAL2H = 120,		/* 2 hours 		*/
	INTERVAL3H = 180,		/* 3 hours 		*/
	INTERVAL4H = 240,		/* 4 hours 		*/
	INTERVAL5H = 300,		/* 5 hours 		*/
	INTERVAL6H = 360,		/* 6 hours 		*/
	INTERVAL12H = 720,		/* 12 hours 	*/
	INTERVAL18H = 1080,		/* 18 hours 	*/
	INTERVAL24H = 1440		/* 12 hours 	*/

}time_interval_t;
/** @} */


/**
 * 	\brief	Real time clock configuration
 *
 * 	This function initialize RTC module.
 *
 *	\return void
 */
void RTC_Configuration(void);


/**
 * 	\brief	Restore system clock configuration
 *
 * 	This function reinitialize system clock.
 *
 *	\return void
 */
void RestoreRCC(void);


/**
 * 	\brief	Alarm configuration
 *
 * 	This function initialize alarm.
 *
 *	\return void
 */
void RTC_Alarm_Configuration(void);


/**
 * 	\brief	Turn alarm on
 *
 * 	This function turn alarm on.
 *
 *	\return void
 */
void turnAlarmOn(void);


/**
 * 	\brief	Turn alarm off
 *
 * 	This function turn alarm off.
 *
 *	\return void
 */
void turnAlarmOff(void);


void setUpWakeUpInterval(time_interval_t);


/**
 * 	\brief	Turn Wake up off
 *
 * 	This function turn RTC wake up module off.
 *
 *	\return void
 */
void turnWakeUpIntervalOn(void);


/**
 * 	\brief	Turn Wake up off
 *
 * 	This function turn RTC wake up module off.
 *
 *	\return void
 */
void turnWakeUpIntervalOff(void);


#endif /* RTC_H_ */
