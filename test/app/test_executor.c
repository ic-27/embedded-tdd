#ifdef TEST

#include "unity.h"

#include "executor.h"
#include "mock_uart.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_Executor_Init_Should_InitializeModules(void)
{
  // setup

  // exercise
  Uart_Init_Expect();

  // verify
  Executor_Init();
}

#endif // TEST
