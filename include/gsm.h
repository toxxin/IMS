/**
 * 	\file
 *	
 *	\brief GSM high level management.
 *
 *	\page License
 *
 *  Created on: 29.08.2011
 *      Author: Anton Glukhov
 */

#ifndef GSM_H_
#define GSM_H_

#include "errors.h"

extern volatile uint8_t gsm_module_config;

#define CFG_ECHO_MODE			BIT(0)	/* 1 - echo mode on */
#define CFG_TEXT_MODE			BIT(1)	/* 1 - text mode */


/**	@name SMS Mode */
enum
{
	SMS_PDU_MODE 	= 0,
	SMS_TEXT_MODE 	= 1
} sms_mode;
/** @} */


/**	@name Unstructured Supplementary Service Data */
enum
{
	USSD_NO_USER_ACTION_REQ	= 0,
	USSD_USER_ACTION_REQ	= 1,
	USSD_TERMINATED			= 2,
	USSD_NOT_SUPPORTED		= 3,
	USSD_NETWORK_TIME_OUT 	= 4
} usd_mode;
/** @} */


/**	@name SMS Mode */
#define	PREFER_STOR_SIM			"SM"
#define	PREFER_STOR_DEV			"ME"
#define	PREFER_STOR_SIM_DEV		"MT"
/** @} */


/**	@name SMS Mode */
#define	SMS_STAT_TEXT_UNREAD	"REC UNREAD"
#define	SMS_STAT_TEXT_READ		"REC READ"
#define	SMS_STAT_TEXT_UNSENT	"STO UNSENT"
#define	SMS_STAT_TEXT_SENT		"STO SENT"
#define	SMS_STAT_TEXT_ALL		"ALL"

#define	SMS_STAT_PDU_UNREAD		'0'
#define	SMS_STAT_PDU_READ		'1'
#define	SMS_STAT_PDU_UNSENT		'2'
#define	SMS_STAT_PDU_SENT		'3'
#define	SMS_STAT_PDU_ALL		'4'
/**	@} */

#define SMS_MAX_CMD				10

#define CHECKING_GSM_NETWORK	70

#define USSD_MAX_LENGTH			182

#define GSM_FIRST_CHAR_TIMEOUT_MS	200
#define GSM_LAST_CHAR_TIMEOUT_MS	200


/** \struct _gsm_reg_t
 *  \brief This structure define registration module in GSM network \
 *  (+CREG: 1,"0145","291A").
 *  \var _gsm_reg_t::urcmode
 *  Member 'urcmode' define level of information about network registration.
 *  \var _gsm_reg_t::regstatus
 *  Member 'regstatus' shows status of registration GSM module.
 *  \var _gsm_reg_t::netlac
 *  Member 'netlac' shows two byte location area code in hexadecimal format.
 *  \var _gsm_reg_t::netcellid
 *  Member 'netcellid' shows two byte cell ID in hexadecimal format.
 */
typedef struct _gsm_reg_t
{
	int urcmode;
	int regstatus;
	char netlac[2];
	char netcellid[2];
} _gsm_reg_t;


/** \struct _gsm_quality_t
 *  \brief This structure define quality GSM signal
 *  \var _gsm_quality_t::rssi
 *  Member 'rssi' contains signal quality in offline mode.
 *  \var _gsm_quality_t::ber
 */
typedef struct _gsm_quality_t
{
	uint8_t rssi;
	uint8_t ber;
} _gsm_quality_t;

extern _gsm_quality_t 	GSMSignal;


/*
 * 	Internet Session
 */
#define SESSION_URC_MODE		0
#define	SESSION_POLL_MODE		1


/**	@name IC data */
typedef struct
{
	char model[12];
	char revision[32];
	char manufacturer[32];
	uint64_t serial;
	uint64_t imsi;
} _gsm_module_t;
/**	@} */


int setModuleData(uint8_t *);


int getModuleData(uint8_t *, uint8_t *);


/**
 * 	\brief	Check Sim Card
 *
 * 	This function get information about SIM Card
 *
 * 	\retval #_error_t
 */
_error_t checkSIMCard(void);


/**
 * 	\brief	Checking GSM Module
 *
 * 	This function checks working of GSM module.
 *
 * 	\retval ErrorNumber
 */
int checkGSMModule(void);


/**
 * 	\brief	Update GSM Signal in global vars
 *
 * 	This function checks power of GSM signal.
 *
 * 	0		- -113dB or less 	\n
 * 	1		- -111dB			\n
 * 	2..30	- -109 to -53dB		\n
 * 	31		- -51dB or more		\n
 * 	99		- unknowing value
 *
 * 	\retval ErrorNumber
 */
int updateGSMSignal(void);


/**
 * 	\brief	Get GSM registration status
 *
 * 	This function checks registration in GSM network.
 *
 * 	\retval ErrorNumber
 */
int getRegistrationState(_gsm_reg_t *);


/**
 * 	\brief	Setup session mode
 *
 * 	This function setup URC or polling mode.
 *
 *	\param mode			- pointer to session mode
 *
 * 	\retval ErrorNumber
 */
uint8_t setSessionMode(const char *mode);


/**
 * 	\brief	Read data through Internet session
 *
 * 	This function read data from Internet session.
 * 	It works with URC(Unsolicited Result Code).
 *
 *	\param srvProfileId	- Internet connection profile identifier
 *	\param length		- number of bytes, with can be received (max. 1500 bytes)
 *
 * 	\retval ErrorNumber
 */
uint8_t readDataSession(uint8_t srvProfileId, size_t length);


/**
 * 	\brief	Write data to Internet session
 *
 * 	This function write data to Internet session.
 *
 *	\param srvProfileId	- Internet connection profile identifier
 *	\param length		- number of bytes, with can be transmitted (max. 1500 bytes)
 *	\param eodFlag		- end of data flag
 *
 * 	\retval ErrorNumber
 */
uint8_t writeDataSession(uint8_t srvProfileId, size_t length, uint8_t eodFlag);


/**
 * 	\brief	Close Internet session
 *
 * 	This function close Internet session.
 *
 *	\param srvProfileId	- Internet connection profile identifier
 *
 * 	\retval ErrorNumber
 */
uint8_t closeSession(uint8_t srvProfileId);


void gsmOn(void);
void gsmOff(void);
void checkGSMNetwork(void);
int getManufacturerId(uint8_t *);
int getModelId(uint8_t *);
int getSerialNumber(uint8_t *);

#endif /* GSM_H_ */
