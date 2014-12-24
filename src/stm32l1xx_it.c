/**
  ******************************************************************************
  * @file    RTC/Calendar/stm32l1xx_it.c 
  * @author  MCD Application Team
  * @version V1.1.1
  * @date    13-April-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_it.h"
#include "ims.h"
#ifdef USE_STM32L152D_EVAL 
  #include "stm32l152d_eval.h"
#elif defined USE_STM32L152_EVAL 
  #include "stm32l152_eval.h"
#endif 

/** @addtogroup STM32L1xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup RTC_Calendar
  * @{
  */ 


/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}


/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}


/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}


/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}


/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}


/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
//void SVC_Handler(void)
//{
//}


/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}


/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}


/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
//}


void RTC_Alarm_IRQHandler(void)
{
  if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
  {
	  GPIO_TOGGLE(GPIOB, GPIO_Pin_7);

	  RTC_TimeTypeDef RTC_TimeStruct;

	  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
	  _DEBUG("Time - %.2d:%.2d", RTC_TimeStruct.RTC_Minutes,
			  	  	  	  	  	  	  	  	  	  RTC_TimeStruct.RTC_Seconds);

    RTC_ClearITPendingBit(RTC_IT_ALRA);
    EXTI_ClearITPendingBit(EXTI_Line17);
  }
}


void USART1_IRQHandler(void)
{
	//TODO:: refactoring!
#ifdef DISCOVERY
	usart_gps_rx_int_hanler();
#else
	usart_gsm_rx_int_hanler();
#endif
}

void USART2_IRQHandler(void)
{
#ifdef DISCOVERY
	usart_gsm_rx_int_hanler();
#else
	usart_gps_rx_int_hanler();
#endif
}


void TIM2_IRQHandler(void)
{
	timer2_ovf_handler();
}


void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
	  ext_acc_int();
  }
}

