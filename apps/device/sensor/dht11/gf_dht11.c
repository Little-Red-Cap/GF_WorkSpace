#include "gf_dht11.h"
#if GF_DHT11_ENABLE

#include "stm32f1xx_hal_gpio.h"
//#ifdef STM32F1xx_HAL_GPIO_H
//#elifdef STM32F1xx_LL_GPIO_H
//#endif
#define DHT11_Pin GPIO_PIN_0
#define DHT11_GPIO_Port GPIOB

void delay_us(volatile unsigned long nTime);
static inline void dht11_delay_us(volatile unsigned long nTime) { delay_us(nTime);}
static inline void dht11_delay_ms(volatile unsigned long nTime) { HAL_Delay(nTime);}
static inline void dht11_pin_low() { HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET); }
static inline void dht11_pin_high() { HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET); }
static inline uint8_t dht11_read_pin() { return HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin); }
static inline void dht11_pin_dir(enum {input = 0, output = 1} dir)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_Pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    if(dir == output) {
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
    } else {
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
    }
}

uint8_t DHT11_Read_Data(DHT11_Data_TypeDef *dht11)
{
    /* DHT11响应开始 */
    dht11_pin_dir(1);
    /* DHT11 复位 */
    dht11_pin_low();
    dht11_delay_ms(20);     //拉低至少18ms
    dht11_pin_high();
    dht11_delay_us(30);      //主机拉高20~40us
    /* DHT11 检查 */
    dht11_pin_dir(0);
    for (uint8_t retry = 0; dht11_read_pin(); retry++) { // DHT11会拉低40~80us
        if (retry >= 100) // 接收信号超时
            return 1;
        dht11_delay_us(1);
    }
    for (uint8_t retry = 0; !dht11_read_pin(); retry++) { // DHT11拉低后会再次拉高40~80us
        if (retry >= 100) // 接收信号超时
            return 1;
        dht11_delay_us(1);
    }
    /* DHT11 读取数据 */
    for (uint8_t index = 0; index < 5; index++)//读取40位数据
    {
        for (uint8_t bit = 0; bit < 8; bit++) { // 从DHT11读取一个字节
            // 从DHT11读取一个位
            dht11->buff[index] <<= 1;
            while (dht11_read_pin());  //等待变为低电平
            while (!dht11_read_pin()); //等待变为高电平
            dht11_delay_us(40);//等待40us
            dht11->buff[index] |= dht11_read_pin();
        }
    }
    /* 检验和校验 */
    if ((dht11->buff[0] + dht11->buff[1] + dht11->buff[2] + dht11->buff[3]) != dht11->buff[4])
        return 1;
    return 0;
}

#endif
