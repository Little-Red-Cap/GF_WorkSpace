#include "lcd_init.h"

#if GF_LCD_INIT_ENABLE

#include "spi.h"

#define LCD_USE_SPI 1

inline static void delay_ms(uint32_t ms) {HAL_Delay(ms);}
inline static void LCD_CS_Clr() {HAL_GPIO_WritePin(IPS_PORT_CS, IPS_PIN_CS, GPIO_PIN_RESET);}
inline static void LCD_CS_Set() {HAL_GPIO_WritePin(IPS_PORT_CS, IPS_PIN_CS, GPIO_PIN_SET);}
inline static void LCD_DC_Clr() {HAL_GPIO_WritePin(IPS_PORT_DC, IPS_PIN_DC, GPIO_PIN_RESET);}
inline static void LCD_DC_Set() {HAL_GPIO_WritePin(IPS_PORT_DC, IPS_PIN_DC, GPIO_PIN_SET);}
inline static void LCD_RES_Clr() {HAL_GPIO_WritePin(IPS_PORT_RST, IPS_PIN_RST, GPIO_PIN_RESET);}
inline static void LCD_RES_Set() {HAL_GPIO_WritePin(IPS_PORT_RST, IPS_PIN_RST, GPIO_PIN_SET);}
inline static void LCD_SCLK_Clr() {HAL_GPIO_WritePin(IPS_PORT_SCL, IPS_PIN_SCL, GPIO_PIN_RESET);}
inline static void LCD_SCLK_Set() {HAL_GPIO_WritePin(IPS_PORT_SCL, IPS_PIN_SCL, GPIO_PIN_SET);}
inline static void LCD_MOSI_Clr() {HAL_GPIO_WritePin(IPS_PORT_SDA, IPS_PIN_SDA, GPIO_PIN_RESET);}
inline static void LCD_MOSI_Set() {HAL_GPIO_WritePin(IPS_PORT_SDA, IPS_PIN_SDA, GPIO_PIN_SET);}
//inline static void LCD_BLK_Clr() {HAL_GPIO_WritePin(IPS_PORT_BLK, IPS_PIN_BLK, GPIO_PIN_RESET);}
//inline static void LCD_BLK_Set() {HAL_GPIO_WritePin(IPS_PORT_BLK, IPS_PIN_BLK, GPIO_PIN_SET);}

