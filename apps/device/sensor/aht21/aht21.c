#include "aht21.h"

#if 0
static const uint8_t AHT_21_ADDR = 0x38 << 1;
static const uint32_t i2c_RETRY_TIME = 100;

HAL_StatusTypeDef AHT21_init(void)
{
    HAL_StatusTypeDef ret;
    uint8_t buff[8];

    ret = HAL_I2C_IsDeviceReady(&AHT21_I2C_PORT, AHT_21_ADDR, 2, i2c_RETRY_TIME); // is device avaialble ?
    if (ret != HAL_OK) {
        return HAL_ERROR;
    }
    buff[0] = 0x71;
    ret = HAL_I2C_Master_Transmit(&AHT21_I2C_PORT, AHT_21_ADDR, buff, 1, i2c_RETRY_TIME);
    if (ret != HAL_OK) {
        return HAL_ERROR;
    }
    ret = HAL_I2C_Master_Receive(&AHT21_I2C_PORT, AHT_21_ADDR, buff, 1, i2c_RETRY_TIME);
    buff[0] = buff[0] & 0x18;
    if (buff[0] != 0x18) {
        return HAL_ERROR;
    }
    return ret;
}

uint32_t AHT21_Read_Humidity(void)
{
    HAL_StatusTypeDef ret;
    uint8_t buff[8];
    uint32_t humidity;

    buff[0] = 0xAC;
    buff[1] = 0X33;
    buff[2] = 0x00;
    ret = HAL_I2C_Master_Transmit(&AHT21_I2C_PORT, AHT_21_ADDR, buff, 3, i2c_RETRY_TIME);
    if (ret != HAL_OK) {
        return HAL_ERROR;
    }
    HAL_Delay(100);
    ret = HAL_I2C_Master_Receive(&AHT21_I2C_PORT, AHT_21_ADDR, buff, 6, i2c_RETRY_TIME);
    //CALCULATING HUMIDITY
    humidity = (buff[1] << 12) | (buff[2] << 4) | (buff[3] >> 4);
    humidity = (humidity * 100);
    humidity = humidity / 0x100000;
    return humidity;
}

uint32_t AHT21_Read_Temperature(void)
{
    HAL_StatusTypeDef ret;
    uint8_t buff[8];
    uint32_t temperature;

    buff[0] = 0xAC;
    buff[1] = 0X33;
    buff[2] = 0x00;
    ret = HAL_I2C_Master_Transmit(&AHT21_I2C_PORT, AHT_21_ADDR, buff, 3, i2c_RETRY_TIME);
    if (ret != HAL_OK) {
        return HAL_ERROR;
    }
    HAL_Delay(100);
    ret = HAL_I2C_Master_Receive(&AHT21_I2C_PORT, AHT_21_ADDR, buff, 6, i2c_RETRY_TIME);
    //CALCULATING TEMPERATURE
    temperature = ((buff[3] & 0xF) << 16) | (buff[4] << 8) | (buff[5]);
    temperature = (temperature * 200);
    temperature = temperature / 0x100000;
    temperature = temperature - 50;
    return temperature;
}

HAL_StatusTypeDef AHT21_Read(uint32_t *temperature, uint32_t *humidity)
{
    /*
    uint8_t addr[] = {0xAC,0x33, 0x00};
    HAL_I2C_Master_Transmit(hi2c,AHT21_READ_CMD << 1,(uint8_t *)addr,3,1000);
    uint8_t data[6] = {0};
    HAL_I2C_Master_Receive(hi2c,(AHT21_READ_CMD << 1)|1,(uint8_t *)data,6,1000);
    *humidity = ((data[1] << 12) | (data[2] << 4) | (data[3] >> 4)) / 1048576.0;
    *temperature = ((((data[3] & 0x0F) << 16) | (data[4] << 8) | data[5]) / 1048576.0 * 200) - 50;
     * */
    HAL_StatusTypeDef ret;
    uint8_t buff[8];

    buff[0] = 0xAC;
    buff[1] = 0X33;
    buff[2] = 0x00;
    ret = HAL_I2C_Master_Transmit(&AHT21_I2C_PORT, AHT_21_ADDR, buff, 3, i2c_RETRY_TIME);
    if (ret != HAL_OK) {
        return HAL_ERROR;
    }
    HAL_Delay(100);
    ret = HAL_I2C_Master_Receive(&AHT21_I2C_PORT, AHT_21_ADDR, buff, 6, i2c_RETRY_TIME);
//    *humidity = (buff[1] << 12) | (buff[2] << 4) | (buff[3] >> 4);
//    *humidity = (*humidity * 100);
//    *humidity = *humidity / 0x100000;
//    *temperature = ((buff[3] & 0xF) << 16) | (buff[4] << 8) | (buff[5]);
//    *temperature = (*temperature * 200);
//    *temperature = *temperature / 0x100000;
//    *temperature = *temperature - 50;
    *humidity = ((buff[1] << 12) | (buff[2] << 4) | (buff[3] >> 4)) / 0x100000 * 100;
    *temperature = (((buff[3] & 0xF) << 16) | (buff[4] << 8) | (buff[5])) / 0x100000 * 200 - 50;
    return ret;
}
#endif



