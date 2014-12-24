/**
 * 	\file
 *	
 *	\brief Main file
 *
 *	\page License
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
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
 *  Created on: 29.08.2011
 *      Author: Anton Glukhov
 */

#ifndef IMS_H_
#define IMS_H_

#include <string.h>		/* for _DEBUG macro */
#include <stdio.h>		/* for _DEBUG macro */

#include "stm32l1xx.h"

//TODO:: Add all include's files
#include "gsm.h"
#include "gps.h"
#include "errors.h"
#include "filter.h"
#include "cmd.h"
#include "sms.h"
#include "task_queue.h"
#include "task_result_queue.h"
#include "inception_stack.h"
#include "tmap.h"
#include "lis331dlh.h"
#include "nmea/nmea.h"
#include "gprs/gprs.h"

#ifndef ARCH
	#error *** Undefined architecture! ***
#endif

#if ARCH == AVR
//	#include <avr/io.h>
#endif

//TODO:: Add type of device, MCU, frequency, etc


#define HW_VERSION	1
#define SW_VERSION_MAJ	0
#define SW_VERSION_MIN	1
#define SN "EW-1409010001-T"

#define LIS331DLM	1
//#define LIS331DLH	1


/** @name EEPROM addresses */
	#define	ID_LEN				(4)
	#define	HW_VERSION_LEN		(4)
	#define	SW_VERSION_LEN		(4)
	#define TIME_INTERVAL_LEN	(1)
	#define	ACCELEROMETER_LEN	(1)
	#define STATE_LEN			(1)
	#define SMS_PASS_LEN		(4)
	#define PHONE_LEN			(12)
	#define PHONE1_LEN			(12)
	#define PHONE2_LEN			(12)
	#define	SERVER1_LEN			(64)
	#define	SERVER2_LEN			(64)
	#define	CRC_LEN				(1)

	#define	EEPROM_ID_ADDR				(EEPROM_START_ADDR)
	#define	EEPROM_HW_VERSION_ADDR		(EEPROM_START_ADDR + 0x0004)
	#define	EEPROM_SW_VERSION_ADDR		(EEPROM_START_ADDR + 0x0004)
	#define EEPROM_TIME_INTERVAL_ADDR 	(EEPROM_START_ADDR + 0x000C)
	#define EEPROM_STATE_ADDR			(EEPROM_START_ADDR + 0x000D)
	#define EEPROM_SMS_PASS_ADDR		(EEPROM_START_ADDR + 0x000E)
	#define EEPROM_PHONE1_ADDR			(EEPROM_START_ADDR + 0x0012)
	#define EEPROM_PHONE2_ADDR			(EEPROM_START_ADDR + 0x001E)
	#define EEPROM_SERVER1_ADDR			(EEPROM_START_ADDR + 0x0030)
	#define	EEPROM_SERVER2_ADDR			(EEPROM_START_ADDR + 0x0070)
	#define	EEPROM_CRC_ADDR				(EEPROM_START_ADDR + 0x00B0)
	#define	EEPROM_DBG_ADDR				(EEPROM_START_ADDR + 0x00B1)

#if ARCH == STM
	#define EEPROM_START_ADDR		(0x08080000)
	#define	EEPROM_END_ADDR			(0x08080FFF)
#endif

/** @} */


/** @name Default settings */
#define	DEFAULT_MODE			""
#define	DEFAULT_SMS_PASS		"1234"
#define	DEFAULT_SERVER1			"easywhere.ru"
#define	DEFAULT_SERVER2			"dev.easywhere.ru"
#define DEFAULT_PORT			7777
#define DEFAULT_EMAIL			"user@user.ru"
/** @} */


/** @name Device properties */
typedef struct
{
	char	 		sn[20];
	uint16_t		hw_version;
	uint8_t			sw_version_maj;
	uint8_t			sw_version_min;
	uint16_t		type;
	uint8_t			battery_stat;
	uint16_t		time_interval;
//	uint8_t			mode;
	uint8_t			state;
	char			sms_pass[SMS_PASS_LEN];
	char			phone_number1[PHONE1_LEN];
	char			phone_number2[PHONE2_LEN];
	char			server1[SERVER1_LEN];
	char			server2[SERVER2_LEN];
} _dev_info_t;
/** @} */

extern _dev_info_t 		Device;


/** @name GSM Module properties */
typedef struct
{
	char		 	manufacturer_id[40];
	char			model_id[40];
	char			revision[40];
	uint64_t		sn;
	uint64_t		imsi;
} _module_info_t;
/** @} */

extern _module_info_t 	GSMModule;


/**	@name Device state */
#define IDLE_MODE			(00)
#define	STANDBY_MODE		(01)
#define	GPS_MODE			(02)
#define	SEND_SMS_MODE		(03)
#define RECEIVE_SMS_MODE	(04)
#define GPRS_MODE			(05)

#define HW_CHECK			(07)
/** @} */


/**	@name User mode */
//#define IDLE			(0)
#define	PERIODIC		(1)
#define	CONTINUOUS		(2)
#define	ACCELEROMETER	(3)
/** @} */


#define BIT(x) (1 << (x))

/* return 0 - if bit is clear */
//#define CHKBIT_REG(reg, nbit)  (reg & (1 << nbit))
#define CHKBIT_REG(reg, BIT)  (reg & BIT)

extern uint8_t	state;		// Operation state of MCU
extern uint8_t	usermode;	// User operation mode

/**
 * 	\brief	MCU initialization
 *
 * 	This function initializes the device periphery.
 */
void mcu_init(void);

