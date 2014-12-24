/**
 * 	\file
 *	
 *	\brief Interrupt handle
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
 *  Created on: 15.06.2011
 *      Author: Anton Glukhov
 */


#include "include/ims.h"


void ext_gsm_int(void);
void ext_event_int(void);
void ext_adc_int(void);


uint8_t state;
uint8_t usermode;

/**
 * 	\brief	WDT interrupt handler
 *
 * 	This function handles interrupt from WDT.
 */
void wdt_int_handler()
{
	/* Code here */
	_DEBUG("wdt");
}


/**
 * 	\brief	GPS receive interrupt handler
 *
 * 	This function handles interrupt from USART(GPS).
 *
 *  \return error		- error number
 */
void usart_gps_rx_int_hanler()
{
	/* If Overrun occures, clear the OVR condition */
	if (USART_GetFlagStatus(GPS_USART_PORT, USART_FLAG_ORE) != RESET)
	{
		_DEBUG("OR interrupt GPS");
		(void)USART_ReceiveData(GPS_USART_PORT);
	}

	if(USART_GetITStatus(GPS_USART_PORT, USART_IT_RXNE) != RESET)
    {
//    	USART_ClearITPendingBit(GSM_USART_PORT, USART_IT_RXNE); /* not need if receiveData here */
    	uart_buffer = (uint8_t)USART_ReceiveData(GPS_USART_PORT);
    	SETBIT(FLAG, FLAG_USART);
    }
}


void usart_gsm_rx_int_hanler()
{
	/* If Overrun occures, clear the OVR condition */
	if (USART_GetFlagStatus(GSM_USART_PORT, USART_FLAG_ORE) != RESET)
	{
		_DEBUG("OR interrupt GSM");
	}

    if(USART_GetITStatus(GSM_USART_PORT, USART_IT_RXNE) != RESET)
    {
//    	USART_ClearITPendingBit(GSM_USART_PORT, USART_IT_RXNE); /* not need if receiveData here */
    	uart_buffer = (uint8_t)USART_ReceiveData(GSM_USART_PORT);
    	SETBIT(FLAG, FLAG_USART);
    }
}


/**
 * 	\brief	Timer overflow interrupt handler
 *
 * 	This function handles interrupt from Timer2 which used for receiving data
 * 	from USART channels.
 *
 *  \return error		- error number
 */
void timer2_ovf_handler()
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		SETBIT(FLAG,FLAG_TIM2);
	}
}


void ext_gsm_int()	/* RING */
{
	if (state == HW_CHECK)	return;

	switch(state)
	{
	case SEND_SMS_MODE:
		break;
	case RECEIVE_SMS_MODE:
		break;

	default:

		break;
	}
	/* Code here */
}


void ext_event_int()
{
	if (usermode == GPRS_MODE)
	{
		// GPRS Conection - if URC(Unresolicited Result Code) mode active
	}

	if (usermode == CONTINUOUS)
	{
		switch(state)
		{
		case SEND_SMS_MODE:
			//
			break;
		case RECEIVE_SMS_MODE:
			//
			break;

		default:
			//
			break;
		}
	}
}


/**
 * 	\brief	Accelerometer interrupt handler
 *
 * 	This function handles interrupt from accelerometer GPIO.
 *
 *  \return void
 */
void ext_acc_int()
{
	SetInt1Configuration(INT_XHIE_DISABLE | INT_XLIE_DISABLE |
						INT_YHIE_DISABLE | INT_YLIE_DISABLE |
						INT_ZHIE_DISABLE | INT_ZLIE_DISABLE | INT_OR);
    /* Toggle LED1 */
    _DEBUG("int");

    /* code here */

    /* Clear the EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);

    ResetInt1Latch();

	SetInt1Configuration(INT_XHIE_ENABLE | INT_XLIE_DISABLE |
						INT_YHIE_ENABLE | INT_YLIE_DISABLE |
						INT_ZHIE_ENABLE | INT_ZLIE_DISABLE | INT_OR);
}


void ext_adc_int()
{
	/* Code here */
}


void RTC_WKUP_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
	{
	  GPIO_TOGGLE(GPIO_LED_PORT, GPIO_LED_PIN);

	  RTC_TimeTypeDef RTC_TimeStruct;

	  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
	  _DEBUG("Time in interrupt - %.2d:%.2d", RTC_TimeStruct.RTC_Minutes,
			  	  	  	  	  	  	  	  	  	  RTC_TimeStruct.RTC_Seconds);

	  RTC_ClearITPendingBit(RTC_IT_WUT);
	  EXTI_ClearITPendingBit(EXTI_Line20);
	}
}
