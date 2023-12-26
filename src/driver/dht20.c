#include <stdio.h>
#include <stdint.h>
#include "dht20.h"
#include "../hal/i2c.h"
#include "common.h"

#define DHT20_TH_NUM_BYTES (6) // TH: temp/humi
#define DHT20_BUSY (1<<7) // status 

static int32_t temp, humi;

void DHT20_Init(void)
{
    temp = NO_SAMPLE_YET;
    humi = NO_SAMPLE_YET;
}

uint8_t DHT20_Read(void)
{
    uint8_t stat;
    int32_t tTemp, tHumi; // temp
    uint8_t i2cData[6];

    // send cmd to measure temp/humi
    if (I2C_Start()) goto DHT20_READ_FAIL;
    
    if (I2C_WriteAddr(DHT20_ADDR)) goto DHT20_READ_FAIL;
    if (I2C_WriteData(DHT20_TRIG_MEASURE_CMD_1)) goto DHT20_READ_FAIL;
    if (I2C_WriteData(DHT20_TRIG_MEASURE_CMD_2)) goto DHT20_READ_FAIL;
    if (I2C_WriteData(DHT20_TRIG_MEASURE_CMD_3)) goto DHT20_READ_FAIL;
    
    I2C_StopTransmission();
    
    _delay_ms(80); // see datasheet, requires 80ms wait
    
    // read data
    if (I2C_Start()) goto DHT20_READ_FAIL;
    if (I2C_ReadAddr(DHT20_ADDR)) goto DHT20_READ_FAIL;
    if (I2C_ReadData(&stat)) goto DHT20_READ_FAIL;
    if ((stat & DHT20_BUSY)) {
	goto DHT20_READ_FAIL;
    }
    for (uint8_t i=0; i<DHT20_TH_NUM_BYTES-1; ++i) {
	if (I2C_ReadData(&i2cData[i])) goto DHT20_READ_FAIL;
    }
    if (I2C_ReadDataLast(&i2cData[DHT20_TH_NUM_BYTES-1])) goto DHT20_READ_FAIL;
    I2C_StopTransmission();

    // calculate temp/humi from i2cData
    tTemp = calcTemperature(&i2cData[0]);
    tHumi = calcHumidity(&i2cData[0]);

    if (tTemp < MIN_TEMP_ADJ || tTemp > MAX_TEMP_ADJ ||
	tHumi < MIN_HUMI_ADJ || tHumi > MAX_HUMI_ADJ) {
	return DHT20_READ_OUT_OF_BOUND;
    }
    temp = tTemp;
    humi = tHumi;
    
    return DHT20_READ_SUCC;
    
 DHT20_READ_FAIL:
    I2C_StopTransmission();
    return DHT20_READ_FAIL;
}

int32_t DHT20_GetTemperature(void)
{
    return temp;
}

int32_t DHT20_GetHumidity(void)
{
    return humi;
}

int32_t calcTemperature(uint8_t *i2cData)
{
    int32_t temp;

    // this method only works on host gcc, not on avr-gcc?
    /* temp = (((uint32_t)(tempHumiData[2]&0xF)<<16) | */
    /* 	    (uint32_t)(tempHumiData[3]<<8) | (uint32_t)(tempHumiData[4])) */
    
    temp = (i2cData[2]&0x0F) << 8;
    temp = (temp+i2cData[3]) << 8;
    temp += i2cData[4];

    temp = (((float)temp/0x100000)*200.0 - 50.0) * 100.0;
    
    return temp;
}

int32_t calcHumidity(uint8_t *i2cData)
{
    int32_t humi;
    
    humi = (((uint32_t)i2cData[0]<<12) | (uint32_t)(i2cData[1]<<4) |
	    (uint32_t)(i2cData[2]&0xF0));
    humi = ((float)humi/0x100000)*100.0*100.0;
    
    return humi;
}
