#ifdef TEST

#include "unity.h"

#include "executor.h"
#include "mock_util.h"
#include "mock_usart.h"
#include "mock_i2c.h"
#include "mock_dht20.h"
#include "mock_common.h"

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
    Util_Init_Expect();
    Usart_Init_Expect();
    I2C_Init_Expect();
    
    // verify
    Executor_Init();
}

#endif // TEST
