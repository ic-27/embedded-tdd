#ifndef DHT20_H
#define DHT20_H

#include <stdint.h>

// dht20 addr & commands
#define DHT20_ADDR (0x38)
#define DHT20_TRIG_MEASURE_CMD_1 (0xAC)
#define DHT20_TRIG_MEASURE_CMD_2 (0x33)
#define DHT20_TRIG_MEASURE_CMD_3 (0x00)

// bounds
#define MIN_TEMP (-40) // celsius
#define MAX_TEMP (80)  // celsius

#define MIN_HUMI (0) 
#define MAX_HUMI (100)

#define MIN_TEMP_ADJ (MIN_TEMP*100)
#define MAX_TEMP_ADJ (MAX_TEMP*100)

#define MIN_HUMI_ADJ (MIN_HUMI*100)
#define MAX_HUMI_ADJ (MAX_HUMI*100)


// Temp can span into negatives, so set error code beyond bounds
#define NO_SAMPLE_YET ((int32_t)(-100))

typedef enum {
    DHT20_READ_SUCC=0,
    DHT20_READ_FAIL,
    DHT20_READ_OUT_OF_BOUND,
} DHT20_Read_ErrCodes_t;
/* #define DHT20_READ_SUCCESS_CODE (0) */
/* #define DHT20_READ_FAIL_CODE ((uint8_t)-1) */

void DHT20_Init(void);
uint8_t DHT20_Read(void);
int32_t DHT20_GetTemperature(void);
int32_t DHT20_GetHumidity(void);

#endif // DHT20_H
