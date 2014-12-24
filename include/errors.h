/*
 * errors.h
 *
 *  Created on: Aug 4, 2013
 *      Author: anton
 */

#ifndef ERRORS_H_
#define ERRORS_H_

#include "stm32l1xx.h"

/**	@name Error types */
typedef enum
{
	SYS_FATAL	= 0,
	SYS_ERROR	= 1,
	SYS_WARN	= 2,
	SYS_INFO	= 3,
	SYS_DEBUG	= 4,
} _error_tt;
/** @} */

/**	@name Error codes */
typedef enum
{
	E_OK 				= 0,
		ERR_UNKNOWN				= -1,
		ERR_INIT_FAIL 			= -2,
		ERR_LENGTH 				= -3,
		ERR_UNK_SIGN 			= -4,
		ERR_UNK_PACKAGE 		= -5,
		ERR_ASCII_TO_GSM_CONVERT = -6,
		ERR_MEM_AVAILABLE 		= -7,
		ERR_TIME_OUT 			= -8,
		ERR_INCORRECT_VALUE 	= -9,
		ERR_EMPTY_STRING 		= -10,
		ERR_BUF_FULL 			= -11,
		ERR_ARRAY_IS_EMPTY 		= -12,
		ERR_NO_SIM 				= -13,
		ERR_CHANGE_SMS_MODE 	= -14,
		ERR_SET_STORE 			= -15,
		ERR_READ_MESSAGE 		= -16,
		ERR_GPS_MODULE_OFF 		= -17,
		ERR_GSM_MODULE_OFF 		= -18,
		ERR_GSM_SIGNAL_OFF 		= -19,
		ERR_SERVER_TIMEOUT 		= -20,
		ERR_NO_NUMBER 			= -21,		/* in EEPROM no phone number */
		ERR_INCORRECT_PASS 		= -22,		/* sms message */
		ERR_INCORRECT_NUMBER 	= -23,		/* sms message */
		ERR_FAIL_MESSAGE 		= -24,		/* sms message */
		ERR_EEPROM_CRC_FAIL 	= -25,
		ERR_NO_ECHO				= -26,
		ERR_GSM_ERROR			= -27,
		ERR_MEMS_ERROR			= -28,
		ERR_NO_FREE_TASK_SLOT	= -29,
		ERR_NO_FREE_INCEPTION_SLOT = -30,
		ERR_NO_FREE_TMAP_SLOT	= -31,
		ERR_SMS_INCEPTION_STACK_EMPTY = -32,
		ERR_UNKNOWN_SMS_TYPE	= -33,
		ERR_INCORRECT_CMD_ARGS	= -34,


		/* Last error */
		ERR_LAST_ERROR = -36,
} _error_t;
/** @} */

typedef struct {
    _error_t  code;
    char *message;
} _errordesc;


/**	@name Error codes */
typedef enum
{
	EB_NO_ERR	= 0,
	EB_PWR,
	EB_EEPROM,
	EB_BAD_FIRMWARE,
	EB_CURRENT_MEASURE,
	EB_GSM_MOD,
	EB_GPS_MOD,
	EB_ACCEL,
	EB_NO_SIM,
	EB_UNKNOWN,
	EB_RES1,
	EB_RES2,
	EB_RES3,
	EB_RES4,
	EB_RES5,
	EB_RES6,
} _eblink_t;
/** @} */


#define BLINK_NUMBERS	4
#define BLINKCODE_MASK	0x00FF

/*
 *  0 - short blink
 *  1 - long blink
 */
typedef struct {
	_eblink_t code;
	uint8_t bcode;
} _eblinkdesc;


typedef enum
{
	EGSM_GSM_NOT_ALLOW			= 3,
	EGSM_INVALID_OPTION			= 800,
	EGSM_INVALID_OPTION_VAL		= 801,
	EGSM_NOT_ENOUGH_MEM			= 802,
	EGSM_NOT_ALLOW_FOR_TCPIP	= 803,
	EGSM_DEV_ALREADY_OPEN		= 804,
	EGSM_NETINTERF_NOT_AVAIL	= 805,
	EGSM_OPERATION_NOT_ALLOWED	= 806,
	EGSM_BCF_LINE_BUSY			= 807,
	EGSM_BCF_NO_ANSWER			= 808,
	EGSM_BCF_NO_CARRIER			= 809,
	EGSM_BCF_NO_SIM				= 810,
	EGSM_BCF_SIM_NOT_READY		= 811,
	EGSM_BCF_GPRS_FAILURE		= 812,
	EGSM_BCF_PPP_LCP_NEG		= 813,
	EGSM_BCF_PPP_AUTH			= 814,
//	ERR_MEM_AVAILABLE			= 900,	/* error from general errors */
} _egsm_t;

typedef struct {
	_egsm_t  code;
    char *message;
} _errorgsmdesc;

extern _errordesc errordesc[];
extern _eblinkdesc eblinkdesc[];
extern _errorgsmdesc errgsmdesc[];


//char *getErrorMessageByNum(error_t num);
//uint8_t getCodeblinkByErrorCode(eblink_t code);

#endif /* ERRORS_H_ */
