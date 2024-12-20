/*
 * lps25hb.c


 */
#include "lps25hb.h"
#include "i2c.h"
#include "usart.h"
#include "stdint.h"
#include "math.h"

uint8_t usartMsgSensorSetlps25hb[] = "lps25hb_set\n\r";

uint32_t data_read_set=0;

uint8_t whoAmI = 0;
uint8_t writeSettingsSensor=0;

uint32_t pressureData=0;
float pressureDataF=0;

uint32_t pressureOffset=0;

uint32_t height =0;

uint32_t (*readCallback)(uint8_t, uint8_t, uint8_t) = NULL;
uint32_t (*writeCallback)(uint8_t, uint8_t, uint8_t, uint8_t) = NULL;

void lps25hb_ReadCallback(uint32_t (*callback)(uint8_t, uint8_t, uint8_t)){
	if(callback != 0){
		readCallback = callback;
	}
}
void lps25hb_WriteCallback(uint32_t (*callback)(uint8_t, uint8_t, uint8_t, uint8_t)){
	if(callback != 0){
		writeCallback = callback;
	}
}


void lps25hb_Init(void){
	//Who am I [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]
	data_read_set = (uint32_t)i2c_read(LPS25HB_DEVICE_ADDRESS_READ_1, LPS25HB_WHO_AM_I_ADDRES, 0);
	if(data_read_set == LPS25HB_WHO_AM_I_VALUE){
		whoAmI = 1;
	}
	//Who am I [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]***


	//Sensor settings [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]
	if(writeSettingsSensor == 0){
		uint8_t settingsREG1=0xC0;
		uint8_t settingsREG2=0x40;
		uint8_t settingsRES=0x1;
		uint8_t settingsFIFO_CTRL=0xC7;

		i2c_write(LPS25HB_DEVICE_ADDRESS_WRITE_1, LPS25HB_CTRL_REG1, settingsREG1, 1);
		i2c_write(LPS25HB_DEVICE_ADDRESS_WRITE_1, LPS25HB_CTRL_REG2, settingsREG2, 1);
		i2c_write(LPS25HB_DEVICE_ADDRESS_WRITE_1, LPS25HB_RES_CONF, settingsRES, 1);
		i2c_write(LPS25HB_DEVICE_ADDRESS_WRITE_1, LPS25HB_FIFO_CTRL, settingsFIFO_CTRL, 1);

		if((uint32_t)i2c_read(LPS25HB_DEVICE_ADDRESS_READ_1, LPS25HB_CTRL_REG1, 0) == settingsREG1 && (uint32_t)i2c_read(LPS25HB_DEVICE_ADDRESS_READ_1, LPS25HB_RES_CONF, 0) == settingsRES){
			writeSettingsSensor=1;
		}
	}
	//Sensor settings [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]***


	//Offset read value [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]
	pressureOffset=(uint32_t)i2c_read(LPS25HB_DEVICE_ADDRESS_READ_1, LPS25HB_RPDS_L, 2);

	//Offset read value [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]***


	//Check of setup [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]
	if(whoAmI && writeSettingsSensor){
		USART2_PutBuffer(usartMsgSensorSetlps25hb, sizeof(usartMsgSensorSetlps25hb));
		LL_mDelay(100);
	}
	//Check of setup [_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_][_]***
}

float pressureRead(void){
	pressureData=0;
	pressureData=(uint32_t)i2c_read(LPS25HB_DEVICE_ADDRESS_READ_1, LPS25HB_PRESS_OUT_XL, 3);
	pressureDataF=0;
	pressureDataF=(float)(pressureData+pressureOffset)/4096;
	//pressureData=~pressureOffset+1;
	return pressureDataF;
}

double heightCalculation(float basePressure, float currentPressure, float temp){
	return ( ( (double)287.05 * (double)temp) / (double)9.80665 )*(log(basePressure/currentPressure));
}


