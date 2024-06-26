#include "sg90.h"
#if SG90_ENABLE
/*
    TIM1_ETR     PA12
    TIM1_CH1     PA8            TIM2_CH1_ETR PA0            TIM3_CH1     PA6            TIM4_CH1     PB6
    TIM1_CH2     PA9            TIM2_CH2     PA1            TIM3_CH2     PA7            TIM4_CH2     PB7
    TIM1_CH3     PA10           TIM2_CH3     PA2            TIM3_CH3     PB0            TIM4_CH3     PB8
    TIM1_CH4     PA11           TIM2_CH4     PA3            TIM3_CH4     PB1            TIM4_CH4     PB9

    SG90 Servo Motor Device (180)
        high pulse T1                           cycle 20ms:
        —— —— —— ——
        |           |                           high lave T1: 0.5ms     move 0°
        |           |                           high lave T1:   1ms     move 45°
        |           |                           high lave T1: 1.5ms     move 90°
        |           |                           high lave T1:   2ms     move 135°
                    —— —— —— —— —— ——           high lave T1: 2.5ms     move 180°
        --------cycle T2为20ms--------

    T1: 1.5ms     move: 90°     position:   0°    arg:1850
    T1: 0.5ms     move: 0°      position: -90°    arg:1945
    T1:   1ms     move: 45°     position: -45°    arg:1900
    T1: 1.5ms     move: 90°     position:   0°    arg:1850
    T1:   2ms     move: 135°    position: +45°    arg:1800
    T1: 2.5ms     move: 180°    position: +90°    arg:1750

    -90°    TIM_SetCompare1(TIMx,1945);   // (2000-1945)/2000*20ms=0.5ms
    -45°    TIM_SetCompare1(TIMx,1900);   // (2000-1900)/2000*20ms=1ms
    0°      TIM_SetCompare1(TIMx,1850);   // (2000-1850)/2000*20ms=1.5ms
    +45°    TIM_SetCompare1(TIMx,1800);   // (2000-1800)/2000*20ms=2ms
    +90°    TIM_SetCompare1(TIMx,1750);   // (2000-1750)/2000*20ms=2.5ms
 * */
#include "tim.h"

void servo_motor_init()
{
    MX_TIM1_Init();
    MX_TIM4_Init();
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
}

#endif