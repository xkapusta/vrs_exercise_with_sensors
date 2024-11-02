/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "i2c.h"
#include "usart.h"

/* USER CODE BEGIN 0 */
uint8_t data_recive=0;

uint8_t data_recive_8=0;
uint16_t data_recive_16=0;
uint32_t data_recive_32=0;

uint8_t data_recive_multy[3];
/* USER CODE END 0 */

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  LL_I2C_InitTypeDef I2C_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  /**I2C1 GPIO Configuration
  PB6   ------> I2C1_SCL
  PB7   ------> I2C1_SDA
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* I2C1 interrupt Init */
  NVIC_SetPriority(I2C1_EV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(I2C1_EV_IRQn);

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */

  /** I2C Initialization
  */
  LL_I2C_EnableAutoEndMode(I2C1);
  LL_I2C_DisableOwnAddress2(I2C1);
  LL_I2C_DisableGeneralCall(I2C1);
  LL_I2C_EnableClockStretching(I2C1);
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.Timing = 0x2000090E;
  I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  I2C_InitStruct.DigitalFilter = 0;
  I2C_InitStruct.OwnAddress1 = 2;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C1, &I2C_InitStruct);
  LL_I2C_SetOwnAddress2(I2C1, 0, LL_I2C_OWNADDRESS2_NOMASK);
  /* USER CODE BEGIN I2C1_Init 2 */
  LL_I2C_Enable(I2C1);
  /* USER CODE END I2C1_Init 2 */

}

uint32_t i2c_read(uint8_t slave_address, uint8_t register_address, uint8_t number_of_registers){
	data_recive=0;
	if(number_of_registers == 1){
		data_recive_8=0;
		register_address |= 0x80;
	}
	if(number_of_registers == 2){
		data_recive_16=0;
		register_address |= 0x80;
	}
	if(number_of_registers == 3){
		data_recive_32=0;
		register_address |= 0x80;
	}

	USART2_PutBuffer("0R", sizeof("0R"));

	LL_I2C_HandleTransfer(I2C1, slave_address, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);
	while (!LL_I2C_IsActiveFlag_TXIS(I2C1)) {}

	USART2_PutBuffer("1R", sizeof("1R"));

	LL_I2C_TransmitData8(I2C1, register_address);
	while (!LL_I2C_IsActiveFlag_TC(I2C1)) {}

	USART2_PutBuffer("2R", sizeof("2R"));

	if(number_of_registers <= 1){
		LL_I2C_HandleTransfer(I2C1, slave_address, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);
	}
	if(number_of_registers > 1){
		LL_I2C_HandleTransfer(I2C1, slave_address, LL_I2C_ADDRSLAVE_7BIT, number_of_registers, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_READ);
	}
	while (!LL_I2C_IsActiveFlag_RXNE(I2C1)) {}

	USART2_PutBuffer("3R", sizeof("3R"));

	if(number_of_registers > 1){
		USART2_PutBuffer("9R", sizeof("9R"));
		for(int i=0; i<number_of_registers;i++){
			data_recive_multy[i] = LL_I2C_ReceiveData8(I2C1);
			while (!LL_I2C_IsActiveFlag_STOP(I2C1)) {}
		}
	}else{
		data_recive = LL_I2C_ReceiveData8(I2C1);
		while (!LL_I2C_IsActiveFlag_STOP(I2C1)) {}
	}
	//LL_I2C_GenerateStopCondition(I2C1);
	LL_I2C_ClearFlag_STOP(I2C1);



	if(number_of_registers == 1){
		return data_recive_8;
	}
	if(number_of_registers == 2){
		data_recive_16 = (data_recive_multy[1] << 8) | data_recive_multy[0];
		return data_recive_16;
	}
	if(number_of_registers == 3){
		data_recive_32 = (data_recive_multy[2] << 16) | (data_recive_multy[1] << 8) | data_recive_multy[0];
		return data_recive_32;
	}
	if(number_of_registers == 0){
		return data_recive;
	}
}

uint8_t i2c_write(uint8_t slave_address, uint8_t register_address, uint8_t data, uint8_t number_of_registers){

	if(number_of_registers == 1){
		register_address |= 0x80;
	}
	if(number_of_registers == 2){
		register_address |= 0x80;
	}
	if(number_of_registers == 3){
		register_address |= 0x80;
	}



	USART2_PutBuffer("0W", sizeof("0W"));

	LL_I2C_HandleTransfer(I2C1, slave_address, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);
	while (!LL_I2C_IsActiveFlag_TXIS(I2C1)) {}

	USART2_PutBuffer("1W", sizeof("1W"));

	LL_I2C_TransmitData8(I2C1, register_address);
	while (!LL_I2C_IsActiveFlag_TC(I2C1)) {}

	USART2_PutBuffer("2W", sizeof("2W"));

	LL_I2C_TransmitData8(I2C1, data);

	//while (!LL_I2C_IsActiveFlag_TC(I2C1)) {}
	while (!LL_I2C_IsActiveFlag_TXE(I2C1)) {}

	USART2_PutBuffer("3W", sizeof("3W"));

	LL_I2C_ClearFlag_STOP(I2C1);

	return 0;
}




/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
