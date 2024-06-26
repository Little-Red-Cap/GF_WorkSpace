#include "ds18b20.h"

#if 0
#include <stdint.h>

//#include "delay.h"
void delay_us(volatile unsigned long nTime);


void DS18B20_Rst(void)
{
    DS18B20_IO_OUT(); //SET PA0 OUTPUT
    DS18B20_DQ_OUT = 0;
    delay_us(750);
    DS18B20_DQ_OUT = 1; //DQ=1
    delay_us(15);     //15US
}


uint8_t DS18B20_Check(void)
{
    uint8_t retry = 0;
    DS18B20_IO_IN();//SET PA0 INPUT
    while (DS18B20_DQ_IN && retry < 200) {
        retry++;
        delay_us(1);
    };
    if (retry >= 200)return 1;
    else retry = 0;
    while (!DS18B20_DQ_IN && retry < 240) {
        retry++;
        delay_us(1);
    };
    return 0;
}


uint8_t DS18B20_Read_Bit(void)   // read one bit
{
    uint8_t data;
    DS18B20_IO_OUT();//SET PA0 OUTPUT
    DS18B20_DQ_OUT = 0;
    delay_us(2);
    DS18B20_DQ_OUT = 1;
    DS18B20_IO_IN();//SET PA0 INPUT
    delay_us(12);
    if (DS18B20_DQ_IN)data = 1;
    else data = 0;
    delay_us(50);
    return data;
}


uint8_t DS18B20_Read_Byte(void)  // read one byte
{
    uint8_t i, j, dat;
    dat = 0;
    for (i = 1; i <= 8; i++) {
        j = DS18B20_Read_Bit();
        dat = (j << 7) | (dat >> 1);
    }
    return dat;
}


void DS18B20_Write_Byte(uint8_t dat)
{
    uint8_t j;
    uint8_t testb;
    DS18B20_IO_OUT();//SET PA0 OUTPUT;
    for (j = 1; j <= 8; j++) {
        testb = dat & 0x01;
        dat = dat >> 1;
        if (testb) {
            DS18B20_DQ_OUT = 0;// Write 1
            delay_us(2);
            DS18B20_DQ_OUT = 1;
            delay_us(60);
        } else {
            DS18B20_DQ_OUT = 0;// Write 0
            delay_us(60);
            DS18B20_DQ_OUT = 1;
            delay_us(2);
        }
    }
}


void DS18B20_Start(void)    // ds1820 start convert
{
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
}


uint8_t DS18B20_Init(void)
{
//    GPIO_InitTypeDef GPIO_InitStructure;    //GPIO
//    //RCC->APB2ENR|=1<<2;
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  //SPI CS
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//    /* Deselect the PA0 Select high */
//    GPIO_SetBits(GPIOA, GPIO_Pin_0);
    DS18B20_Rst();
    return DS18B20_Check();
}


short DS18B20_Get_Temp(void)
{
    uint8_t temp;
    uint8_t TL, TH;
    short tem;
    DS18B20_Start();                    // ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0xbe);// convert	    
    TL = DS18B20_Read_Byte(); // LSB
    TH = DS18B20_Read_Byte(); // MSB

    if (TH > 7) {
        TH = ~TH;
        TL = ~TL;
        temp = 0;
    } else temp = 1;
    tem = TH;
    tem <<= 8;
    tem += TL;
    tem = (float) tem * 0.625;
    if (temp)return tem;
    else return -tem;
} 
#endif
