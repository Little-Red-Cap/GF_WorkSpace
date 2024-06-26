#ifndef _GF_SOFT_I2C_H_
#define _GF_SOFT_I2C_H_

#ifdef __cplusplus
extern "C" {
#endif

#define GF_SOFT_I2C 0
#if GF_SOFT_I2C
#include "stm32f1xx_hal.h"
#include "gpio.h"


#define CPU_FREQUENCY_MHZ       72
#define SOFT_I2C_SCL_PIN       GPIO_PIN_4
#define SOFT_I2C_SCL_PORT      GPIOB
#define SOFT_I2C_SDA_PIN       GPIO_PIN_11
#define SOFT_I2C_SDA_PORT      GPIOB

#define SDA_Dout_LOW()         HAL_GPIO_WritePin(SOFT_I2C_SDA_PORT, SOFT_I2C_SDA_PIN, GPIO_PIN_RESET)
#define SDA_Dout_HIGH()        HAL_GPIO_WritePin(SOFT_I2C_SDA_PORT, SOFT_I2C_SDA_PIN, GPIO_PIN_SET)
#define SDA_Data_IN()          HAL_GPIO_ReadPin(SOFT_I2C_SDA_PORT, SOFT_I2C_SDA_PIN)
#define SCL_Dout_LOW()         HAL_GPIO_WritePin(SOFT_I2C_SCL_PORT, SOFT_I2C_SCL_PIN, GPIO_PIN_RESET)
#define SCL_Dout_HIGH()        HAL_GPIO_WritePin(SOFT_I2C_SCL_PORT, SOFT_I2C_SCL_PIN, GPIO_PIN_SET)
#define SCL_Data_IN()          HAL_GPIO_ReadPin(SOFT_I2C_SCL_PORT, SOFT_I2C_SCL_PIN)
#define SDA_Write(XX)          HAL_GPIO_WritePin(SOFT_I2C_SDA_PORT, SOFT_I2C_SDA_PIN, (XX?GPIO_PIN_SET:GPIO_PIN_RESET))

#define I2C_SOFT                ((I2C_TypeDef *) 0x00000000UL)
extern I2C_HandleTypeDef hi2c0;

void Soft_I2C_Init(void);

void Soft_I2C_Start(void);

void Soft_I2C_Stop(void);

void Soft_I2C_Send_Byte(uint8_t txd);

uint8_t Soft_I2C_Read_Byte(uint8_t ack);

void Soft_I2C_NAck(void);

void Soft_I2C_Ack(void);

uint8_t Soft_I2C_Wait_Ack(void);


void SOFT_I2C_Master_Transmit(uint8_t daddr, uint8_t *buff, uint8_t len);

void Soft_I2C_Master_Transmit_Reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t data_len, const uint8_t *data);

void Soft_I2C_Master_Receive_Reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t data_len, uint8_t *data);
#endif

#ifdef __cplusplus
}
#endif
#endif
