/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define led_inline_Pin GPIO_PIN_13
#define led_inline_GPIO_Port GPIOC
#define key_curtain_mode_Pin GPIO_PIN_0
#define key_curtain_mode_GPIO_Port GPIOA
#define key_curtain_en_Pin GPIO_PIN_1
#define key_curtain_en_GPIO_Port GPIOA
#define a_gas_Pin GPIO_PIN_4
#define a_gas_GPIO_Port GPIOA
#define a_lighlt_Pin GPIO_PIN_5
#define a_lighlt_GPIO_Port GPIOA
#define a_rain_Pin GPIO_PIN_6
#define a_rain_GPIO_Port GPIOA
#define sensor_int_Pin GPIO_PIN_7
#define sensor_int_GPIO_Port GPIOA
#define sensor_cs_Pin GPIO_PIN_0
#define sensor_cs_GPIO_Port GPIOB
#define sensor_ado_Pin GPIO_PIN_1
#define sensor_ado_GPIO_Port GPIOB
#define sensor_scl_Pin GPIO_PIN_10
#define sensor_scl_GPIO_Port GPIOB
#define sensor_sda_Pin GPIO_PIN_11
#define sensor_sda_GPIO_Port GPIOB
#define d_curtain_mode_Pin GPIO_PIN_12
#define d_curtain_mode_GPIO_Port GPIOB
#define d_gas_Pin GPIO_PIN_13
#define d_gas_GPIO_Port GPIOB
#define d_beep_Pin GPIO_PIN_14
#define d_beep_GPIO_Port GPIOB
#define d_led_Pin GPIO_PIN_15
#define d_led_GPIO_Port GPIOB
#define d_door_Pin GPIO_PIN_8
#define d_door_GPIO_Port GPIOA
#define d_curtain_Pin GPIO_PIN_11
#define d_curtain_GPIO_Port GPIOA
#define rc522_cs_Pin GPIO_PIN_15
#define rc522_cs_GPIO_Port GPIOA
#define rc522_sck_Pin GPIO_PIN_3
#define rc522_sck_GPIO_Port GPIOB
#define rc522_miso_Pin GPIO_PIN_4
#define rc522_miso_GPIO_Port GPIOB
#define rc522_mosi_Pin GPIO_PIN_5
#define rc522_mosi_GPIO_Port GPIOB
#define d_window_Pin GPIO_PIN_6
#define d_window_GPIO_Port GPIOB
#define rc522_rst_Pin GPIO_PIN_7
#define rc522_rst_GPIO_Port GPIOB
#define key_led_mode_Pin GPIO_PIN_8
#define key_led_mode_GPIO_Port GPIOB
#define key_led_index_Pin GPIO_PIN_9
#define key_led_index_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