//TODO:: Doxygen info
void RCC_Configuration(void);
void NVIC_Configuration(void);
void RTC_Configuration(void);
void SysTickConfig(void);
void GPIO_Configuration(void);
void USART_Configuration(void);



/**
 * 	\brief	Error handler
 *
 * 	This function takes error number and print if DEBUG mode is active or
 * 	generate another exception.
 *
 * 	\param ErrNumber		- error number
 */
void ErrHandler(uint8_t ErrNumber);


/**
 * 	\brief	Checking hardware
 *
 * 	This function checks all hardware environment.
 *
 * 	\retval ErrorNumber
 */
void checkHW(void);


/**
 * 	\brief	Delay
 *
 * 	This function implements delay function.
 *
 * 	\param ms		- milliseconds
 */
void Delay_ms(uint32_t ms);


/**
 * 	\brief	Checking external supply
 *
 * 	This function return 0 if BAT power.
 *
 * 	\retval PortValue
 */
inline static uint8_t checkExtVCC()
{
#if (ARCHIT == AVR)
//	return CHKBIT_REG(PORTC, PC3);	/* return 0 if BAT power */
#endif

#if (ARCHIT == STM)
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);
	//TODO::Change port declaration
#endif
}


#ifdef DEBUG
//    #define _DEBUG(fmt, args...) fprintf(stderr, "\r\n%s:%s:%d: "fmt, __FILE__, __FUNCTION__, __LINE__, ##args)
	#define _DEBUG(fmt, args...) fprintf(stderr, "\r\n"fmt, ##args)
#else
    #define _DEBUG(fmt, args...)
#endif


#ifdef DISCOVERY

	#define	GPIO_LED_PORT	GPIOB
	#define	GPIO_LED_PIN	GPIO_Pin_6

	#define GSM_USART_PORT	USART2
	#define	GPS_USART_PORT	USART1
	#define USART_DBG_PORT	USART3

	#define USART_GSM_TX 	GPIO_Pin_2
	#define USART_GSM_RX 	GPIO_Pin_3
	#define USART_GSM_CTS	GPIO_Pin_0
	#define USART_GSM_RTS	GPIO_Pin_1

#else

	#define	GPIO_LED_PORT	GPIOA
	#define	GPIO_LED_PIN	GPIO_Pin_5

	#define GSM_USART_PORT	USART1
	#define	GPS_USART_PORT	USART2
	#define USART_DBG_PORT	USART3

	#define USART_GSM_TX 	GPIO_Pin_9
	#define USART_GSM_RX 	GPIO_Pin_10
	#define USART_GSM_CTS	GPIO_Pin_11
	#define USART_GSM_RTS	GPIO_Pin_12

#endif


#define USART_GSM_DTR_PORT	GPIOB
#define USART_GSM_DTR_PIN	GPIO_Pin_15

#define USART_GSM_DSR_PORT	GPIOB
#define USART_GSM_DSR_PIN	GPIO_Pin_13

#define USART_GSM_DCD_PORT	GPIOB
#define USART_GSM_DCD_PIN	GPIO_Pin_14

#define USART_GSM_RI_PORT	GPIOA
#define USART_GSM_RI_PIN	GPIO_Pin_4


#define USART_GPS_TX	GPIO_Pin_2
#define USART_GPS_RX	GPIO_Pin_3

#define USART_DBG_TX	GPIO_Pin_10
#define USART_DBG_RX	GPIO_Pin_11

#define I2C_MEMS_BUS	I2C1

#define I2C_SDA			GPIO_Pin_9
#define I2C_SCL			GPIO_Pin_8

#ifdef DISCOVERY
#define ACCEL_INT_PORT	GPIOC
#define ACCEL_INT_PIN	GPIO_Pin_0
#else
#define ACCEL_INT_PORT	GPIOA
#define ACCEL_INT_PIN	GPIO_Pin_0
#endif


/* GPS Control */
#define MCU_GPS_PORT	GPIOA
#define MCU_GPS_PIN		GPIO_Pin_6

#define MCU_GPSR_PORT	GPIOB
#define MCU_GPSR_PIN	GPIO_Pin_3

#define GPS_RST_PORT	GPIOB
#define GPS_RST_PIN		GPIO_Pin_6

#define GPS_PPS_PORT	GPIOB
#define GPS_PPS_PIN		GPIO_Pin_0


/* GSM Control */
#define MCU_GSM_PORT	GPIOA
#define MCU_GSM_PIN		GPIO_Pin_7

#define GSM_RST_PORT	GPIOB
#define GSM_RST_PIN		GPIO_Pin_7

#define GSM_ON_PORT		GPIOB
#define GSM_ON_PIN		GPIO_Pin_4

#define GSM_WISMO_PORT	GPIOB
#define GSM_WISMO_PIN	GPIO_Pin_1

#define GSM_DRV_PORT	GPIOB
#define GSM_DRV_PIN		GPIO_Pin_5


#define GPIO_HIGH(a,b) 		a->BSRRL = b
#define GPIO_LOW(a,b)		a->BSRRH = b
#define GPIO_TOGGLE(a,b) 	a->ODR ^= b

//#define SETBIT(a,b) ((a) |= (1<<(b)))
//#define CLEARBIT(a,b) ((a) &= ~(1<<(b)))

#define SETBIT(a,b) ((a) |= b)
#define CLEARBIT(a,b) ((a) &= ~(b))


/**	@name Doxygen defined */
#define DOXYGEN
/** @} */


/**	@name Debug settings */
//#define DEBUG
//#define PRINTF
//#define TEST
/** @} */


#define MAX_STRING_LEN		500
#define MAX_SUBSTRING_LEN	100

#endif /* IMS_H_ */

