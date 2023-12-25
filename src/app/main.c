#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "main.h"
#include "executor.h"
#include "../bsp/util.h"
#include "../hal/i2c.h"

#define F_CPU 8000000
#include <util/delay.h>

#if TEST
#define MAIN TEST_MAIN
#else
#define MAIN main
#endif
int MAIN(void)
{
    Executor_Init();

    /* // https://cdn.sparkfun.com/assets/8/a/1/5/0/DHT20.pdf */
    _delay_ms(100); // 100 ms stabilization time for DHT20 sensor (see datasheet)

    I2C_Init();
    
    I2C_Start();
    I2C_WriteAddr(0x38);
    I2C_WriteData(0xAC);
    I2C_WriteData(0x33);
    I2C_WriteData(0x00);
    I2C_StopTransmission();
  
    _delay_ms(80);

    I2C_Start();
    I2C_ReadAddr(0x38);

    // get status
    uint8_t status;
    I2C_ReadData(&status);
    if (!(status & (1<<7))) {
    }

    // get temp/humi data
    uint8_t tempHumiData[6] = {0};
    for (uint8_t i=0; i<6; ++i) {
	I2C_ReadData(&tempHumiData[i]);
    }

    uint32_t temp, humi;
    humi = ((uint32_t)tempHumiData[0]<<12) | (uint32_t)(tempHumiData[1]<<4) | (uint32_t)(tempHumiData[2]&0xF0);
    humi = ((float)humi/0x100000)*100.0*100.0;

    // calc method 1: only works on host computer???
    //temp = (((uint32_t)(tempHumiData[2]&0xF)<<16) | (uint32_t)(tempHumiData[3]<<8) | (uint32_t)(tempHumiData[4]))
    
    // calc method 2: works on both host gcc and avr-gcc
    temp = (tempHumiData[2]&0x0F) << 8;
    temp = (temp+tempHumiData[3]) << 8;
    temp += tempHumiData[4];
  
    temp = (((float)temp/0x100000)*200.0 - 50.0)*100.0;

    printf("%ld %ld\n", temp, humi);
    
    // stop
    I2C_StopTransmission();
    
    while (1) {
	
    }
}


