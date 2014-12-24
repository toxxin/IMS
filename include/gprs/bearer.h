/*
 * bearer.h
 *
 *  Created on: 29.01.2014
 *      Author: anton
 */

#ifndef BEARER_H_
#define BEARER_H_

//TODO:: correct doxygen documentation
//TODO:: save gprs default params in EEPROM
#define GPRS_DEFAULT_LOGIN		"gdata"
#define GPRS_DEFAULT_PASSWORD	"gdata"
#define GPRS_DEFAULT_APN		"internet"
#define GPRS_DEFAULT_TYPE		BearerIdentifyGPRS


/** @defgroup BEARER_AUTH */
#define GPRS_BEARER_AUTH_PAP		BIT(0)		//0x01
#define GPRS_BEARER_AUTH_CHAR		BIT(1)		//0x02
#define GPRS_BEARER_AUTH_MSCHAP1	BIT(2)		//0x04
#define GPRS_BEARER_AUTH_MSCHAP2	BIT(3)		//0x08
/** @} */

/** @defgroup BEARER_CMD
 * @{
 */
#define GPRS_BEARER_CMD_CLOSE			0
#define GPRS_BEARER_CMD_OPEN			1
#define GPRS_BEARER_CMD_SET				2
#define GPRS_BEARER_CMD_GET				3
#define GPRS_BEARER_CMD_START			4
#define GPRS_BEARER_CMD_STOP			5
#define GPRS_BEARER_CMD_CONFIG			6
/**
  * @}
  */

/** @defgroup BEARER_IDENTIFY
 * @{
 */
#define BearerIdentifyGSM				5
#define BearerIdentifyGPRS				6
/**
  * @}
  */

/** @defgroup BEARER_OPTIONS
  * @{
  */
#define GPRS_BEARER_OPT_LOGIN			0
#define GPRS_BEARER_OPT_PASSWORD		1
#define GPRS_BEARER_OPT_PHONE			2
#define GPRS_BEARER_OPT_PPP_PAP			7
#define GPRS_BEARER_OPT_PPP_CHAP		8
#define GPRS_BEARER_OPT_PPP_MSCHAP1		9
#define GPRS_BEARER_OPT_PPP_MSCHAP2		10
#define GPRS_BEARER_OPT_APN				11
#define GPRS_BEARER_OPT_HEADERCOMP		13
#define GPRS_BEARER_OPT_DATACOMP		14
#define GPRS_BEARER_OPT_IP_ADDR			15
#define GPRS_BEARER_OPT_IP_DST_ADDR		16
#define GPRS_BEARER_OPT_DNS1			17
#define GPRS_BEARER_OPT_DNS2			18
/**
  * @}
  */


typedef struct Bearer
{
	uint8_t bid;
	char login[32];
	char password[32];
	char phone[32];
	uint8_t enable_auth;
	char apn[96];
	uint8_t enable_datacomp;
	char ip_addr[32];
	char dst_ip_addr[32];
	char dns1[32];
	char dns2[32];

	int (*start)(struct Bearer*);
	int (*stop)(struct Bearer*);
	int (*get)(struct Bearer*, uint8_t);
	int (*set)(struct Bearer*, uint8_t, char*);
	int (*open)(struct Bearer*);
	int (*close)(struct Bearer*);

	/* save in gsm module */
	/* int state??? */
} Bearer;


typedef struct
{
	char login[32];
	char password[32];
	char apn[96];
	uint8_t type;
} Bearer_InitTypeDef;


void BearerInit(Bearer *bearer, Bearer_InitTypeDef *Bearer_InitStruct);
int BearerDefInit(Bearer *bearer);
static int openBearer(Bearer *bearer);
int closeBearer(Bearer *bearer);
int startBearer(Bearer *bearer);
int stopBearer(Bearer *bearer);
int getBearerOption(Bearer*, uint8_t);
int setBearerOption(Bearer *bearer, uint8_t option, char *value);


#endif /* BEARER_H_ */
