#include <stdint.h>
#include "main.h"
#include "executor.h"
#include "../bsp/util.h"
#if TEST
int MAIN(void)
#else
int main(void)
#endif
{
  Executor_Init();
  
  for (;;) {
    for (uint32_t i=0; i<50000; ++i);
    Util_Print("Hello World!");
  }
}
