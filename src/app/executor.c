#include <stdint.h>
#include "executor.h"
#include "../hal/usart.h"
#include "../bsp/util.h"

void Executor_Init(void)
{
  Util_Init();
  Usart_Init();
}
