#include <cstdio>

void test1()
{
    // 设变量x为float型且已赋值，则以下语句中能将x的数值保留到小数点后2位，并将第三位四舍五入的是？
    float value = 1.41421356f;
//    float value = 2.232;
//    const float value = 2.235;
    float x = value;
    x = (x * 100) + 0.5 / 100.0;
    printf("%f\n", x);

    x = value;
    x = (x * 100 + 0.5) / 100.0;
    printf("%f\n", x);

    x = value;
    x = (int) (x * 100 + 0.5) / 100.0;
    printf("%f\n", x);

    x = value;
    x = (x / 100 + 0.5) * 100.0;
    printf("%f\n", x);

    // 答案：第三种
    // 这里有个隐藏的坑：四舍六入五成双，即小数点后两位的数字会有两种可能性
}

int main()
{
    test1();
    __asm__ volatile("":: :"memory");
    return 0;
}

// arm studio
//https://developer.arm.com/Tools%20and%20Software/Arm%20Development%20Studio
// arm-clang
//https://developer.arm.com/Tools%20and%20Software/Arm%20Compiler%20for%20Embedded
// arm-none-eabi-gcc
//https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain
