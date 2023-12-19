#ifdef TEST

#include "unity.h"

#include "util.h"
#include "mock_uart.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_Util_Print_should_CallUart_Tx_ExpectedNumberOfTimes_MinString(void)
{
  // setup
  char* str = "a";

  // exercise
  char *i = str;
  while (*i) {
    Uart_Tx_Expect(*i++);
  }
  Uart_Tx_Expect('\r');
  Uart_Tx_Expect('\n');

  // verify
  Util_Print(str);
}

void test_Util_Print_should_CallUart_Tx_ExpectedNumberOfTimes_LargerString(void)
{
  // setup
  char* str = "abcdefghijklmnopqrstuvwxyz";

  // exercise
  char *i = str;
  while (*i) {
    Uart_Tx_Expect(*i++);
  }
  Uart_Tx_Expect('\r');
  Uart_Tx_Expect('\n');

  // verify
  Util_Print(str);
}

void test_Util_Print_should_WorkForNumbersAndSigns(void)
{
  // setup
  char* str = "01234567890[](){}<>=+ ~x";

  // exercise
  char *i = str;
  while (*i) {
    Uart_Tx_Expect(*i++);
  }
  Uart_Tx_Expect('\r');
  Uart_Tx_Expect('\n');

  // verify
  Util_Print(str);
}

void test_Util_Print_should_ImmediatelyReturnForNullStrings(void)
{
  // setup
  char* str = NULL;

  // exercise

  // verify
  Util_Print(str);
}

#endif // TEST
