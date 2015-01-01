/**
 * 	\file
 *
 *	\brief EEPROM software driver interface.
 *
 *	\page License
 *
 *  Created on: 17.07.2013
 *      Author: Anton Glukhov
 */

#include "include/ims.h"

#include "timer.h"


void Timer2_Configuration()
{
//	TIM_DeInit(TIMx);

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);

	TIM_Cmd(TIM2, DISABLE);

//	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

	/* 1ms period */
	TIM_TimeBaseStructure.TIM_Period = 1000;
#ifdef DISCOVERY
	TIM_TimeBaseStructure.TIM_Prescaler = 16000;
#else
	TIM_TimeBaseStructure.TIM_Prescaler = 12000;
#endif
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
}


void TimerOn(TIM_TypeDef* TIMx)
{
	TIM_ClearITPendingBit(TIMx, TIM_IT_Update);

	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIMx, ENABLE);
}


void TimerOff(TIM_TypeDef* TIMx)
{
	TIM_ITConfig(TIMx, TIM_IT_Update, DISABLE);

	TIM_Cmd(TIMx, DISABLE);
}


void timer_test(void)
{
	_DEBUG("Test timer...");
	_DEBUG("a");
	TimerOn(TIM2);
//	Delay_ms(3000);
//	_DEBUG("b");
//	TimerOff(TIM2);
//	TIM_SetAutoreload(TIM2, 500);
//	TIM_SetCounter(TIM2, 0);
//	TimerOn(TIM2);

	while(1);
}
