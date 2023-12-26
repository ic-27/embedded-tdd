#include <stdio.h>
#include <stdint.h>
#include "executor.h"
#include "../hal/usart.h"
#include "../bsp/util.h"
#include "../hal/i2c.h"
#include "../driver/dht20.h"
#include "common.h"

/**
 * Executor_Init() - Initialize everything needed to get system running
 *
 * Return: void
 */
void Executor_Init(void)
{
    Util_Init();
    Usart_Init();
    I2C_Init();
}

/**
 * Executor_Exec() - Main loop to execute everything in 
 *
 * Return: void
 */
void Executor_Exec(void)
{
    _delay_ms(1000);
    uint8_t ret = DHT20_Read();
    printf("ret=%d temp=%ld humi=%ld\n", ret, DHT20_GetTemperature(), DHT20_GetHumidity());
}
