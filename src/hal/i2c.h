#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void I2C_Init(void);
uint8_t I2C_Start(void);
uint8_t I2C_WriteAddr(uint8_t writeAddr);
uint8_t I2C_WriteData(uint8_t data);
uint8_t I2C_ReadAddr(uint8_t readAddr);
uint8_t I2C_ReadData(uint8_t *data);
void I2C_StopTransmission(void);

uint8_t I2C_ReadDataLast(uint8_t *data);
#endif // I2C_H