static void LCD_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    GPIO_InitStruct.Pin = IPS_PIN_CS;
    HAL_GPIO_Init(IPS_PORT_CS, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = IPS_PIN_DC;
    HAL_GPIO_Init(IPS_PORT_DC, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = IPS_PIN_RST;
    HAL_GPIO_Init(IPS_PORT_RST, &GPIO_InitStruct);
#if LCD_USE_SPI
    MX_SPI2_Init();
#else
    GPIO_InitStruct.Pin = IPS_PIN_SDA;
    HAL_GPIO_Init(IPS_PORT_SDA, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = IPS_PIN_SCL;
    HAL_GPIO_Init(IPS_PORT_SCL, &GPIO_InitStruct);
#endif
}

/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
static inline void LCD_Writ_Bus(uint8_t dat)
{
//    LCD_CS_Clr();
#if LCD_USE_SPI
    HAL_SPI_Transmit(&hspi2, &dat, 1, 10);
#else
    for (uint8_t i = 0; i < 8; i++) {
        LCD_SCLK_Clr();
        if (dat & 0x80) {
            LCD_MOSI_Set();
        } else {
            LCD_MOSI_Clr();
        }
        LCD_SCLK_Set();
        dat <<= 1;
    }
#endif
//    LCD_CS_Set();
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
    LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
//    for (uint8_t i = 0; i < 16; i++) {
//        LCD_SCLK_Clr();
//        if (dat & 0x8000) {
//            LCD_MOSI_Set();
//        } else {
//            LCD_MOSI_Clr();
//        }
//        LCD_SCLK_Set();
//        dat <<= 1;
//    }
    LCD_Writ_Bus(dat >> 8);
    LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD_WR_REG(uint8_t dat)
{
    LCD_DC_Clr();//写命令
    LCD_Writ_Bus(dat);
    LCD_DC_Set();//写数据
}


/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    if (USE_HORIZONTAL == 0) {
        LCD_WR_REG(0x2a);//列地址设置
        LCD_WR_DATA(x1 + 26);
        LCD_WR_DATA(x2 + 26);
        LCD_WR_REG(0x2b);//行地址设置
        LCD_WR_DATA(y1 + 1);
        LCD_WR_DATA(y2 + 1);
        LCD_WR_REG(0x2c);//储存器写
    } else if (USE_HORIZONTAL == 1) {
        LCD_WR_REG(0x2a);//列地址设置
        LCD_WR_DATA(x1 + 26);
        LCD_WR_DATA(x2 + 26);
        LCD_WR_REG(0x2b);//行地址设置
        LCD_WR_DATA(y1 + 1);
        LCD_WR_DATA(y2 + 1);
        LCD_WR_REG(0x2c);//储存器写
    } else if (USE_HORIZONTAL == 2) {
    LCD_WR_REG(0x2a);//列地址设置
    LCD_WR_DATA(x1 + 1);
    LCD_WR_DATA(x2 + 1);
    LCD_WR_REG(0x2b);//行地址设置
    LCD_WR_DATA(y1 + 26);
    LCD_WR_DATA(y2 + 26);
    LCD_WR_REG(0x2c);//储存器写
    } else {
        LCD_WR_REG(0x2a);//列地址设置
        LCD_WR_DATA(x1 + 1);
        LCD_WR_DATA(x2 + 1);
        LCD_WR_REG(0x2b);//行地址设置
        LCD_WR_DATA(y1 + 26);
        LCD_WR_DATA(y2 + 26);
        LCD_WR_REG(0x2c);//储存器写
    }
}

void LCD_Init(void) // ST7735S
{
    LCD_GPIO_Init();//初始化GPIO
    LCD_CS_Clr();

    LCD_RES_Clr();//复位
    delay_ms(100);
    LCD_RES_Set();
    delay_ms(100);

//    LCD_BLK_Set();//打开背光
//    delay_ms(100);

    LCD_WR_REG(0x11);     //Sleep out
    delay_ms(120);                //Delay 120ms
    LCD_WR_REG(0xB1);     //Normal mode
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);
    LCD_WR_REG(0xB2);     //Idle mode
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);
    LCD_WR_REG(0xB3);     //Partial mode
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);
    LCD_WR_REG(0xB4);     //Dot inversion
    LCD_WR_DATA8(0x03);
    LCD_WR_REG(0xC0);     //AVDD GVDD
    LCD_WR_DATA8(0xAB);
    LCD_WR_DATA8(0x0B);
    LCD_WR_DATA8(0x04);
    LCD_WR_REG(0xC1);     //VGH VGL
    LCD_WR_DATA8(0xC5);   //C0
    LCD_WR_REG(0xC2);     //Normal Mode
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x00);
    LCD_WR_REG(0xC3);     //Idle
    LCD_WR_DATA8(0x8D);
    LCD_WR_DATA8(0x6A);
    LCD_WR_REG(0xC4);     //Partial+Full
    LCD_WR_DATA8(0x8D);
    LCD_WR_DATA8(0xEE);
    LCD_WR_REG(0xC5);     //VCOM
    LCD_WR_DATA8(0x0F);
    LCD_WR_REG(0xE0);     //positive gamma
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x0E);
    LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x10);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x02);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x0F);
    LCD_WR_DATA8(0x25);
    LCD_WR_DATA8(0x36);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x10);
    LCD_WR_REG(0xE1);     //negative gamma
    LCD_WR_DATA8(0x0A);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x0F);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x02);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x0F);
    LCD_WR_DATA8(0x25);
    LCD_WR_DATA8(0x35);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x10);

    LCD_WR_REG(0xFC);
    LCD_WR_DATA8(0x80);

    LCD_WR_REG(0x3A);
    LCD_WR_DATA8(0x05);
    LCD_WR_REG(0x36);
    if (USE_HORIZONTAL == 0)LCD_WR_DATA8(0x08);
    else if (USE_HORIZONTAL == 1)LCD_WR_DATA8(0xC8);
    else if (USE_HORIZONTAL == 2)LCD_WR_DATA8(0x78);
    else LCD_WR_DATA8(0xA8);
    LCD_WR_REG(0x21);     //Display inversion
    LCD_WR_REG(0x29);     //Display on
    LCD_WR_REG(0x2A);     //Set Column Address
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x1A);  //26
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x69);   //105
    LCD_WR_REG(0x2B);     //Set Page Address
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x01);    //1
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0xA0);    //160
    LCD_WR_REG(0x2C);
}

#endif
