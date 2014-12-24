/**
 *	\file
 *
 *	\brief Main file
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

/** \mainpage
 * 	\section intro Introduction
 * 	This software is IMS Tracker for AVR and STM microcontrollers.
 *
 *
 *  \section files Main Files
 * <H4>High Level:</H4>
 * - ims.c: the main file
 * - gps.c: API for gps module
 * - gsm.c: API for gsm module
 * - int_handler.c: interrupt handlers
 * - err.c: error handlers
 *
 * <H4>Low Level:</H4>
 * - ioport.c: I/O port driver
 * - usart.c: USART driver
 * - timer.c: TIMER driver
 * - eeprom.c: EEPROM driver
 * - wdt.c: WatchDOG timer driver
 *
 * <H4>Tests:</H4>
 * - GPStest.c: gps test
 * - GSMtest.c: gsm test
 * - SELFtest.c: self-test
 *
 * <H4>Test number: +79295170921</H4>
 *
 *
 *  \section example Description of the example
 *
 *  \section compinfo Compilation Info
 * This software was written for the GNU GCC and IAR for AVR.
 * Other compilers may or may not work.
 *
 *  \section contactinfo Contact Information
 * For further information, mail-to
 * <A href=mailto:glukhov@elins.ru>Anton Glukhov</A>.\n
 *
 *  \section todolist TODO list:
 * TODO::Change error numbers
 */

#if (ARCH == AVR)
	#include <string.h>
//	#include <avr/wdt.h>
//	#include <util/delay.h>
//	#include <avr/sleep.h>
//	#include <avr/interrupt.h>
	#include <stdlib.h>
#include <stdio.h>
#endif

#include "include/ims.h"
#include "include/gps.h"
#include "include/gsm.h"

//#include "stm32l1xx.h"
//#include "stm32l15x_it.h"

#include "usart.h"
#include "rtc.h"
//#include "ioport.h"
//#include "timer.h"
//#include "eeprom.h"
//#include "i2cmaster.h"
//#include "adc.h"
//
//#include "config/conf_usart.h"
//#include "config/conf_clock.h"

uint8_t state;
uint8_t usermode;

_dev_info_t 	Device;
_module_info_t 	GSMModule;
_gsm_quality_t  GSMSignal;


#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"



void vApplicationTickHook( void )
{
	/* This function will be called by each tick interrupt if
	configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
	added here, but the tick hook is called from an interrupt context, so
	code must not attempt to block, and only the interrupt safe FreeRTOS API
	functions can be used (those that end in FromISR()). */
//	_DEBUG("vApplicationTickHook!\r\n");
}


void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	_DEBUG("vApplicationMallocFailedHook!\r\n");
	taskDISABLE_INTERRUPTS();
	for( ;; );
}


void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
	_DEBUG("vApplicationIdleHook!\r\n");
}



void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
volatile unsigned long ulSetToNonZeroInDebuggerToContinue = 0;

	/* Parameters are not used. */
	( void ) ulLine;
	( void ) pcFileName;

	taskENTER_CRITICAL();
	{
		while( ulSetToNonZeroInDebuggerToContinue == 0 )
		{
			/* Use the debugger to set ulSetToNonZeroInDebuggerToContinue to a
			non zero value to step out of this function to the point that raised
			this assert(). */
			__asm volatile( "NOP" );
			__asm volatile( "NOP" );
		}
	}
	taskEXIT_CRITICAL();
}




void Task1( void *pvParameters )
{
    for(;;)
    {
    	_DEBUG("Task1.");
    	GPIO_TOGGLE(GPIO_LED_PORT, GPIO_LED_PIN);
    	vTaskDelay( 1000 );
    }
    vTaskDelete( NULL );
}


void vUARTReport( void *pvParameters )
{
    for(;;)
    {
    	_DEBUG("Task2.");
        vTaskDelay( 1000 );
    }
    vTaskDelete( NULL );
}


void types_print(void)
{
	_DEBUG("short - %d", sizeof(short));
	_DEBUG("int - %d", sizeof(int));
	_DEBUG("unsigned long - %d", sizeof(unsigned long));
	_DEBUG("unsigned long long - %d", sizeof(unsigned long long));
	_DEBUG("float - %d", sizeof(float));
	_DEBUG("double - %d", sizeof(double));
}


