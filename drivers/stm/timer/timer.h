/*
 * timer.h
 *
 *  Created on: 22.07.2013
 *      Author: Anton
 */

#ifndef TIMER_H_
#define TIMER_H_

void Timer2_Configuration(void);
void TimerOn(TIM_TypeDef* TIMx);
void TimerOff(TIM_TypeDef* TIMx);

#endif /* TIMER_H_ */
