#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#define GF_LCD_INIT_ENABLE 0
#if GF_LCD_INIT_ENABLE

#define USE_HORIZONTAL 2  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

#if USE_HORIZONTAL == 0 || USE_HORIZONTAL == 1
#define LCD_W 80
#define LCD_H 160
#else
#define LCD_W 160
#define LCD_H 80
#endif

#include "main.h"

//-----------------LCD端口定义---------------- 

//#define LCD_SCLK_Clr() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0, GPIO_PIN_RESET)//SCL=SCLK
//#define LCD_SCLK_Set() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0, GPIO_PIN_SET)
//
//#define LCD_MOSI_Clr() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1, GPIO_PIN_RESET)//SDA=MOSI
//#define LCD_MOSI_Set() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1, GPIO_PIN_SET)
//
//#define LCD_RES_Clr()  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2, GPIO_PIN_RESET)//RES
//#define LCD_RES_Set()  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2, GPIO_PIN_SET)
//
//#define LCD_DC_Clr()   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3, GPIO_PIN_RESET)//DC
//#define LCD_DC_Set()   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3, GPIO_PIN_SET)
//
//#define LCD_CS_Clr()   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4, GPIO_PIN_RESET)//CS
//#define LCD_CS_Set()   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4, GPIO_PIN_SET)
//
//#define LCD_BLK_Clr()  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5, GPIO_PIN_RESET)//BLK
//#define LCD_BLK_Set()  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5, GPIO_PIN_SET)

#define IPS_PIN_CS          ips_cs_Pin
#define IPS_PIN_DC          ips_dc_Pin
#define IPS_PIN_RST         ips_rst_Pin
#define IPS_PIN_SDA         ips_mosi_Pin
#define IPS_PIN_SCL         ips_sck_Pin
#define IPS_PIN_BLK
#define IPS_PORT_CS         ips_cs_GPIO_Port
#define IPS_PORT_DC         ips_dc_GPIO_Port
#define IPS_PORT_RST        ips_rst_GPIO_Port
#define IPS_PORT_SDA        ips_mosi_GPIO_Port
#define IPS_PORT_SCL        ips_sck_GPIO_Port
#define IPS_PORT_BLK

#ifdef __cplusplus
extern "C" {
#endif
//void LCD_Writ_Bus(uint8_t dat);//模拟SPI时序
void LCD_WR_DATA8(uint8_t dat);//写入一个字节
void LCD_WR_DATA(uint16_t dat);//写入两个字节
void LCD_WR_REG(uint8_t dat);//写入一个指令
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
#ifdef __cplusplus
}
#endif

#endif // enable file

#endif
