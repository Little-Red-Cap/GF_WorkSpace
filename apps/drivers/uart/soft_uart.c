#include "soft_uart.h"

//https://mp.weixin.qq.com/s/R5gyJweFJRv7SKRVsNa-Dg

//以9600波特率为例，每一位的持续时间约为104us.
//因此在CubeMX中我们可以设置定时器的触发时间为104us。

//int num = 0;
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//
//    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,Data[num++]);
//    if(num == 10)
//    {
//        num = 0;
//        HAL_TIM_Base_Stop_IT(&htim1);//关闭定时器
//    }
//}

//void MyPrintf(int s)
//{
//    Data[0] = 0;
//    for(unsigned char i = 0;i<8;i++)
//    {
//        if(s & 0x01){
//            Data[i+1] = 1;
//        }
//        else{
//            Data[i+1] = 0;
//        }
//        s = s >> 1;
//    }
//    Data[9] = 1;
//    HAL_TIM_Base_Start_IT(&htim1);
//}


//int fputc(int ch, FILE *f) {
//    // 发送单个字符
//    MyPrintf(ch);
//    HAL_Delay(50);//稍微延时，等待发送完成
//    // 返回发送的字符
//    return ch;
//}