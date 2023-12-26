#ifdef TEST

#include "unity.h"

#include "dht20.h"
#include "mock_i2c.h"
#include "mock_common.h"

STATIC int32_t calcTemperature(uint8_t *i2cData);
STATIC int32_t calcHumidity(uint8_t *i2cData);

void setUp(void)
{
}

void tearDown(void)
{
}

void test_DHT20_Init_ShouldInitializeTempHumiValues(void)
{
    // setup
    
    // exercise
    DHT20_Init();

    // verify
    TEST_ASSERT_EQUAL_INT32(NO_SAMPLE_YET, DHT20_GetTemperature());
    TEST_ASSERT_EQUAL_INT32(NO_SAMPLE_YET, DHT20_GetHumidity());
}

void dht_read_helper(uint8_t stat, uint8_t *i2cData)
{
    I2C_Start_ExpectAndReturn(0);
    I2C_WriteAddr_ExpectAndReturn(DHT20_ADDR, 0);
    I2C_WriteData_ExpectAndReturn(DHT20_TRIG_MEASURE_CMD_1, 0);
    I2C_WriteData_ExpectAndReturn(DHT20_TRIG_MEASURE_CMD_2, 0);
    I2C_WriteData_ExpectAndReturn(DHT20_TRIG_MEASURE_CMD_3, 0);
    I2C_StopTransmission_Expect();

    I2C_Start_ExpectAndReturn(0);
    I2C_ReadAddr_ExpectAndReturn(DHT20_ADDR, 0);
    I2C_ReadData_ExpectAnyArgsAndReturn(0); // status
    I2C_ReadData_ReturnThruPtr_data(&stat); 
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[0]);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[1]);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[2]);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[3]);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[4]);
    I2C_ReadDataLast_ExpectAnyArgsAndReturn(0);
    I2C_ReadDataLast_ReturnThruPtr_data(&i2cData[5]);
    I2C_StopTransmission_Expect();    
}

void test_DHT20_Read_ShouldCall_I2CHAL_ToSampleTempHumi(void)
{
    // setup
    uint8_t stat = 0x00;
    uint8_t i2cData[6] = {130, 236, 53, 199, 176, 6}; // 22.25 C, 51.14% H
    DHT20_Init();

    // exercise
    I2C_Start_ExpectAndReturn(0);
    I2C_WriteAddr_ExpectAndReturn(DHT20_ADDR, 0);
    I2C_WriteData_ExpectAndReturn(DHT20_TRIG_MEASURE_CMD_1, 0);
    I2C_WriteData_ExpectAndReturn(DHT20_TRIG_MEASURE_CMD_2, 0);
    I2C_WriteData_ExpectAndReturn(DHT20_TRIG_MEASURE_CMD_3, 0);
    I2C_StopTransmission_Expect();

    _delay_ms_Expect(80);
    
    I2C_Start_ExpectAndReturn(0);
    I2C_ReadAddr_ExpectAndReturn(DHT20_ADDR, 0);
    I2C_ReadData_ExpectAnyArgsAndReturn(0); // status
    I2C_ReadData_ReturnThruPtr_data(&stat); 
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadDataLast_ExpectAnyArgsAndReturn(0);
    I2C_StopTransmission_Expect();        
    
    // verify
    DHT20_Read();
}

void test_DHT20_Read_ShouldStoreTempHumiValues(void)
{
    // setup
    uint8_t ret;
    uint8_t stat = 0x00;
    uint8_t i2cData[6] = {130, 236, 53, 199, 176, 6}; // 22.25 C, 51.14% H
    int32_t expectedTemp = 2225;
    int32_t expectedHumi = 5114;
    DHT20_Init();
    
    // exercise
    I2C_Start_ExpectAndReturn(0);
    I2C_WriteAddr_ExpectAndReturn(DHT20_ADDR, 0);
    I2C_WriteData_ExpectAndReturn(DHT20_TRIG_MEASURE_CMD_1, 0);
    I2C_WriteData_ExpectAndReturn(DHT20_TRIG_MEASURE_CMD_2, 0);
    I2C_WriteData_ExpectAndReturn(DHT20_TRIG_MEASURE_CMD_3, 0);
    I2C_StopTransmission_Expect();

    _delay_ms_Expect(80);
    
    I2C_Start_ExpectAndReturn(0);
    I2C_ReadAddr_ExpectAndReturn(DHT20_ADDR, 0);
    I2C_ReadData_ExpectAnyArgsAndReturn(0); // status
    I2C_ReadData_ReturnThruPtr_data(&stat); 
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[0]);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[1]);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[2]);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[3]);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[4]);
    I2C_ReadDataLast_ExpectAnyArgsAndReturn(0);
    I2C_ReadDataLast_ReturnThruPtr_data(&i2cData[5]);
    I2C_StopTransmission_Expect();
    
    ret = DHT20_Read();
    
    // verify
    TEST_ASSERT_EQUAL_UINT8(DHT20_READ_SUCC, ret);
    TEST_ASSERT_EQUAL_INT32(expectedTemp, DHT20_GetTemperature());
    TEST_ASSERT_EQUAL_INT32(expectedHumi, DHT20_GetHumidity());
}

