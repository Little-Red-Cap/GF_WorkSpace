//#include "main.h"
//#include "max7219.h"
//
//// Pin
//#define Max7219_pinCLK_1    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET)
//#define Max7219_pinCLK_0    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET)
//#define Max7219_pinCS_1     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET)
//#define Max7219_pinCS_0     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET)
//#define Max7219_pinDIN_1    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET)
//#define Max7219_pinDIN_0    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET)
//
////MAX7219
//#define REG_DECODE        0x09
//#define REG_INTENSITY     0x0a
//#define REG_SCAN_LIMIT    0x0b
//#define REG_SHUTDOWN      0x0c
//#define REG_DISPLAY_TEST  0x0f
//#define INTENSITY_MIN     0x00
//#define INTENSITY_MAX     0x0f
//
///*
//*********************************************************************************************************
//* LED Segments:         a
//*                     ----
//*                   f|    |b
//*                    |  g |
//*                     ----
//*                   e|    |c
//*                    |    |
//*                     ----  o dp
//*                       d
//*   Register bits:
//*      bit:  7  6  5  4  3  2  1  0
//*           dp  a  b  c  d  e  f  g
//*********************************************************************************************************
//* Example : The letter 'I' is represented by illuminating LED's 'b' and 'c' (refer above diagram)
//*           Therfore the binary representation of 'I' is as follows
//*
//*           abcdefg
//*           0110000
//*
//*           The table below contains all the binary values for the desired font. New font characters
//*           can be added or altered as required.
//*
//*           The DP bit is used to switch on the decimal place LED. DP is not included in the below table
//*           but is added in the register within the libary depending on the content being displayed.
//*********************************************************************************************************
//
//*/
//static const struct max7219_Font_t {
//    char ascii;
//    char segs;
//} max7219_Font[] __attribute__((unused)) = {
//        {'A',0b1110111},{'B',0b1111111},{'C',0b1001110},{'D',0b1111110},{'E',0b1001111},{'F',0b1000111},
//        {'G',0b1011110},{'H',0b0110111},{'I',0b0110000},{'J',0b0111100},{'L',0b0001110},{'N',0b1110110},
//        {'O',0b1111110},{'P',0b1100111},{'R',0b0000101},{'S',0b1011011},{'T',0b0001111},{'U',0b0111110},
//        {'Y',0b0100111},{'[',0b1001110},{']',0b1111000},{'_',0b0001000},{'a',0b1110111},{'b',0b0011111},
//        {'c',0b0001101},{'d',0b0111101},{'e',0b1001111},{'f',0b1000111},{'g',0b1011110},{'h',0b0010111},
//        {'i',0b0010000},{'j',0b0111100},{'l',0b0001110},{'n',0b0010101},{'o',0b1111110},{'p',0b1100111},
//        {'r',0b0000101},{'s',0b1011011},{'t',0b0001111},{'u',0b0011100},{'y',0b0100111},{'-',0b0000001},
//        {' ',0b0000000},{'0',0b1111110},{'1',0b0110000},{'2',0b1101101},{'3',0b1111001},{'4',0b0110011},
//        {'5',0b1011011},{'6',0b1011111},{'7',0b1110000},{'8',0b1111111},{'9',0b1111011},{'\0',0b0000000},
//};
//static const char font[] = {
//        0b1111110,0b0110000,0b1101101,0b1111001,0b0110011,
//        0b1011011,0b1011111,0b1110000,0b1111111,0b1111011
//};
//
//static void max7219_write_byte(uint8_t *data)
//{
//    for (uint8_t i = 7; i; i--)
//    {
//        Max7219_pinCLK_0;
//        if(*data & 0x80)
//            Max7219_pinDIN_1;
//        else
//            Max7219_pinDIN_0;
//        *data <<= 1;
//        Max7219_pinCLK_1;
//    }
//}
//
//void max7219_write(uint8_t address, uint8_t data)
//{
//    Max7219_pinCS_0;
//    max7219_write_byte(&address);
//    max7219_write_byte(&data);
//    Max7219_pinCS_1;
//}
//
//void max7219_init(void)
//{
//    max7219_write(0x09, 0x00);  //译码方式为BCD码
//    max7219_write(0x0a, 0x03);  //亮度
//    max7219_write(0x0b, 0x07);  //扫描界限；8位数码管显示
//    max7219_write(0x0c, 0x01);  //掉电模式为0.普通模式为1；
//    max7219_write(0x0f, 0x00);  //显示测试为一；测试结束，正常显示为0；
//}
//
//void max7219_brightness(uint8_t val)
//{
//    max7219_write(REG_INTENSITY, val &= 0x0f);
//}
//
//void max7219_off(_Bool enable)
//{
//    max7219_write(REG_SHUTDOWN, enable);
//}
//
//void max7219_scanLimit(uint8_t bit_digits)
//{
//    max7219_write(REG_SCAN_LIMIT, bit_digits);
//}
//
//void max7219_decodeMode(uint8_t mode)
//{
//    max7219_write(REG_DECODE, mode);
//}
//
//void max7219_test(_Bool enable)
//{
//    max7219_write(REG_DISPLAY_TEST, enable);
//}
//
//void max7219_clear(void)
//{
//    for (int i = 1; i <= 8; i++)
//        max7219_write(i, 0x00);
//}
//
//#ifdef USE_APP_CODE
//static char get_font(char code)
//{
//    for (int i = 0; i < 60; ++i)
//    {
//        if (max7219_Font[i].ascii == code)
//            return max7219_Font[i].segs;
//    }
//}
//
//void max7219_write_oneDigit(uint8_t bit, uint8_t digit)
//{
//    max7219_write(9 - bit, font[digit]);
//}
//
//void max7219_showPWM_ratio(uint8_t dut)
//{
//    max7219_write(8 + 1 - 5, get_font(' '));
//    max7219_write(8 + 1 - 6, font[dut / 100]);
//    max7219_write(8 + 1 - 7, font[dut / 10 % 10]);
//    max7219_write(8 + 1 - 8, font[dut % 10]);
//}
//
//void max7219_showPWM_frequency(uint16_t hz)
//{
//    max7219_write(8 + 1 - 1, font[hz / 1000 % 10]);
//    max7219_write(8 + 1 - 2, font[hz / 100 % 10]);
//    max7219_write(8 + 1 - 3, font[hz / 10 % 10]);
//    max7219_write(8 + 1 - 4, font[hz % 10]);
//}
//
//void max7219_showNum(int16_t num1, int16_t num2)
//{
//
//}
//
//void max7219_showStr(char *string)
//{
//    ;
//}
//#endif
//
//
//#if 0
///*
//* The MIT License (MIT)
//*
//* Copyright (c) JEMRF
//*
//* Permission is hereby granted, free of charge, to any person obtaining a copy
//* of this software and associated documentation files (the "Software"), to deal
//* in the Software without restriction, including without limitation the rights
//* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//* copies of the Software, and to permit persons to whom the Software is
//* furnished to do so, subject to the following conditions:
//*
//* The above copyright notice and this permission notice shall be included in all
//* copies or substantial portions of the Software.
//*
//* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//* SOFTWARE.
//*
//********************************************************************************
//*
//* Module     : max7219.cpp
//* Author     : Jonathan Evans
//* Description: MAX7219 LED Display Driver
//*
//* The MAX7219/MAX7221 are compact, serial input/output common-cathode display drivers that interface
//* microprocessors (µPs) to 7-segment numeric LED displays of up to 8 digits, bar-graph displays, or 64
//* individual LEDs
//* Datasheet  : https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf
//*
//* Library Description
//*
//*  - This library implements the 7-segment numeric LED display of 8 digits
//*  - The host communicates with the MAX7219 using three signals: CLK (pin 10), CS (pin 11), DIN (pin 12).
//*  - Pins can be configured in max7219.h
//*  - The MAX7219 is a SPI interface
//*  - This library uses the bitbang method for communication with the MAX7219
//*
//* Usage
//*
//* Three methods are exposed for use:
//*
//*  1. Begin
//*  This method initializes communication, takes the display out of test mode, clears the screen and sets intensity.
//*  Intensity is set at maximum but can be configured in max7219.h
//*
//*  2. DisplayChar(Digit, Value, DP)
//*  This method displays a single value (character) in position DIGIT (0=right most digit, 7=left most digit)
//*
//*  3. DisplayText(Text, Justify)
//*  This method displays a text string (Text) either right justified (Justify=0) or left justified (Justify=1)
//*/
//
//#include "max7219.h"
//
//MAX7219::MAX7219(void)
//{
//    pinMode(MAX_DIN, OUTPUT);
//    pinMode(MAX_CS, OUTPUT);
//    pinMode(MAX_CLK, OUTPUT);
//}
//
//void MAX7219::MAX7219_ShutdownStart(void)
//{
//    MAX7219_Write(REG_SHUTDOWN, 0);
//}
//
//void MAX7219::MAX7219_DisplayTestStart(void)
//{
//    MAX7219_Write(REG_DISPLAY_TEST, 1);
//}
//
//void MAX7219::Clear(void)
//{
//
//    for (int i = 0; i < 8; i++)
//    {
//        MAX7219_Write(i + 1, 0x00);
//    }
//}
//
//void MAX7219::MAX7219_DisplayTestStop(void)
//{
//    MAX7219_Write(REG_DISPLAY_TEST, 0);
//}
//
//void MAX7219::MAX7219_SetBrightness(char brightness)
//{
//    brightness &= 0x0f;
//    MAX7219_Write(REG_INTENSITY, brightness);
//}
//
//unsigned char MAX7219::MAX7219_LookupCode(char character, unsigned int dp)
//{
//    int i;
//    unsigned int d = 0;
//    if (dp) d = 1;
//    if (character >= 35 && character <= 44)
//    {
//        character += 13;
//        d = 1;
//    }
//    for (i = 0; MAX7219_Font[i].ascii; i++)
//        if (character == MAX7219_Font[i].ascii)
//        {
//            if (d)
//            {
//                d = MAX7219_Font[i].segs;
//                d |= (1 << 7);
//                return (d);
//            } else
//            {
//                return MAX7219_Font[i].segs;
//            }
//        }
//
//    return 0;
//}
//
//void MAX7219::DisplayText(char *text, int justify)
//{
//    int decimal[16];
//    char trimStr[16] = "";
//    int x, y = 0;
//    int s;
//
//    s = strlen(text);
//    if (s > 16) s = 16;
//    for (x = 0; x < s; x++)
//    {
//        if (text[x] == '.')
//        {
//            decimal[y - 1] = 1;
//        } else
//        {
//            trimStr[y] = text[x];
//            decimal[y] = 0;
//            y++;
//        }
//    }
//    if (y > 8) y = 8;
//    for (x = 0; x < y; x++)
//    {
//        if (justify == 0)
//            DisplayChar((int) (y - x + 7 - y), trimStr[x], decimal[x]);
//        if (justify == 1)
//        {
//            DisplayChar((int) (y - x + 7 - y - (8 - y)), trimStr[x], decimal[x]);
//
//        }
//    }
//}
//
//void MAX7219::MAX7219_Write(volatile byte opcode, volatile byte data)
//{
//    digitalWrite(MAX_CS, LOW);
//    shiftOut(MAX_DIN, MAX_CLK, MSBFIRST, opcode);
//    shiftOut(MAX_DIN, MAX_CLK, MSBFIRST, data);
//    digitalWrite(MAX_CS, HIGH);
//}
//
//void MAX7219::DisplayChar(int digit, char value, bool dp)
//{
//    MAX7219_Write(digit + 1, MAX7219_LookupCode(value, dp));
//}
//
//void MAX7219::MAX7219_ShutdownStop(void)
//{
//    MAX7219::MAX7219_Write(REG_SHUTDOWN, 1);
//}
//
//void MAX7219::Begin()
//{
//    digitalWrite(MAX_CS, HIGH);
//    MAX7219_Write(REG_SCAN_LIMIT, 7);
//    MAX7219_Write(REG_DECODE, 0x00);
//    MAX7219_ShutdownStop();
//    MAX7219_DisplayTestStop();
//    Clear();
//    MAX7219_SetBrightness(INTENSITY_MAX);
//}
//#endif