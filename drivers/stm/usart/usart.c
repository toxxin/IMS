/**
 *	\file
 *
 *	\brief USART management
 *
 *	\page License
 *
 *  Created on: 09.06.2011
 *      Author: Anton Glukhov
 */

#include "include/ims.h"

#include "usart.h"

void USART_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;

	/* Configure the USART1 for GSM */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(GSM_USART_PORT, &USART_InitStructure);

	memset(&USART_InitStructure, 0, sizeof(USART_InitTypeDef));

	/* Configure the USART2 for GPS */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(GPS_USART_PORT, &USART_InitStructure);

	memset(&USART_InitStructure, 0, sizeof(USART_InitTypeDef));

	/* Configure the USART3 for DEBUG */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART_DBG_PORT, &USART_InitStructure);

	USART_Cmd(GSM_USART_PORT, ENABLE);
	USART_Cmd(GPS_USART_PORT, ENABLE);
	USART_Cmd(USART_DBG_PORT, ENABLE);
}


uint8_t USART_STR_TX(uint8_t *data, USART_TypeDef* USARTx)
{
	while(*data != '\0')
	{
	    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET); /* Waitwhile TX full */
	    USART_SendData(USARTx, *(uint8_t *)data);
		data++;
	}
	return E_OK;
}


void USART_BUF_TX(uint8_t *data, size_t len, USART_TypeDef* USARTx)
{
	while(len > 0)
	{
	    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET); /* Waitwhile TX full */
	    USART_SendData(USARTx, *(uint8_t *)data);
	    data++;
	    len--;
	}
}


int getBuf(uint8_t *buf, size_t sz, USART_TypeDef* num, uint16_t bt, uint16_t at)
{
	size_t cnt = 0;
	FLAG = 0;
	uart_buffer = 0;

	if (USART_GetFlagStatus(GSM_USART_PORT, USART_FLAG_ORE) != RESET)
		(void)USART_ReceiveData(GSM_USART_PORT);

	USART_ITConfig(GSM_USART_PORT, USART_IT_RXNE, ENABLE);
	TimerOff(TIM2);
	TIM_SetAutoreload(TIM2, bt);
	TIM_SetCounter(TIM2, 0);
	TimerOn(TIM2);
	while(!CHKBIT_REG(FLAG, FLAG_TIM2))
	{
		if (cnt >= sz) return ERR_LENGTH;

		if (CHKBIT_REG(FLAG, FLAG_USART))
		{
			TimerOff(TIM2);
			USART_ITConfig(GSM_USART_PORT, USART_IT_RXNE, DISABLE);
			cnt++;
			*(buf++) = uart_buffer;
			CLEARBIT(FLAG, FLAG_USART);
			CLEARBIT(FLAG, FLAG_TIM2);
			TIM_SetAutoreload(TIM2, at);
			TIM_SetCounter(TIM2, 0);
			TimerOn(TIM2);
			USART_ITConfig(GSM_USART_PORT, USART_IT_RXNE, ENABLE);
		}
	}

	TimerOff(TIM2);
	USART_ITConfig(GPS_USART_PORT, USART_IT_RXNE, DISABLE);
	return cnt;
}


#ifdef DEBUG
void USART_LINES_STATUS(void)
{
	if (GPIO_ReadInputDataBit(USART_GSM_DSR_PORT, USART_GSM_DSR_PIN))
	_DEBUG("DSR is HIGH", NULL);
	else
		_DEBUG("DSR is LOW", NULL);

	if (GPIO_ReadInputDataBit(USART_GSM_DCD_PORT, USART_GSM_DCD_PIN))
		_DEBUG("DCD is HIGH", NULL);
	else
		_DEBUG("DCD is LOW", NULL);

	if (GPIO_ReadInputDataBit(USART_GSM_RI_PORT, USART_GSM_RI_PIN))
		_DEBUG("RI is HIGH", NULL);
	else
		_DEBUG("RI is LOW", NULL);
}
#endif