void test_DHT20_Read_ShouldFailIfSensorBusy(void)
{
    // setup
    uint8_t ret;
    uint8_t stat = 0x80;
    uint8_t i2cData[6] = {130, 236, 53, 199, 176, 6}; // 22.25 C, 51.14% H
    
    int32_t expectedTemp = 2225;
    int32_t expectedHumi = 5114;
    DHT20_Init();
    
    // exercise
    I2C_Start_ExpectAndReturn(0);
    I2C_WriteAddr_ExpectAndReturn(DHT20_ADDR, 0);
    I2C_WriteData_ExpectAndReturn(DHT20_TRIG_MEASURE_CMD_1, 0);
    I2C_WriteData_ExpectAndReturn(DHT20_TRIG_MEASURE_CMD_2, 0);
    I2C_WriteData_ExpectAndReturn(DHT20_TRIG_MEASURE_CMD_3, 0);
    I2C_StopTransmission_Expect();

    _delay_ms_Expect(80);
    
    I2C_Start_ExpectAndReturn(0);
    I2C_ReadAddr_ExpectAndReturn(DHT20_ADDR, 0);
    I2C_ReadData_ExpectAnyArgsAndReturn(0); // status
    I2C_ReadData_ReturnThruPtr_data(&stat); 
    I2C_StopTransmission_Expect();    

    ret = DHT20_Read();
    
    // verify
    TEST_ASSERT_EQUAL_UINT8(DHT20_READ_FAIL, ret);
    TEST_ASSERT_EQUAL_INT32(NO_SAMPLE_YET, DHT20_GetTemperature());
    TEST_ASSERT_EQUAL_INT32(NO_SAMPLE_YET, DHT20_GetHumidity());
}

void test_DHT20_Read_ReturnErrorCodeIfI2CFailed(void)
{
    // setup
    uint8_t ret;
    
    DHT20_Init();
    
    // exercise
    I2C_Start_ExpectAndReturn(-1);
    I2C_StopTransmission_Expect();    

    ret = DHT20_Read();
    
    // verify
    TEST_ASSERT_EQUAL_UINT8(DHT20_READ_FAIL, ret);
    TEST_ASSERT_EQUAL_INT32(NO_SAMPLE_YET, DHT20_GetTemperature());
    TEST_ASSERT_EQUAL_INT32(NO_SAMPLE_YET, DHT20_GetHumidity());
}

void test_DHT20_Read_ReturnErrorCodeIfTemperatureOutofBounds_And_CheckTemperatureUnchanged(void)
{
    // setup
    uint8_t ret;
    uint8_t stat = 0x00;
    uint8_t i2cData[6] = {255, 255, 255, 199, 176, 6}; // 22.25 C, 51.14% H
    int32_t expectedTemp = 2225;
    int32_t expectedHumi = 5114;
    DHT20_Init();
    
    // exercise
    I2C_Start_ExpectAndReturn(0);
    I2C_WriteAddr_ExpectAndReturn(DHT20_ADDR, 0);
    I2C_WriteData_ExpectAndReturn(DHT20_TRIG_MEASURE_CMD_1, 0);
    I2C_WriteData_ExpectAndReturn(DHT20_TRIG_MEASURE_CMD_2, 0);
    I2C_WriteData_ExpectAndReturn(DHT20_TRIG_MEASURE_CMD_3, 0);
    I2C_StopTransmission_Expect();

    _delay_ms_Expect(80);
    
    I2C_Start_ExpectAndReturn(0);
    I2C_ReadAddr_ExpectAndReturn(DHT20_ADDR, 0);
    I2C_ReadData_ExpectAnyArgsAndReturn(0); // status
    I2C_ReadData_ReturnThruPtr_data(&stat); 
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[0]);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[1]);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[2]);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[3]);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[4]);
    I2C_ReadDataLast_ExpectAnyArgsAndReturn(0);
    I2C_ReadDataLast_ReturnThruPtr_data(&i2cData[5]);
    I2C_StopTransmission_Expect();    

    ret = DHT20_Read();
    
    // verify
    TEST_ASSERT_EQUAL_UINT8(DHT20_READ_OUT_OF_BOUND, ret);
    TEST_ASSERT_EQUAL_INT32(NO_SAMPLE_YET, DHT20_GetTemperature());
    TEST_ASSERT_EQUAL_INT32(NO_SAMPLE_YET, DHT20_GetHumidity());
}

void test_DHT20_GetTemperature_ShouldReturnNoSampleOnInit(void)
{
    // setup
    int32_t temp;
    
    DHT20_Init();
    
    // exercise
    temp = DHT20_GetTemperature();
    
    // verify
    TEST_ASSERT_EQUAL_INT32(NO_SAMPLE_YET, temp);
}

