#ifndef D24_01_DEMOFILE_H
#define D24_01_DEMOFILE_H

#include "stm32f1xx_hal.h"
#ifdef __cplusplus
extern "C" {
#endif
void SGP30_Init();
//float SGP30_GetCO2andVOC();
HAL_StatusTypeDef SGP30_GetCO2andVOC(I2C_HandleTypeDef *hi2c, float *CO2Value, float *VOCValue);
#ifdef __cplusplus
}
#endif
#endif //D24_01_DEMOFILE_H
