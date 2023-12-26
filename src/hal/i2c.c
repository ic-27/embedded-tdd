#include "i2c.h"
#include <stdint.h>

#define I2C_BIT_RATE (0x8) // 100 kHz @8000000

// workaround to define registers in ram for tests.
#if TEST
#include "bit_defs.h"
#include "twi_defs.h"

uint8_t TWBR, TWCR, TWDR, TWSR;
#else
#include <avr/io.h>
#include <util/twi.h>
#endif

// get rid of infinite loop for ease of testing
#if TEST
#define I2C_CHK_JOB_DONE() (0)
#else
#define I2C_CHK_JOB_DONE() (!(TWCR & (1<<TWINT)))
#endif

/**
 * I2C_Init() - Initialize I2C
 *
 * Return: void
 */
void I2C_Init(void)
{
    TWBR = I2C_BIT_RATE;
}

/**
 * I2C_Start() - Send I2C start transmission
 *
 * Return: 0 on success, else -1
 */
uint8_t I2C_Start(void)
{
    // start transmission
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while (I2C_CHK_JOB_DONE());
    if ((TWSR & 0xF8) != TW_START) {
	return -1;
    }
    return 0;
}

/**
 * I2C_WriteAddr() - Send an address with write bit
 *
 * Return: 0 on success, else -1
 */
uint8_t I2C_WriteAddr(uint8_t writeAddr)
{
    // send addr+w/rw
    uint8_t tempWriteAddr = writeAddr;
    tempWriteAddr <<= 1;
    tempWriteAddr &= ~(0x1);
    TWDR = tempWriteAddr;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while (I2C_CHK_JOB_DONE());
    if ((TWSR & 0xF8) != TW_MT_SLA_ACK) {
	return -1;
    }
    return 0;
}

/**
 * I2C_WriteData() - Write data to I2C slave
 *
 * Return: 0 on success, else -1
 */
uint8_t I2C_WriteData(uint8_t data)
{
    // send data
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while (I2C_CHK_JOB_DONE());
    if ((TWSR & 0xF8) != TW_MT_DATA_ACK) {
	return -1;
    }
    return 0;
}

/**
 * I2C_ReadAddr() - Send address with read bit
 *
 * Return: 0 on success, else -1
 */
uint8_t I2C_ReadAddr(uint8_t readAddr) 
{
    TWDR = (readAddr << 1) | TW_READ; // only 7 bits!
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (I2C_CHK_JOB_DONE());
    if ((TWSR & 0xF8) != TW_MR_SLA_ACK) {
	return -1;
    }
    return 0;
}

/**
 * I2C_ReadData() - Read data from I2C slave
 * @data: pointer to var to read data into
 *
 * Return: 0 on success, else -1
 */
uint8_t I2C_ReadData(uint8_t *data) 
{
    TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
    while (I2C_CHK_JOB_DONE());
    if ((TWSR & 0xF8) != TW_MR_DATA_ACK) {
	return -1;
    }
    *data = TWDR;
    return 0;
}

/**
 * I2C_ReadDataLast() - Read last byte from I2C slave
 * @data: pointer to var to read data into
 *
 * Return: 0 on success, else -1
 */
uint8_t I2C_ReadDataLast(uint8_t *data) 
{
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (I2C_CHK_JOB_DONE());
    if ((TWSR & 0xF8) != TW_MR_DATA_NACK) {
	return -1;
    }
    *data = TWDR;
    return 0;
}


/**
 * I2C_StopTransmission() - Send a stop transmission
 *
 * Return: void
 */
void I2C_StopTransmission(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
    while(!(TWCR & (1<<TWSTO)));
}