void test_DHT20_GetTemperature_ShouldReturnCorrectTempAfterSample(void)
{
    // setup
    uint8_t ret;
    uint8_t stat = 0x00;
    uint8_t i2cData[6] = {130, 236, 53, 199, 176, 6}; // 22.25 C, 51.14% H
    int32_t expectedTemp = 2225;
    int32_t expectedHumi = 5114;
    DHT20_Init();
    
    // exercise
    I2C_Start_ExpectAndReturn(0);
    I2C_WriteAddr_ExpectAndReturn(DHT20_ADDR, 0);
    I2C_WriteData_ExpectAndReturn(DHT20_TRIG_MEASURE_CMD_1, 0);
    I2C_WriteData_ExpectAndReturn(DHT20_TRIG_MEASURE_CMD_2, 0);
    I2C_WriteData_ExpectAndReturn(DHT20_TRIG_MEASURE_CMD_3, 0);
    I2C_StopTransmission_Expect();

    _delay_ms_Expect(80);
    
    I2C_Start_ExpectAndReturn(0);
    I2C_ReadAddr_ExpectAndReturn(DHT20_ADDR, 0);
    I2C_ReadData_ExpectAnyArgsAndReturn(0); // status
    I2C_ReadData_ReturnThruPtr_data(&stat); 
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[0]);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[1]);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[2]);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[3]);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[4]);
    I2C_ReadDataLast_ExpectAnyArgsAndReturn(0);
    I2C_ReadDataLast_ReturnThruPtr_data(&i2cData[5]);
    I2C_StopTransmission_Expect();
    
    ret = DHT20_Read();
    
    // verify
    TEST_ASSERT_EQUAL_UINT8(DHT20_READ_SUCC, ret);

    TEST_ASSERT_EQUAL_INT32(expectedTemp, DHT20_GetTemperature());
}

void test_DHT20_GetHumidity_ShouldReturnNoSampleOnInit(void)
{
    // setup
    int32_t humi;
    
    DHT20_Init();
    
    // exercise
    humi = DHT20_GetHumidity();
    
    // verify
    TEST_ASSERT_EQUAL_INT32(NO_SAMPLE_YET, humi);    
}

void test_DHT20_GetHumidity_ShouldReturnCorrectHumiAfterSample(void)
{
    // setup
    uint8_t ret;
    uint8_t stat = 0x00;
    uint8_t i2cData[6] = {130, 236, 53, 199, 176, 6}; // 22.25 C, 51.14% H
    int32_t expectedTemp = 2225;
    int32_t expectedHumi = 5114;
    DHT20_Init();
    
    // exercise
    I2C_Start_ExpectAndReturn(0);
    I2C_WriteAddr_ExpectAndReturn(DHT20_ADDR, 0);
    I2C_WriteData_ExpectAndReturn(DHT20_TRIG_MEASURE_CMD_1, 0);
    I2C_WriteData_ExpectAndReturn(DHT20_TRIG_MEASURE_CMD_2, 0);
    I2C_WriteData_ExpectAndReturn(DHT20_TRIG_MEASURE_CMD_3, 0);
    I2C_StopTransmission_Expect();

    _delay_ms_Expect(80);
    
    I2C_Start_ExpectAndReturn(0);
    I2C_ReadAddr_ExpectAndReturn(DHT20_ADDR, 0);
    I2C_ReadData_ExpectAnyArgsAndReturn(0); // status
    I2C_ReadData_ReturnThruPtr_data(&stat); 
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[0]);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[1]);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[2]);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[3]);
    I2C_ReadData_ExpectAnyArgsAndReturn(0);
    I2C_ReadData_ReturnThruPtr_data(&i2cData[4]);
    I2C_ReadDataLast_ExpectAnyArgsAndReturn(0);
    I2C_ReadDataLast_ReturnThruPtr_data(&i2cData[5]);
    I2C_StopTransmission_Expect();
    
    ret = DHT20_Read();
    
    // verify
    TEST_ASSERT_EQUAL_UINT8(DHT20_READ_SUCC, ret);

    TEST_ASSERT_EQUAL_INT32(expectedHumi, DHT20_GetHumidity());
}

void test_calcTemperature_shouldCorrectlyCalculateTemperature(void)
{
    // setup
    uint8_t i2cData[6] = {130, 236, 53, 199, 176, 6}; // 22.25 C, 51.14% H
    int32_t temp;
    
    // exercise
    temp = calcTemperature(&i2cData[0]);
    
    // verify
    TEST_ASSERT_EQUAL_INT32(2225, temp);
	  
}

void test_calcHumidity_shouldCorrectlyCalculateHumidity(void)
{
    // setup
    uint8_t i2cData[6] = {130, 236, 53, 199, 176, 6}; // 22.25 C, 51.14% H
    int32_t humi;

    // exercise
    humi = calcHumidity(&i2cData[0]);
    
    // verify
    TEST_ASSERT_EQUAL_INT32(5114, humi);
    
}

#endif // TEST
