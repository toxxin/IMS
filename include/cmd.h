/*
 * cmd.h
 *
 *  Created on: 21.09.2013
 *      Author: Anton
 */

#ifndef CMD_H_
#define CMD_H_

#include "errors.h"


#define CHAR_SPACE				BIT(0)
#define CHAR_NEW_LINE 			BIT(1)
#define CHAR_NEW_CARRIAGE		BIT(2)
#define CHAR_NULL_TERMINATE		BIT(3)


typedef enum e_gsm_cmd
{
	CMD_GPS_LOCATION	= 0,
	CMD_GSM_LOCATION	= 1,
	CMD_BATTERY			= 2,
	CMD_ACCEL			= 3,
	CMD_NUMBER			= 4,
	CMD_TIME			= 5,
	CMD_ECHO			= 6,
	CMD_TEMPERATURE		= 7,
	CMD_RESET			= 8,
	CMD_PASSWORD		= 9,
	CMD_DEFAULT			= 10,


} _gsm_cmd_t;


/** \struct _cmd_t
 *  \brief This structure define execution command
 *  \var _cmd_t::id
 *  Member 'id' contains unique command number.
 *  \var _cmd_t::name
 *  Member 'name' contains debug name for each command.
 *  \var _cmd_t::sms
 *  Member 'sms' contains all duplicate chars for command.
 *  \var _cmd_t::web
 *  Member 'web' contains prototype for web command.
 *  \var _cmd_t::filter
 *  Member 'filter' contains constraints ONLY for SET command.
 *  \var _cmd_t::getData
 *  Member 'getData' contains pointer to function which return some data.
 *  \var _cmd_t::setData
 *  Member 'setData' contains pointer to function which update some data.
 *  \var _cmd_t::help
 *  Member 'setData' contains pointer to function which gives user help.
 */
typedef struct _cmd_t
{
	_gsm_cmd_t id;
	uint8_t *name;
	uint8_t *sms;
	size_t cmd_len;
	uint32_t filter;		/* only for set commands */
	int(*handler)(void *, void *);
} _cmd_t;


#define createCmdList() 					\
											\
_cmd_t cmd_list[] = {						\
{											\
		CMD_GPS_LOCATION,					\
		(uint8_t*)"gps/glonass location",	\
		(uint8_t*)"G",						\
		1,									\
		NO_FILTER,							\
		hdlrLocation						\
},											\
{											\
		CMD_GSM_LOCATION,					\
		(uint8_t*)"gsm location",			\
		(uint8_t*)"C",						\
		1,									\
		NO_FILTER,							\
		hdlrGSMLocation						\
},											\
{											\
		CMD_BATTERY,						\
		(uint8_t*)"Battery capacity",		\
		(uint8_t*)"B",						\
		1,									\
		NO_FILTER,							\
		hdlrBattery							\
},											\
{											\
		CMD_ACCEL,							\
		(uint8_t*)"Accelerometer handler",	\
		(uint8_t*)"A",						\
		1,									\
		ONLY_ON_OFF | MIN_ARGS_LEN_1 | MAX_ARGS_LEN_3, \
		hdlrAccel							\
},											\
{											\
		CMD_NUMBER,							\
		(uint8_t*)"Change number",			\
		(uint8_t*)"N",						\
		1,									\
		ONLY_PHONE_ARGS | MAX_ARGS_LEN_12 | MIN_ARGS_LEN_9, \
		hdlrNumber							\
},											\
{											\
		CMD_TIME,							\
		(uint8_t*)"Change time",			\
		(uint8_t*)"T",						\
		1,									\
		NO_FILTER,							\
		hdlrTimeUpdate						\
},											\
{											\
		CMD_PASSWORD,						\
		(uint8_t*)"Change password",		\
		(uint8_t*)"P",						\
		1,									\
		ONLY_NUMBER_ARGS,					\
		hdlrPass							\
},											\
{											\
		CMD_DEFAULT,						\
		(uint8_t*)"Set default",			\
		(uint8_t*)"DEFAULT",				\
		7,									\
		NO_FILTER,							\
		hdlrDefault							\
},											\
{											\
		CMD_ECHO,							\
		(uint8_t*)"Echo cmd",				\
		(uint8_t*)"E",						\
		1,									\
		NO_FILTER,							\
		hdlrEcho							\
}											\
};

#define CMD_NUMBERS		(sizeof(cmd_list) / sizeof(cmd_list[0]))

#endif /* CMD_H_ */