void board_init(void)
{
	int ret;
	uint8_t buf[100];

	_DEBUG("\r\nHardware init...");

	/* Device info */
	Device.hw_version = HW_VERSION;
	Device.sw_version_maj = SW_VERSION_MAJ;
	Device.sw_version_min = SW_VERSION_MIN;
	strcpy(Device.sn, SN);

	/* GSM module info */

	gsmOn();

	SETBIT(gsm_module_config, CFG_ECHO_MODE);

	if (!checkGSMModule())
		_DEBUG("GSM module not found");
		//TODO:: Add exception if GSM can't be found!
	else
		_DEBUG("GSM module:");

	checkGSMNetwork();

	ret = getManufacturerId(&buf[0]);
	if (ret > 0)
	{
		strcpy(GSMModule.manufacturer_id, &buf[0]);
		_DEBUG("  Manufacturer id - %s", GSMModule.manufacturer_id);
	}

	memset(&buf[0], 'n', 30);
	ret = getModelId(&buf[0]);
	if (ret > 0)
	{
		strcpy(GSMModule.model_id, &buf[0]);
		_DEBUG("  Model id - %s", GSMModule.model_id);
	}

	memset(&buf[0], 'n', 30);
	ret = getRevision(&buf[0]);
	if (ret > 0)
	{
		strcpy(GSMModule.revision, &buf[0]);
		_DEBUG("  Revision - %s", GSMModule.revision);
	}

	memset(&buf[0], 'n', 30);
	ret = getSerialNumber(&buf[0]);
	if (ret > 0)
	{
		GSMModule.sn = strtoull(&buf[0], NULL, 10);
		_DEBUG("  Serial number - %llu", GSMModule.sn);
	}

	memset(&buf[0], 'n', 30);
	ret = getIMSI(&buf[0]);
	if (ret > 0)
	{
		GSMModule.imsi = strtoull(&buf[0], NULL, 10);
		_DEBUG("  IMSI - %llu", GSMModule.imsi);
	}

	memset(&buf[0], 'n', 30);
	ret = updateGSMSignal();
	if (ret > 0)
	{
		_DEBUG("  GSM signal: %d", GSMSignal.rssi);
	}
//	ret = getGSMSignal(&signal);
//	if (ret > 0)
//	{
//		_DEBUG("  GSM signal: %d", signal.rssi);
//	}

	gsmOff();

	/* GPS module info */

	/* Accel info */
#ifndef DISCOVERY
	Accel_Configuration();
#endif
}


int main(void)
{

	mcu_init();

	_DEBUG("Start delay, not block in low power mode...");
	Delay_ms(2000);

#ifdef TEST
	#include "embUnit/embUnit.h"

	TestRef SELFTest_test(void);
	TestRef ErrorHandlerTest_test(void);
	TestRef AtParserTest_test(void);
	TestRef SmsParserTest_test(void);
	TestRef SmsCmdInQueueTest_test(void);
	TestRef NMEAParserTest_test(void);

	_DEBUG("Start testing...", NULL);
	TestRunner_start();
	TestRunner_runTest(SELFTest_test());
	TestRunner_runTest(ErrorHandlerTest_test());
	TestRunner_runTest(AtParserTest_test());
	TestRunner_runTest(SmsParserTest_test());
	TestRunner_runTest(SmsCmdInQueueTest_test());
	TestRunner_runTest(NMEAParserTest_test());
//	TestRunner_runTest(GSMTest_test());
//	TestRunner_runTest(GPSTest_test());
	TestRunner_end();
	_DEBUG("Testing done.", NULL);
#endif

	board_init();

	while(1){

		gsmOn();

		SETBIT(gsm_module_config, CFG_ECHO_MODE);

		checkGSMNetwork();

//		coap_test();
		gsm_betta();

		gsmOff();

		Delay_ms(1000);
	}

	return 0;
}


void mcu_init(void)
{
	RCC_Configuration();

	GPIO_Configuration();

	USART_Configuration();

	I2C_Configuration();

	_DEBUG("========== START ==========", NULL);

	RTC_Configuration();

//	RTC_Alarm_Configuration();

	NVIC_Configuration();

//	SysTickConfig();

	Timer2_Configuration();
}

