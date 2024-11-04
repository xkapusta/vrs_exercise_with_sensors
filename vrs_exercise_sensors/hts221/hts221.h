/*
 * hts221.h
 *
 *  Created on: Nov 3, 2024
 *      Author: user
 */

#ifndef HTS221_H_
#define HTS221_H_

#define		HTS221_DEVICE_ADDRESS		0
#define		HTS221_WHO_AM_I_ADDRES		0x0F
#define 	HTS221_WHO_AM_I_VALUE		0xBC

#define 	HTS221_DEVICE_ADDRESS_READ		0xBF
#define 	HTS221_DEVICE_ADDRESS_WRITE		0xBE

#define 	HTS221_AV_CONF			0x10

#define 	HTS221_CTRL_REG1		0x20
#define 	HTS221_CTRL_REG2		0x21
#define 	HTS221_CTRL_REG3		0x22

#define 	HTS221_HUMIDITY_OUT_L	0x28
#define 	HTS221_HUMIDITY_OUT_H	0x29

#define 	HTS221_TEMP_OUT_L		0x2A
#define 	HTS221_TEMP_OUT_H		0x2B

#define 	HTS221_H0_rH_x2			0x30
#define 	HTS221_H1_rH_x2			0x31

#define 	HTS221_T1_T0_msb		0x35

#define 	HTS221_H0_T0_OUT		0x36 // Dis is 36 and 37
#define 	HTS221_H1_T0_OUT		0x3A // Double 8

#define 	HTS221_T0_degC_x8		0x32
#define 	HTS221_T1_degC_x8		0x33

#define 	HTS221_T0_OUT			0x3C // Double 8
#define 	HTS221_T1_OUT			0x3E // Double 8


void hts221_Init(void);
float humidityRead(void);
float tempreatureRead(void);


#endif /* HTS221_H_ */
