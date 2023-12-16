#ifndef UART_H
#define UART_H

#include <stdint.h>

void Uart_Init(void);
void Uart_Tx(uint8_t data);
void Uart_Rx(void);

#endif // UART_H