# if 1
#if AHT21_MODE == SOFT
/*Software I2C Strat*/
void I2c_Start(void)
{
    SDA_SET;
    SDA_D_OUT
            SCL_SET;
    SCL_D_OUT;
    DELAY_0_1US;
    SDA_CLR;
    DELAY_0_1US;
    SCL_CLR;
}

void I2c_Stop(void)
{
    SDA_CLR;
    DELAY_0_1US;
    SCL_SET;
    DELAY_0_1US;
    SDA_SET;
    DELAY_0_1US;
}

void I2c_WrByte(uint8_t dat)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        if (dat & 0x80)
        {
            SDA_SET;
        }
        else
        {
            SDA_CLR;
        }
        DELAY_0_1US;
        SCL_SET;
        dat <<= 1;
        DELAY_0_1US;
        SCL_CLR;
    }
    SDA_SET;
    DELAY_0_1US;
    SCL_SET;
    DELAY_0_1US;
    SCL_CLR;
}

uint8_t I2c_RdByte(void)
{
    uint8_t dat, i;
    SDA_SET;
    SDA_D_IN;
    dat = 0;
    for (i = 0; i < 8; i++)
    {
        DELAY_0_1US;
        SCL_SET;
        DELAY_0_1US;
        dat <<= 1;
        if (SDA_IN)
            dat++;
        SCL_CLR;
    }
    SDA_D_OUT;
    SDA_CLR;
    DELAY_0_1US;
    SCL_SET;
    DELAY_0_1US;
    SCL_CLR;
    return (dat);
}
/*Software I2C End*/
void GetAHT21Data(float *humidity, float *temperature)
{
    I2c_Start();
    I2c_WrByte(AHT21_READ_CMD << 1);
    I2c_WrByte(0xAC);
    I2c_WrByte(0x33);
    I2c_WrByte(0x00);
    I2c_Stop();
    HAL_Delay(80);
    uint8_t data[6] = {0};
    I2c_Start();
    I2c_WrByte((AHT21_READ_CMD << 1) | 1);
    for (int i = 0; i < 6; i++)
    {
        data[i] = I2c_RdByte();
    }
    I2c_Stop();
    *humidity = ((data[1] << 12) | (data[2] << 4) | (data[3] >> 4)) / 1048576.0;
    *temperature = ((((data[3] & 0x0F) << 16) | (data[4] << 8) | data[5]) / 1048576.0 * 200) - 50;
}
void StartAHT21(void)
{
    I2c_Start();
    I2c_WrByte(AHT21_READ_CMD << 1);
    I2c_WrByte(0xBE);
    I2c_WrByte(0x08);
    I2c_WrByte(0x00);
    I2c_Stop();
    HAL_Delay(10);
}
void ResetAHT21(void)
{
    I2c_Start();
    I2c_WrByte(AHT21_READ_CMD << 1);
    I2c_WrByte(0xBA);
    I2c_Stop();
    HAL_Delay(20);
}
#elif AHT21_MODE == HARD
void GetAHT21Data(I2C_HandleTypeDef *hi2c,float *humidity, float *temperature)
{
    uint8_t addr[] = {0xAC,0x33, 0x00};
    if (HAL_I2C_Master_Transmit(hi2c,AHT21_READ_CMD << 1,(uint8_t *)addr,3,1000) != HAL_OK)
        return;
    uint8_t data[6] = {0};
    HAL_I2C_Master_Receive(hi2c,(AHT21_READ_CMD << 1)|1,(uint8_t *)data,6,1000);
    *humidity = ((data[1] << 12) | (data[2] << 4) | (data[3] >> 4)) / 1048576.0 * 100;
    *temperature = ((((data[3] & 0x0F) << 16) | (data[4] << 8) | data[5]) / 1048576.0 * 200) - 50;
}
void StartAHT21(I2C_HandleTypeDef *hi2c)
{
    uint8_t addr[] = {0xBE,0x08, 0x00};
    HAL_I2C_Master_Transmit(hi2c,AHT21_READ_CMD << 1,(uint8_t *)addr,3,1000);
    HAL_Delay(10);
}
void ResetAHT21(I2C_HandleTypeDef *hi2c)
{
    HAL_I2C_Master_Transmit(hi2c,AHT21_READ_CMD << 1,(uint8_t *)0xBA,3,1000);
    HAL_Delay(20);
}
#else
#warning "AHT21_MODE not defined correctly!"
#endif

#endif