#ifdef DISCOVERY
void RCC_Configuration(void)
{

	/* Enable HSI Clock */
	RCC_HSICmd(ENABLE);

	/*!< Wait till HSI is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

	/* Set HSI as sys clock*/
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);

	/* Set MSI clock range to ~4.194MHz*/
	RCC_MSIRangeConfig(RCC_MSIRange_6);

	/* Enable the GPIOs clocks */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC , ENABLE);

	/* Enable comparator, LCD and PWR mngt clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_COMP | RCC_APB1Periph_PWR | RCC_APB1Periph_I2C1 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM2 ,ENABLE);

	/* Enable ADC & SYSCFG clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_SYSCFG , ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* Allow access to the RTC */
	PWR_RTCAccessCmd(ENABLE);

	/* Reset RTC Backup Domain */
	RCC_RTCResetCmd(ENABLE);
	RCC_RTCResetCmd(DISABLE);

	/* LSE Enable */
	RCC_LSEConfig(RCC_LSE_ON);

	/* Wait until LSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

	/* RTC Clock Source Selection */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	/* Enable the RTC */
	RCC_RTCCLKCmd(ENABLE);

	/*Disable HSE*/
	RCC_HSEConfig(RCC_HSE_OFF);
	if(RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET )
	{
		/* Stay in infinite loop if HSE is not disabled*/
		while(1);
	}
}
#else
void RCC_Configuration(void)
{

	RCC_HSEConfig(ENABLE);

	while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);

	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);

	/* Enable the GPIOs clocks */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC| RCC_AHBPeriph_GPIOD, ENABLE);

	/* Enable comparator, LCD and PWR mngt clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_I2C1 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM2 ,ENABLE);

	/* Enable ADC & SYSCFG clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG , ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* RTC configuration in rtc.c file */
}

#endif


void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure1;
	NVIC_InitTypeDef NVIC_InitStructure2;
	NVIC_InitTypeDef NVIC_InitStructure3;
	NVIC_InitTypeDef NVIC_InitStructure4;
	NVIC_InitTypeDef NVIC_InitStructure5;
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure2;
	EXTI_InitTypeDef EXTI_InitStructure3;

	//Test Timer interrupt
	NVIC_InitStructure1.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure1);

	//Timer interrupt
	NVIC_InitStructure3.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure3.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure3.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure3.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure3);

	NVIC_InitStructure4.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure4.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure4.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure4.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure4);

	NVIC_InitStructure5.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure5.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure5.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure5.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure5);


	//Button interrupt
//	NVIC_InitStructure2.NVIC_IRQChannel = EXTI0_IRQn ;
//	NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure2);

	/* Enable the RTC Alarm Interrupt */
//	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure2.NVIC_IRQChannel = RTC_WKUP_IRQn;
	NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure2);

//	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

	/* EXTI configuration */
//	EXTI_ClearITPendingBit(EXTI_Line17);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);

	/* EXTI configuration */
	EXTI_ClearITPendingBit(EXTI_Line20);
	EXTI_InitStructure2.EXTI_Line = EXTI_Line20;
	EXTI_InitStructure2.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure2.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure2.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure2);


	/* EXTI configuration - PA0 */
//	EXTI_ClearITPendingBit(EXTI_Line0);
//	EXTI_InitStructure2.EXTI_Line = EXTI_Line0;
//	EXTI_InitStructure2.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure2.EXTI_Trigger = EXTI_Trigger_Rising;
//	EXTI_InitStructure2.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure2);

	/* Accel int */
//	EXTI_ClearITPendingBit(EXTI_Line0);
//	EXTI_InitStructure3.EXTI_Line = EXTI_Line0;
//	EXTI_InitStructure3.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure3.EXTI_Trigger = EXTI_Trigger_Rising;
//	EXTI_InitStructure3.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure3);
//
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	__enable_irq();
}


void SysTickConfig(void)
{

}


void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure the LED_pin as output push-pull usage*/
	GPIO_InitStructure.GPIO_Pin = GPIO_LED_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIO_LED_PORT, &GPIO_InitStructure);

	memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitTypeDef));

	/* MCO - microcontroller clock output for debug system clock */
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitTypeDef));

	/* Configure the USART1 as alternative usage*/
	GPIO_InitStructure.GPIO_Pin =  USART_GSM_TX | USART_GSM_RX | USART_GSM_CTS | USART_GSM_RTS;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitTypeDef));

