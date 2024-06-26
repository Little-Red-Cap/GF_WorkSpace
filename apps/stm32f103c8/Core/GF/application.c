#include "main.h"
#include "usart.h"
#include "adc.h"
#include "tim.h"
#include "i2c.h"
#include "dma.h"

void SystemClock_Config();
void MX_GPIO_Init();

__attribute__((__noreturn__)) __attribute__((used))
void main_application()
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_USART1_UART_Init();
//    MX_I2C2_Init();
//    HAL_I2C_DeInit(&hi2c2);
//    MX_ADC1_Init();
//    HAL_ADCEx_Calibration_Start(&hadc1);
    for (;;) {
        HAL_GPIO_TogglePin(led_inline_GPIO_Port, led_inline_Pin);
        HAL_Delay(100);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart1) {
        return;
    }
}

__attribute__((used)) void gc_print(const char *data, int len)
{
    HAL_UART_Transmit(&huart1, (uint8_t *) data, len, 100);
}
