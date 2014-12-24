/**
 * 	\file
 *
 *	\brief GPS high level management.
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
 *  Created on: 17.06.2011
 *      Author: Anton Glukhov
 */

#if (ARCH == AVR)
//	#include <string.h>
//	#include <avr/sleep.h>
//	#include <avr/interrupt.h>
//  #include <util/delay.h>
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/ims.h"
#include "include/gps.h"

#include "usart.h"
#include "timer.h"

volatile uint8_t uart_buffer = 0;

volatile uint8_t FLAG = 0;

#if 0
uint8_t ParseString(char *string, _str_locate_t *str_dat)
{
	uint8_t j = 0;
	char tmp[10];
	uint8_t buf_comma[16];

/*
*         1         2       3 4        5 6 7  8   9  10 11 12 13  14   15
*         |         |       | |        | | |  |   |   | |   | |   |    |
*  $--GGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh
*/

	if (strncmp((char *)string, "$GPGGA", 6)) return ERR_UNK_PACKAGE;


	for(uint8_t i = 0; (string[i] != 0x0D); i++)
	{
		if (string[i] == ',') buf_comma[j++] = i;
	}

	buf_comma[j] = '\0';


	/* time_hours */
	if (buf_comma[0] != (buf_comma[1] - 1))
	{
		strlcpy(&tmp[0], &string[buf_comma[0] + 1], 3);
		str_dat->time_h = (uint8_t)atoi(&tmp[0]);
		strlcpy(&tmp[0], &string[buf_comma[0] + 3], 3);
		str_dat->time_m = (uint8_t)atoi(&tmp[0]);
		strlcpy(&tmp[0], &string[buf_comma[0] + 5], 3);
		str_dat->time_s = (uint8_t)atoi(&tmp[0]);
	}
	else
	{
		str_dat->time_h = 0;
		str_dat->time_m = 0;
		str_dat->time_s = 0;
	}


	/* latitude */
	if (buf_comma[1] != (buf_comma[2] - 1))
	{
		strlcpy(&tmp[0], &string[buf_comma[1] + 1], 5);
		str_dat->latitude_d = (uint16_t)atoi(&tmp[0]);
		strlcpy(&tmp[0], &string[buf_comma[1] + 6], 5);
		str_dat->latitude_m = (uint16_t)atoi(&tmp[0]);
	}
	else
	{
		str_dat->latitude_d = 0;
		str_dat->latitude_m = 0;
	}

	//
	if (buf_comma[2] != (buf_comma[3] - 1))
	{
		str_dat->ns = string[buf_comma[2] + 1];
	}
	else
	{
		str_dat->ns = 0;
	}

	/* longtitude */
	if (buf_comma[3] != (buf_comma[4] - 1))
	{
		strlcpy(&tmp[0], &string[buf_comma[3] + 1], 6);
		str_dat->longitude_d = (uint16_t)atoi(&tmp[0]);
		strlcpy(&tmp[0], &string[buf_comma[3] + 7], 5);
		str_dat->longitude_m = (uint16_t)atoi(&tmp[0]);
	}
	else
	{
		 str_dat->longitude_d = 0;
		 str_dat->longitude_m = 0;
	}

	//
	if (buf_comma[4] != (buf_comma[5] - 1))
	{
		 str_dat->ew = string[buf_comma[4] + 1];
	}
	else
	{
		 str_dat->ew = 0;
	}

	return ERR_NO_ERROR;
}


uint8_t WaitCharset(char chr)
{
	while (!CHKBIT_REG(FLAG, FLAG_TIM1))
	{
//		if ((CHKBIT_REG(FLAG, FLAG_USART)) && (uart_buf == chr))
//		{
//			return ERR_NO_ERROR;
//		}

#if (ARCH == AVR)
//		set_sleep_mode(SLEEP_MODE_IDLE);
//		sleep_mode();
#endif

#if (ARCH == STM)
		/* Code here*/
#endif
	}

	return ERR_TIME_OUT;
}

uint8_t getString(char *test_buf, size_t size, USART_TypeDef* num, uint8_t(*p)(char))
{
	char *cbuf = test_buf;

	uint8_t CharCount = 0;

	FLAG = 0;
	uart_buffer = 0;

//	TimerOn(0xFC17, TIM_DIV_1024);	/* ~100ms */	// divider 1024; T = 1024/F_CPU = 0,0001024 sec; T*1000 = 0,1024 sec -> count 1000(dec) = 3E8(hex) -> start value (FFFF - 03E8) = FC17
//	TimerInterruptOn();

	Timer_Configuration(TIM3);
	USART_ITConfig(GSM_USART_PORT, USART_IT_RXNE, ENABLE);
//	USART_Cmd(num, ENABLE);

	__enable_irq();

	/* Waiting for the first character */
//	if(p != NULL)
//	{
//		if (((*p)('$')) != ERR_NO_ERROR)
//		{
//			cli();
//			TimerOff();
//			USART_InterruptOff(num);
//			return ERR_TIME_OUT;
//		}
//		else
//		{
//			*(cbuf++) = uart_buf;
//			CLEARBIT(FLAG, FLAG_TIM1);
//			CLEARBIT(FLAG, FLAG_USART);
//		}
//	}

//	else /* skip <LF> and <CR> chars */
//	{
//		while()
//	}

	while(!CHKBIT_REG(FLAG, FLAG_TIM1))
	{
		if (CharCount >= size) return ERR_LENGTH;

	//TODO::flow control
//#if 0
//		/* flow control */
//		if (CharCount >= size)
//		{
//			/* flow control */
//			SETBIT(GSM_PORT_RTS, GSM_NPORT_RTS);
//			/* <set bufer overflow FLAG> */
//			SETBIT(FLAG, BUFOVF);
//			cli();
//			TimerOff();
//			USART_InterruptOff(GPS_USART_PORT);
//			return ERR_BUF_FULL;
//		}
//#endif

		if (CHKBIT_REG(FLAG, FLAG_USART))	/* USART flag */
		{
			if ((uart_buffer == 0x0A) || (uart_buffer == 0x0D))
			{
				if (CharCount == 0)
					continue;
				__disable_irq();
				TimerOff(TIM3);
				USART_ITConfig(GSM_USART_PORT, USART_IT_RXNE, DISABLE);
//				USART_InterruptOff(num);
				*cbuf = '\0';
				return ERR_NO_ERROR;
			}
			else
			{
				CharCount++;
				*(cbuf++) = uart_buffer;
				CLEARBIT(FLAG, FLAG_USART);
			}
		}

#if (ARCH == AVR)
//		set_sleep_mode(SLEEP_MODE_IDLE);
//		sleep_mode();
#endif

	}
	/* end while ( <timer not occur> ) */
//	cli();
	__disable_irq();
	TimerOff(TIM3);
//	TimerInterruptOff();
//	USART_InterruptOff(num);
	USART_ITConfig(GSM_USART_PORT, USART_IT_RXNE, DISABLE);
	return ERR_TIME_OUT;
}
#endif


void gpsOn(void)
{
	GPIO_HIGH(MCU_GPS_PORT, MCU_GPS_PIN);
	Delay_ms(1000);
	GPIO_HIGH(GPS_RST_PORT, GPS_RST_PIN);
}


void gpsOff(void)
{

}


#if 0
uint8_t checkGPSModule()
{
	uint8_t result = ERR_NO_ERROR;

	//TODO:: interrupt turn on
//	gps_turn_on();
	_delay_ms(2000);
	if (getLocation()) result = ERR_GPS_MODULE_OFF;
//	gps_turn_off();

	return result;
}
#endif
