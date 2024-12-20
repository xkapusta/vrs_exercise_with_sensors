/*
 * lps25hb.h
 *
 *  Created on: Nov 2, 2024
 *      Author: user
 */

#ifndef LPS25HB_H_
#define LPS25HB_H_

#include "main.h"
//CTRL_REG1
#define		LPS25HB_DEVICE_ADDRESS		0b101110
#define 	LPS25HB_WHO_AM_I_ADDRES		0x0FU
#define 	LPS25HB_WHO_AM_I_VALUE		0b10111101

#define 	LPS25HB_DEVICE_ADDRESS_READ_0		0xB9
#define 	LPS25HB_DEVICE_ADDRESS_WRITE_0		0xB8
#define 	LPS25HB_DEVICE_ADDRESS_READ_1		0xBB
#define 	LPS25HB_DEVICE_ADDRESS_WRITE_1		0xBA

#define 	LPS25HB_RES_CONF				0x10

#define 	LPS25HB_CTRL_REG1				0x20
#define 	LPS25HB_CTRL_REG2				0x21
#define 	LPS25HB_CTRL_REG3				0x22

#define 	LPS25HB_PRESS_OUT_XL			0x28
#define 	LPS25HB_PRESS_OUT_L				0x29
#define 	LPS25HB_PRESS_OUT_H				0x2A

#define 	LPS25HB_RPDS_L					0x39
#define 	LPS25HB_RPDS_H					0x3A

#define 	LPS25HB_FIFO_CTRL				0x2E


void lps25hb_Init(void);
float pressureRead(void);
double heightCalculation(float basePressure, float currentPressure, float temp);
void lps25hb_WriteCallback(uint32_t (*callback)(uint8_t, uint8_t, uint8_t, uint8_t));
void lps25hb_ReadCallback(uint32_t (*callback)(uint8_t, uint8_t, uint8_t));

#endif /* LPS25HB_H_ */
