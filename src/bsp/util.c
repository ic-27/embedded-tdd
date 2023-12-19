#include "util.h"
#include "../hal/uart.h"

void Util_Print(char *str)
{
  if (!str) {
    return;
  }
  
  while (*str) {
    Uart_Tx(*str++);
  }
  Uart_Tx('\r');
  Uart_Tx('\n');
}
