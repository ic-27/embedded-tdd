#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "main.h"
#include "executor.h"
#include "../bsp/util.h"

#define F_CPU 8000000
#include <util/delay.h>

#if TEST
int MAIN(void)
#else
    int main(void)
#endif
{
    Executor_Init();

    /* // https://cdn.sparkfun.com/assets/8/a/1/5/0/DHT20.pdf */
    _delay_ms(100); // 100 ms stabilization time for DHT20 sensor (see datasheet)
  
    TWBR = 0x8;
  
    // start
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    if ((TWSR & 0xF8) != 0x08) {
	return 0;
    }

    // send addr+w
    TWDR = (0x38) << 1; // only 7 bits!
    TWCR = (1<<TWINT) | (1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    if ((TWSR & 0xF8) != 0x18) {
	return 0;
    }

    // request data (trigger measurement)
    TWDR = (0xAC);
    TWCR = (1<<TWINT) | (1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    if ((TWSR & 0xF8) != 0x28) {
	return 0;
    }
    TWDR = (0x33);
    TWCR = (1<<TWINT) | (1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    if ((TWSR & 0xF8) != 0x28) {
	return 0;
    }
    TWDR = (0x00);
    TWCR = (1<<TWINT) | (1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    if ((TWSR & 0xF8) != 0x28) {
	return 0;
    }
  
    // stop
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
  
    _delay_ms(80);

    // start
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    if ((TWSR & 0xF8) != 0x08) {
	return 0;
    }
  
    // send addr+r
    TWDR = ((0x38) << 1) | 0x1; // only 7 bits!
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    if ((TWSR & 0xF8) != 0x40) {
	return 0;
    }

    // get status
    TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    if ((TWSR & 0xF8) != 0x50) {
    }
    uint8_t status = TWDR;
    if (!(status & (1<<7))) {
    }

    // get temp/humi data
    uint8_t tempHumiData[6] = {0};
    uint32_t humi;
    uint32_t temp;

    for (uint8_t i=0; i<6; ++i) {
	TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != 0x50) {
	}
	tempHumiData[i] = TWDR;
    }
  
    humi = ((uint32_t)tempHumiData[0]<<12) | (uint32_t)(tempHumiData[1]<<4) | (uint32_t)(tempHumiData[2]&0xF0);
    humi = ((float)humi/0x100000)*100.0;

    // calc method 1: only works on host computer???
    //temp = (((uint32_t)(tempHumiData[2]&0xF)<<16) | (uint32_t)(tempHumiData[3]<<8) | (uint32_t)(tempHumiData[4])) 

    // calc method 2: works on both host gcc and avr-gcc
    temp = (tempHumiData[2]&0x0F) << 8;
    temp = (temp+tempHumiData[3]) << 8;
    temp += tempHumiData[4];
  
    temp = ((float)temp/0x100000)*200.0 - 50.0;

    printf("%ld %ld\n", temp, humi);
    
    // stop
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
    
    while (1) {
	
    }
}


