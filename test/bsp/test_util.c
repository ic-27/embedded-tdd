#ifdef TEST

// This is a relatively simple function, and it is a function that is passed to
// printf. No need for extensive testing.

#include "unity.h"

#include "util.h"
#include "mock_usart.h"

void setUp(void)
{
}

void tearDown(void)
{
}

/* void test_Util_PutChar_Printf_should_CallUart_Tx_ExpectedNumberOfTimes_MinString(void) */
/* { */
/*   // setup */
/*   FILE *dummyStream; */
/*   char byte = 'a'; */

/*   // exercise */
/*   Usart_Tx_Expect(byte); */

/*   // verify */
/*   Util_PutChar_Printf(byte, dummyStream); */
/* } */

#endif // TEST
