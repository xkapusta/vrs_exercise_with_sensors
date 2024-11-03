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

uint32_t data_read_set_HT=0;

uint8_t whoAmI_HT = 0;
uint8_t writeSettingsSensor_HT=0;

void hts221_Init(void){
	//Who am I [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]
	data_read_set_HT = (uint32_t)i2c_read(HTS221_DEVICE_ADDRESS_READ, HTS221_WHO_AM_I_ADDRES, 0);
	if(data_read_set_HT == HTS221_WHO_AM_I_VALUE){
		whoAmI_HT = 1;
	}
	//Who am I [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]***


	//Sensor settings [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]
	if(writeSettingsSensor_HT == 0){
		uint8_t settingsREG1=0xC0;
		uint8_t settingsRES=0x1;
		i2c_write(HTS221_DEVICE_ADDRESS_WRITE, HTS221_CTRL_REG1, settingsREG1, 1);
		i2c_write(HTS221_DEVICE_ADDRESS_WRITE, HTS221_AV_CONF, settingsRES, 1);

		if((uint32_t)i2c_read(HTS221_DEVICE_ADDRESS_READ,HTS221_CTRL_REG1, 0) == settingsREG1 && (uint32_t)i2c_read(HTS221_DEVICE_ADDRESS_READ, HTS221_AV_CONF, 0) == settingsRES){
			writeSettingsSensor_HT=1;
		}
	}
	//Sensor settings [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]***


	//Check of setup [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]
	if(whoAmI_HT && writeSettingsSensor_HT){
		USART2_PutBuffer(("lps25hb_set\n\r"), sizeof(("lps25hb_set\n\r")));
	}
	//Check of setup [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]***
}
