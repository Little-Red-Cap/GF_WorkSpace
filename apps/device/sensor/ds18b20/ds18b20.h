#ifndef DS18B20_H
#define DS18B20_H

#if 0
#ifdef __cplusplus
extern "C" {
#endif
#define DS18B20_IO_IN()  {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=8<<0;}
#define DS18B20_IO_OUT() {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=3<<0;}
#define	DS18B20_DQ_OUT PAout(0)
#define	DS18B20_DQ_IN  PAin(0)

unsigned char DS18B20_Init(void);
short DS18B20_Get_Temp(void);
void DS18B20_Start(void);
void DS18B20_Write_Byte(unsigned char dat);
unsigned char DS18B20_Read_Byte(void);
unsigned char DS18B20_Read_Bit(void);
unsigned char DS18B20_Check(void);
void DS18B20_Rst(void);

#ifdef __cplusplus
}
#endif
#endif
#endif // DS18B20_H