//	GPIO_InitStructure.GPIO_Pin = USART_GSM_DTR_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(USART_GSM_DTR_PORT, &GPIO_InitStructure);
//
//	memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitTypeDef));
//
//	GPIO_InitStructure.GPIO_Pin = USART_GSM_DSR_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(USART_GSM_DSR_PORT, &GPIO_InitStructure);
//
//	memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitTypeDef));
//
//	GPIO_InitStructure.GPIO_Pin = USART_GSM_DCD_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(USART_GSM_DCD_PORT, &GPIO_InitStructure);
//
//	memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitTypeDef));
//
//	GPIO_InitStructure.GPIO_Pin = USART_GSM_RI_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(USART_GSM_RI_PORT, &GPIO_InitStructure);
//
//	memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitTypeDef));

	/* Configure the USART2 as alternative usage*/
	GPIO_InitStructure.GPIO_Pin =  USART_GPS_TX | USART_GPS_RX;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitTypeDef));

	/* Configure the USART3 as alternative usage*/
	GPIO_InitStructure.GPIO_Pin =  USART_DBG_TX | USART_DBG_RX;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitTypeDef));

	/* Configure the I2C1 as alternative usage*/
	GPIO_InitStructure.GPIO_Pin =  I2C_SDA | I2C_SCL;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = MCU_GPS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(MCU_GPS_PORT, &GPIO_InitStructure);

	memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitTypeDef));

//	GPIO_InitStructure.GPIO_Pin = MCU_GPSR_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(MCU_GPSR_PORT, &GPIO_InitStructure);
//
//	memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitTypeDef));

	GPIO_InitStructure.GPIO_Pin = GPS_RST_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPS_RST_PORT, &GPIO_InitStructure);

	memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitTypeDef));

	GPIO_InitStructure.GPIO_Pin = GPS_PPS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPS_PPS_PORT, &GPIO_InitStructure);

	memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitTypeDef));


	GPIO_InitStructure.GPIO_Pin = MCU_GSM_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(MCU_GSM_PORT, &GPIO_InitStructure);

	memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitTypeDef));

	GPIO_InitStructure.GPIO_Pin = GSM_RST_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GSM_RST_PORT, &GPIO_InitStructure);

	memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitTypeDef));

	GPIO_InitStructure.GPIO_Pin = GSM_ON_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GSM_ON_PORT, &GPIO_InitStructure);

	memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitTypeDef));

//	GPIO_InitStructure.GPIO_Pin = GSM_WISMO_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GSM_WISMO_PORT, &GPIO_InitStructure);

	memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitTypeDef));

	GPIO_InitStructure.GPIO_Pin = GSM_DRV_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GSM_DRV_PORT, &GPIO_InitStructure);

	memset(&GPIO_InitStructure, 0, sizeof(GPIO_InitTypeDef));

	/* Accel int */
	GPIO_InitStructure.GPIO_Pin = ACCEL_INT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(ACCEL_INT_PORT, &GPIO_InitStructure);

	/* Configure the ....*/
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_SDIO);
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_SDIO);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_USART1);
//	GPIO_PinAFConfig(USART_GSM_DTR_PORT, USART_GSM_DTR_PIN, GPIO_AF_SDIO);
//	GPIO_PinAFConfig(USART_GSM_DSR_PORT, USART_GSM_DSR_PIN, GPIO_AF_SDIO);
//	GPIO_PinAFConfig(USART_GSM_DCD_PORT, USART_GSM_DCD_PIN, GPIO_AF_SDIO);
	GPIO_PinAFConfig(USART_GSM_RI_PORT, USART_GSM_RI_PIN, GPIO_AF_SDIO);

#ifdef DISCOVERY
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_USART2);
#endif
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);

	/* GPS Control */
#ifndef DISCOVERY
	GPIO_PinAFConfig(MCU_GPS_PORT, MCU_GPS_PIN, GPIO_AF_SDIO);

	// TODO:: Conflict with I2C1 PB8 PB9
//	GPIO_PinAFConfig(MCU_GPSR_PORT, MCU_GPSR_PIN, GPIO_AF_SDIO);

	GPIO_PinAFConfig(GPS_RST_PORT, GPS_RST_PIN, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GPS_PPS_PORT, GPS_PPS_PIN, GPIO_AF_SDIO);

	/* GSM Control */
	GPIO_PinAFConfig(MCU_GSM_PORT, MCU_GSM_PIN, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GSM_RST_PORT, GSM_RST_PIN, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GSM_ON_PORT, GSM_ON_PIN, GPIO_AF_SDIO);
//	GPIO_PinAFConfig(GSM_WISMO_PORT, GSM_WISMO_PIN, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GSM_DRV_PORT, GSM_DRV_PIN, GPIO_AF_SDIO);
#endif
}


void Delay_ms(uint32_t ms)
{
        volatile uint32_t nCount;
        RCC_ClocksTypeDef RCC_Clocks;
        RCC_GetClocksFreq (&RCC_Clocks);

        nCount=(RCC_Clocks.HCLK_Frequency/10000)*ms;
        for (; nCount!=0; nCount--);
}
