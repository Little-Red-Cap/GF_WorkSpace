#ifndef GF_DHT11_H
#define GF_DHT11_H

#define GF_DHT11_ENABLE 0
#ifdef __cplusplus
extern "C" {
#endif
/* DHT11数据结构体 */
typedef union {
    unsigned char buff[5];
    struct {
        unsigned char humiH;      /* 湿度整数位 */
        unsigned char humiL;      /* 湿度小数位 */
        unsigned char tempH;      /* 温度整数位 */
        unsigned char tempL;      /* 温度小数位 */
        unsigned char check_sum;  /* 校验和 */
    } data;
} DHT11_Data_TypeDef;
/**
 * @brief 读取DHT11温湿度传感器的数据，温度值(范围:0~50°)，湿度值(范围:20%~90%)
 * @param  data 传感器返回的数据
 * @retval 返回读取状态 0表示读取成功，1表示数据校验失败
 */
unsigned char DHT11_Read_Data(DHT11_Data_TypeDef *dht11);
#ifdef __cplusplus
}
#endif
#endif //GF_DHT11_H
