#ifdef TEST

#include "unity.h"

#include "i2c.h"
#include "bit_defs.h"
#include "twi_defs.h"

extern uint8_t TWBR, TWCR, TWDR, TWSR;

void setUp(void)
{
    TWBR = 0x0;
    TWCR = 0x0;
    TWDR = 0xFF; // this is the correct initial value
    TWSR = 0xF8; // this is the correct initial value
}

void tearDown(void)
{
}

void test_I2C_Init_CorrectlyInitializesRegisters(void)
{
    // setup

    // exercise
    I2C_Init();
    
    // verify
    TEST_ASSERT_EQUAL_HEX8(0x8, TWBR);
}

void test_I2C_Start_InitiatesTransmission_And_CorrectlyReceivesAck(void)
{
    // setup
    uint8_t ret = 0;
    I2C_Init();

    TWSR = TW_START;
    
    // exercise
    ret = I2C_Start();
    
    // verify
    TEST_ASSERT_EQUAL_HEX8(0, ret);
}

void test_I2C_Start_InitiatesTransmission_And_CorrectlyHandlesNack(void)
{
    // setup
    uint8_t ret = 0;
    I2C_Init();

    TWSR = TW_BUS_ERROR;
    
    // exercise
    ret = I2C_Start();
    
    // verify
    TEST_ASSERT_EQUAL_HEX8(-1, ret);
}

void test_I2C_WriteAddr_CorrectlySendsSevenBitAddress(void)
{
    // setup
    uint8_t dummyWriteAddress = 0x38;
    uint8_t ret = 0;
    I2C_Init();
    
    TWSR = TW_START;
    ret = I2C_Start();
    TEST_ASSERT_EQUAL_HEX8(0, ret);

    TWSR = TW_MT_SLA_ACK;
    
    // exercise
    ret = I2C_WriteAddr(dummyWriteAddress);
    
    // verify
    TEST_ASSERT_EQUAL_HEX8(dummyWriteAddress<<1, TWDR);
}

void test_I2C_WriteAddr_Correctly_HandlesAck(void)
{
    // setup
    uint8_t dummyWriteAddress = 0x38;
    uint8_t ret = 0;
    I2C_Init();
    
    TWSR = TW_START;
    ret = I2C_Start();
    TEST_ASSERT_EQUAL_HEX8(0, ret);

    TWSR = TW_MT_SLA_ACK;
    
    // exercise
    ret = I2C_WriteAddr(dummyWriteAddress);
    
    // verify
    TEST_ASSERT_EQUAL_HEX8(0, ret);
}

void test_I2C_WriteAddr_Correctly_HandlesNack(void)
{
    // setup
    uint8_t dummyWriteAddress = 0x38;
    uint8_t ret = 0;
    I2C_Init();
    
    TWSR = TW_START;
    ret = I2C_Start();
    TEST_ASSERT_EQUAL_HEX8(0, ret);

    TWSR = TW_BUS_ERROR;
    
    // exercise
    ret = I2C_WriteAddr(dummyWriteAddress);
    
    // verify
    TEST_ASSERT_EQUAL_HEX8(-1, ret);
}

void test_I2C_WriteData_Correctly_HandlesAck(void)
{
    // setup
    uint8_t dummyWriteAddress = 0x38;
    uint8_t ret = 0;
    I2C_Init();
    
    TWSR = TW_START;
    ret = I2C_Start();
    TEST_ASSERT_EQUAL_HEX8(0, ret);

    TWSR = TW_MT_DATA_ACK;
    
    // exercise
    ret = I2C_WriteData(dummyWriteAddress);
    
    // verify
    TEST_ASSERT_EQUAL_HEX8(0, ret);
}


void test_I2C_WriteData_Correctly_HandlesNack(void)
{
    // setup
    uint8_t dummyWriteAddress = 0x38;
    uint8_t ret = 0;
    I2C_Init();
    
    TWSR = TW_START;
    ret = I2C_Start();
    TEST_ASSERT_EQUAL_HEX8(0, ret);

    TWSR = TW_BUS_ERROR;
    
    // exercise
    ret = I2C_WriteData(dummyWriteAddress);
    
    // verify
    TEST_ASSERT_EQUAL_HEX8(-1, ret);
}

void test_I2C_ReadAddr_SuccessfulyTransmitsAddr_and_HandlesAck(void)
{
    // setup
    uint8_t dummyWriteAddress = 0x38;
    uint8_t ret = 0;
    I2C_Init();
    
    TWSR = TW_START;
    ret = I2C_Start();
    TEST_ASSERT_EQUAL_HEX8(0, ret);

    TWSR = TW_MR_SLA_ACK;
    
    // exercise
    ret = I2C_ReadAddr(dummyWriteAddress);
    
    // verify
    TEST_ASSERT_EQUAL_HEX8(0, ret);
}

void test_I2C_ReadAddr_HandlesNack(void)
{
    // setup
    uint8_t dummyWriteAddress = 0x38;
    uint8_t ret = 0;
    I2C_Init();
    
    TWSR = TW_START;
    ret = I2C_Start();
    TEST_ASSERT_EQUAL_HEX8(0, ret);

    TWSR = TW_BUS_ERROR;
    
    // exercise
    ret = I2C_ReadAddr(dummyWriteAddress);
    
    // verify
    TEST_ASSERT_EQUAL_HEX8(-1, ret);
}

void test_I2C_ReadAddr_CorrectlySendsSevenBitAddress(void)
{
    // setup
    uint8_t dummyWriteAddress = 0x38;
    uint8_t ret = 0;
    I2C_Init();
    
    TWSR = TW_START;
    ret = I2C_Start();
    TEST_ASSERT_EQUAL_HEX8(0, ret);

    TWSR = TW_MT_SLA_ACK;
    
    // exercise
    ret = I2C_ReadAddr(dummyWriteAddress);
    
    // verify
    TEST_ASSERT_EQUAL_HEX8((dummyWriteAddress<<1) | TW_READ, TWDR);
}

void test_I2C_ReadData_HandlesAckAndReadsData(void)
{
    // setup
    uint8_t ret = 0;
    uint8_t data;
    uint8_t dummyData = 0xAE;
    I2C_Init();

    TWSR = TW_START;
    ret = I2C_Start();
    TEST_ASSERT_EQUAL_HEX8(0, ret);

    TWSR = TW_MR_DATA_ACK;
    TWDR = dummyData;

    // exercise
    ret = I2C_ReadData(&data);
    
    // verify
    TEST_ASSERT_EQUAL_HEX8(0, ret);
    TEST_ASSERT_EQUAL_HEX8(dummyData, data);
}

void test_I2C_ReadData_HandlesNack(void)
{
    // setup
    uint8_t ret = 0;
    uint8_t data = 0xEA;
    uint8_t dummyData = 0xAE;
    I2C_Init();

    TWSR = TW_START;
    ret = I2C_Start();
    TEST_ASSERT_EQUAL_HEX8(0, ret);

    TWSR = TW_BUS_ERROR;
    TWDR = dummyData;

    // exercise
    ret = I2C_ReadData(&data);
    
    // verify
    TEST_ASSERT_EQUAL_HEX8(-1, ret);
    TEST_ASSERT_EQUAL_HEX8(0xEA, data);// make sure data unchanged on nack
}

void test_I2C_StopTransmission_WritesToCorrectRegisters(void)
{
    // setup
    
    // exercise
    I2C_StopTransmission();
    
    // verify
    TEST_ASSERT_EQUAL_HEX8((1<<TWINT)|(1<<TWEN)|(1<<TWSTO), TWCR);
}

#endif // TEST
