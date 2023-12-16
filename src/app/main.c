#include "main.h"
#include "executor.h"
#include "../hal/uart.h"
#if TEST
int MAIN(void)
#else
int main(void)
#endif
{
  Executor_Init();
  
  for (;;) {
    for (uint32_t i=0; i<50000; ++i);
    Uart_Tx('a');
    Uart_Tx('\r');
    Uart_Tx('\n');
  }
}
