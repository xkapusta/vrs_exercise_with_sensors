/*
 * hts221.c
 *
 *  Created on: Nov 3, 2024
 *      Author: user
 */
#include "hts221.h"
#include "i2c.h"
#include "usart.h"
#include "stdint.h"
#include "math.h"

uint8_t usartMsgSensorSethts221[] = "hts221_set\n\r";

uint32_t data_read_set_HT=0;

uint8_t whoAmI_HT = 0;
uint8_t writeSettingsSensor_HT=0;

//Load
/*
 * H_out - two registers
 * H0_rH_x2 - H0_rH_x2/2=H0_rH
 * H1_rH_x2 - H1_rH_x2/2=H1_rH
 * H0_T0_OUT - two registers
 * H1_T1_OUT - two registers
 */
//Humidity
float humidity_value=0;
float humidity_value_1=0;

float humidityMultiplier=0.07;//0.07; // From AV_CONF - actual set to 128 avg

uint32_t H_OUT=0;

uint32_t H0_rH_x2=0;
uint32_t H1_rH_x2=0;
uint32_t H0_rH=0;
uint32_t H1_rH=0;
uint32_t H0_T0_OUT=0;
uint32_t H1_T0_OUT=0;

//Temperature
float temperature_value=0;
float temperature_value_1=0;

float temperatureMultiplier=0.01;//0.01; // From AV_CONF - actual set to 128 avg

uint32_t T_OUT=0;

uint32_t T0_degC_x8=0;
uint32_t T1_degC_x8=0;
uint32_t T0_degC=0;
uint32_t T1_degC=0;
uint32_t T0_OUT=0;
uint32_t T1_OUT=0;
uint32_t T1_T0_msb=0;

void hts221_Init(void){
	//Who am I [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]
	data_read_set_HT = (uint32_t)i2c_read(HTS221_DEVICE_ADDRESS_READ, HTS221_WHO_AM_I_ADDRES, 0);
	if(data_read_set_HT == HTS221_WHO_AM_I_VALUE){
		whoAmI_HT = 1;
	}
	//Who am I [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]***


	//Sensor settings [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]
	if(writeSettingsSensor_HT == 0){
		uint8_t settingsREG1=0x82;
		uint8_t settingsAV_CONF=0b00110101; //Both on 128 AVG --- H multiplier: 0.07 ----- T multiplier: 0.01
		i2c_write(HTS221_DEVICE_ADDRESS_WRITE, HTS221_CTRL_REG1, settingsREG1, 1);
		i2c_write(HTS221_DEVICE_ADDRESS_WRITE, HTS221_AV_CONF, settingsAV_CONF, 1);

		if((uint32_t)i2c_read(HTS221_DEVICE_ADDRESS_READ,HTS221_CTRL_REG1, 0) == settingsREG1 && (uint32_t)i2c_read(HTS221_DEVICE_ADDRESS_READ, HTS221_AV_CONF, 0) == settingsAV_CONF){
			writeSettingsSensor_HT=1;
		}
	}
	//Sensor settings [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]***


	//Check of setup [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]
	if(whoAmI_HT && writeSettingsSensor_HT){
		USART2_PutBuffer(usartMsgSensorSethts221, sizeof(usartMsgSensorSethts221));
		LL_mDelay(100);
	}
	//Check of setup [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]***
}

void hts221_Calibration(void){
	uint32_t extractor =0;
	uint32_t shift =0;
	//Humidity [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]
	H0_rH_x2=(uint32_t)i2c_read(HTS221_DEVICE_ADDRESS_READ, HTS221_H0_rH_x2, 0);
	H1_rH_x2=(uint32_t)i2c_read(HTS221_DEVICE_ADDRESS_READ, HTS221_H1_rH_x2, 0);

	H0_rH=H0_rH_x2/2;
	H1_rH=H1_rH_x2/2;

	H0_T0_OUT=(uint32_t)i2c_read(HTS221_DEVICE_ADDRESS_READ, HTS221_H0_T0_OUT, 2);
	H0_T0_OUT |= ((H0_T0_OUT >> 15) & 1) << 31;
	H1_T0_OUT=(uint32_t)i2c_read(HTS221_DEVICE_ADDRESS_READ, HTS221_H1_T0_OUT, 2);
	H1_T0_OUT |= ((H1_T0_OUT >> 15) & 1) << 31;
	//Humidity [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]***


	//Temperature [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]
	T0_degC_x8=(uint32_t)i2c_read(HTS221_DEVICE_ADDRESS_READ, HTS221_T0_degC_x8, 0);
	T1_degC_x8=(uint32_t)i2c_read(HTS221_DEVICE_ADDRESS_READ, HTS221_T1_degC_x8, 0);
	T1_T0_msb=(uint32_t)i2c_read(HTS221_DEVICE_ADDRESS_READ, HTS221_T1_T0_msb, 0);

	extractor = T1_T0_msb & 0x3;
	T0_degC_x8 &= ~(0x3 << 17); //clear
	shift = extractor << 17;

	T0_degC_x8 |= shift;

	extractor = T1_T0_msb & 0x12;
	T0_degC_x8 &= ~(0x3 << 17); //clear
	shift = extractor << 17;

	T1_degC_x8 |= shift;

	T0_degC=T0_degC_x8/8;
	T1_degC=T1_degC_x8/8;

	T0_OUT=(uint32_t)i2c_read(HTS221_DEVICE_ADDRESS_READ, HTS221_T0_OUT, 2);
	T0_OUT |= ((T0_OUT >> 15) & 1) << 31;
	T1_OUT=(uint32_t)i2c_read(HTS221_DEVICE_ADDRESS_READ, HTS221_T0_OUT, 2);
	T1_OUT |= ((T1_OUT >> 15) & 1) << 31;

	//Temperature [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]***
}

float humidityRead(void){
	humidity_value=0;

	H_OUT=(uint32_t)i2c_read(HTS221_DEVICE_ADDRESS_READ, HTS221_HUMIDITY_OUT_L, 2);
	H_OUT |= ((H_OUT >> 15) & 1) << 31;
	humidity_value=(float)(( ( (H1_rH - H0_rH) * (H_OUT - H0_T0_OUT) ) / (H1_T0_OUT - H0_T0_OUT) )+H0_rH);
	humidity_value_1=(float)((humidity_value*humidityMultiplier)/1000);

	return humidity_value_1;
}

float tempreatureRead(void){
	temperature_value=0;

	T_OUT=(uint32_t)i2c_read(HTS221_DEVICE_ADDRESS_READ, HTS221_TEMP_OUT_L, 2);
	T_OUT |= ((T_OUT >> 15) & 1) << 31;
	temperature_value=(float)((((T1_degC-T0_degC)*(T_OUT-T0_OUT))/(T1_OUT-T0_OUT))+T0_degC);
	temperature_value_1=(float)(temperature_value)*temperatureMultiplier;

	return temperature_value_1;
}


