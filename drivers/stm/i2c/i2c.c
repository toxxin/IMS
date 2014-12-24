/*
 * i2c.c
 *
 *  Created on: 26.09.2013
 *      Author: Anton
 */

#include "i2c.h"
#include "ims.h"

//TODO:: add timeout

void I2C_Configuration(void)
{
	I2C_InitTypeDef I2C_InitStructure;

	I2C_DeInit(I2C_MEMS_BUS);

	I2C_InitStructure.I2C_Mode  = I2C_Mode_I2C;
	I2C_InitStructure.I2C_ClockSpeed = 400000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_OwnAddress1 = 1;

	I2C_Init(I2C_MEMS_BUS, &I2C_InitStructure);

	I2C_Cmd(I2C_MEMS_BUS, ENABLE);
}


int I2C_write(I2C_TypeDef* I2Cx, uint8_t addr, uint8_t reg, uint8_t data)
{
	__IO uint32_t  Timeout = I2CTimeout;

	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
		if((Timeout--) == 0) return ERR_TIME_OUT;

	Timeout = I2CTimeout;

	I2C_GenerateSTART(I2Cx, ENABLE);

	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
		if((Timeout--) == 0) return ERR_TIME_OUT;

	Timeout = I2CTimeout;

	I2C_Send7bitAddress(I2Cx, addr, I2C_Direction_Transmitter);

	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		if((Timeout--) == 0) return ERR_TIME_OUT;

	Timeout = I2CTimeout;

	I2C_SendData(I2Cx, reg);

	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF) == RESET)
		if((Timeout--) == 0) return ERR_TIME_OUT;

	Timeout = I2CTimeout;

	I2C_SendData(I2Cx, data);

	while ((!I2C_GetFlagStatus(I2Cx,I2C_FLAG_TXE)) && (!I2C_GetFlagStatus(I2Cx,I2C_FLAG_BTF)))
		if((Timeout--) == 0) return ERR_TIME_OUT;

	I2C_GenerateSTOP(I2Cx, ENABLE);

	return E_OK;
}


int I2C_read(I2C_TypeDef* I2Cx, uint8_t addr, uint8_t reg, uint8_t* data)
{

	__IO uint32_t  Timeout = I2CTimeout;

	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
		if((Timeout--) == 0) return ERR_TIME_OUT;

	Timeout = I2CTimeout;

	I2C_GenerateSTART(I2Cx, ENABLE);

	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
		if((Timeout--) == 0) return ERR_TIME_OUT;

	Timeout = I2CTimeout;

	I2C_Send7bitAddress(I2Cx, addr, I2C_Direction_Transmitter);

	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		if((Timeout--) == 0) return ERR_TIME_OUT;

	Timeout = I2CTimeout;

	I2C_SendData(I2Cx, reg);

	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF) == RESET)
		if((Timeout--) == 0) return ERR_TIME_OUT;

	Timeout = I2CTimeout;

	I2C_GenerateSTART(I2Cx, ENABLE);

	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
		if((Timeout--) == 0) return ERR_TIME_OUT;

	Timeout = I2CTimeout;

	I2C_Send7bitAddress(I2Cx, addr, I2C_Direction_Receiver);

	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR) == RESET)
		if((Timeout--) == 0) return ERR_TIME_OUT;

	I2C_AcknowledgeConfig(I2Cx, DISABLE);

	(void)I2Cx->SR2;

	Timeout = I2CTimeout;

	I2C_GenerateSTOP(I2Cx, ENABLE);

	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE) == RESET)
		if((Timeout--) == 0) return ERR_TIME_OUT;

	*data = I2C_ReceiveData(I2Cx);

	Timeout = I2CTimeout;

	while(I2Cx->CR1 & I2C_CR1_STOP)
		Timeout = ((uint32_t)0x10000);

	I2C_AcknowledgeConfig(I2Cx, ENABLE);

	return E_OK;
}
