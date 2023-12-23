#ifdef TEST

#include "unity.h"

#include "usart.h"

extern uint8_t UBRRH, UBRRL;
extern uint8_t UCSRA, UCSRB, UCSRC;
extern uint8_t UDR;

void setUp(void) 
{
  UBRRH = 0x0;
  UBRRL = 0x0;
  UCSRA = 0x20; // this is the correct initial value
  UCSRB = 0x0;
  UCSRC = 0x86; // this is the correct initial value

  UDR = 0;
}

void tearDown(void)
{
}

void test_Usart_Init_Should_InitializeCorrectRegisters(void)
{
  // setup
  
  // exercise
  Usart_Init();

  // verify
  TEST_ASSERT_EQUAL_HEX8(0x0,  UBRRH);
  TEST_ASSERT_EQUAL_HEX8(0x33,  UBRRL);
  TEST_ASSERT_EQUAL_HEX8(0x20, UCSRA);
  TEST_ASSERT_EQUAL_HEX8(0x18, UCSRB);
  TEST_ASSERT_EQUAL_HEX8(0x86, UCSRC);
}

void test_Usart_Tx_Should_PlacesDataIntoBufferOnSendWhenDataRegisterEmpty(void)
{
  // setup
  uint8_t data = 0xBF;
  Usart_Init();

  // exercise
  Usart_Tx(data);

  // verify
  TEST_ASSERT_EQUAL_HEX8(0xBF, UDR);
}

#endif // TEST
