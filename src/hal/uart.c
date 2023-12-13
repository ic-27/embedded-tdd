#include <stdint.h>
#include "uart.h"

#if TEST
#include "bit_defs.h"
// workaround to define registers in ram for tests.
uint8_t UBRRH, UBRRL;
uint8_t UCSRA, UCSRB, UCSRC;
uint8_t UDR;
#else
#include <avr/io.h>
#endif

#define UBRR	51		// 9600 baud @8MHz, see datasheet

/**
 * Uart_Init() - Initialize UART
 * 
 * Return: void
 */
void Uart_Init(void)
{
  // set baud rate
  UBRRH	 = (uint8_t)(UBRR>>8);
  UBRRL	 = (uint8_t)(UBRR);

  // set frame format
  UCSRB |= (1<<RXEN)|(1<<TXEN);
  UCSRC |= (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
}

/**
 * Uart_Tx() - Send UART data
 *
 * Return: void
 */
void Uart_Tx(uint8_t data)
{
  while (!( UCSRA & (1<<UDRE)));
  UDR = data;
}

void Uart_Rx(void)
{
  
}
