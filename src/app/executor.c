#include <stdint.h>
#include "executor.h"
#include "../hal/uart.h"

void Executor_Init(void)
{
  Uart_Init();
}
