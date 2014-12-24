/**
 * 	\file
 *
 *	\brief EEPROM software driver interface.
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
