#include "os.h"
#include "main.h"

TCB task1_tcb;
TCB task2_tcb;
TCB task3_tcb;
TCB task4_tcb;


uint32_t taskStacks[MAX_TASKS][STACK_SIZE];

__attribute__((noreturn)) void task1(void *param)
{
    for((void) param;;) {
        HAL_GPIO_TogglePin(led_inline_GPIO_Port, led_inline_Pin);
//        HAL_Delay(500);
        taskDelay(100);
    }
}

__attribute__((noreturn)) void task2(void *param)
{
    int task2Flag = 0;
    for((void) param;;) {
        task2Flag = 1;
        taskDelay(1);
        task2Flag = 0;
        taskDelay(1);
    }
}

__attribute__((noreturn)) void task3(void *param)
{
    int task3Flag = 0;
    for((void) param;;) {
        task3Flag = 1;
        taskDelay(1);
        task3Flag = 0;
        taskDelay(1);
    }
}

__attribute__((noreturn)) void task4(void *param)
{
    int task4Flag = 0;
    for((void) param;;) {
        task4Flag = 1;
        taskDelay(1);
        task4Flag = 0;
        taskDelay(1);
    }
}

void init_app(void)
{
    initTaskStack(&task1_tcb, task1, taskStacks[0], STACK_SIZE, 0);
    initTaskStack(&task2_tcb, task2, taskStacks[1], STACK_SIZE, 1);
    initTaskStack(&task3_tcb, task3, taskStacks[2], STACK_SIZE, 1);
    initTaskStack(&task4_tcb, task4, taskStacks[3], STACK_SIZE, 1);
}
