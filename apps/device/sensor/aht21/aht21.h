#ifndef AHT21_H
#define AHT21_H

#include "main.h"
#include "stm32f1xx_hal.h"
#ifdef __cplusplus
extern "C" {
#endif



#if 0 // code1
extern I2C_HandleTypeDef hi2c2;
#define AHT21_I2C_PORT hi2c2

HAL_StatusTypeDef AHT21_init(void);
uint32_t AHT21_Read_Humidity(void);
uint32_t AHT21_Read_Temperature(void);
HAL_StatusTypeDef AHT21_Read(uint32_t *temperature, uint32_t *humidity);
#endif

#if 1 // code2
#define SOFT 0
#define HARD 1

#define AHT21_MODE HARD

#define AHT21_READ_CMD 0x38
#if AHT21_MODE == SOFT
#define DELAY_0_1US HAL_Delay(1);

#define SCL_Pin GPIO_PIN_8
#define SCL_GPIO_Port GPIOE
#define SDA_Pin GPIO_PIN_7
#define SDA_GPIO_Port GPIOE

#define SCL_SET HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET);
#define SCL_CLR HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_RESET);
#define SCL_D_OUT                                  \
    GPIO_InitTypeDef GPIO_InitStruct_SCL_OUT = {0};      \
    GPIO_InitStruct_SCL_OUT.Pin = SCL_Pin;             \
    GPIO_InitStruct_SCL_OUT.Mode = GPIO_MODE_OUTPUT_PP;  \
    GPIO_InitStruct_SCL_OUT.Pull = GPIO_NOPULL;          \
    GPIO_InitStruct_SCL_OUT.Speed = GPIO_SPEED_FREQ_LOW; \
    HAL_GPIO_Init(SCL_GPIO_Port, &GPIO_InitStruct_SCL_OUT);
#define SDA_SET HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET);
#define SDA_CLR HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET);
#define SDA_IN HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin)
#define SDA_D_OUT                                  \
    GPIO_InitTypeDef GPIO_InitStruct_SDA_OUT = {0};      \
    GPIO_InitStruct_SDA_OUT.Pin = SDA_Pin;             \
    GPIO_InitStruct_SDA_OUT.Mode = GPIO_MODE_OUTPUT_PP;  \
    GPIO_InitStruct_SDA_OUT.Pull = GPIO_NOPULL;          \
    GPIO_InitStruct_SDA_OUT.Speed = GPIO_SPEED_FREQ_LOW; \
    HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct_SDA_OUT);
#define SDA_D_IN                             \
    GPIO_InitTypeDef GPIO_InitStruct_SDA_IN = {0}; \
    GPIO_InitStruct_SDA_IN.Pin = SDA_Pin;        \
    GPIO_InitStruct_SDA_IN.Mode = GPIO_MODE_INPUT; \
    GPIO_InitStruct_SDA_IN.Pull = GPIO_NOPULL;     \
    HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct_SDA_IN);

void I2c_Start(void);
void I2c_Stop(void);
void I2c_WrByte(uint8_t dat);
uint8_t I2c_RdByte(void);
void GetAHT21Data(float *humidity,float *temperature);
void StartAHT21(void);
void ResetAHT21(void);
#elif AHT21_MODE == HARD
void GetAHT21Data(I2C_HandleTypeDef *hi2c,float *humidity, float *temperature);
    void StartAHT21(I2C_HandleTypeDef *hi2c);
    void ResetAHT21(I2C_HandleTypeDef *hi2c);
#endif
#endif

#ifdef __cplusplus
}
#endif
#endif //AHT21_H
