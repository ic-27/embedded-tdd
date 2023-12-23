#include "usart.h"

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
void Usart_Init(void)
{
  // set baud rate
  UBRRH	 = (uint8_t)(UBRR>>8);
  UBRRL	 = (uint8_t)(UBRR);

  // set frame format
  UCSRB |= (1<<RXEN)|(1<<TXEN);
  UCSRC |= (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
}


/**
 * Usart_Tx() - Send data over USART
 *
 * Return: void
 */
void Usart_Tx(char data) {
  while (!(UCSRA & (1<<UDRE)));
  UDR = data;
}

