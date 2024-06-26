#include "os.h"
#include "main.h"

TCB task1_tcb;
TCB task2_tcb;
TCB task3_tcb;
TCB task4_tcb;

uint32_t taskStacks[MAX_TASKS][STACK_SIZE];

int task1Flag = 0;
__attribute__((noreturn)) void task1(void *param)
{
    for((void) param;;) {
        HAL_GPIO_TogglePin(led_inline_GPIO_Port, led_inline_Pin);
//        HAL_Delay(500);
        taskDelay(100);
//        task1Flag = 1;
//        taskDelay(1);
//        task1Flag = 0;
//        taskDelay(1);
    }
}

int task2Flag = 0;
__attribute__((noreturn)) void task2(void *param)
{
    for((void) param;;) {
        task2Flag = 1;
        taskDelay(1);
        task2Flag = 0;
        taskDelay(1);
    }
}

int task3Flag = 0;
__attribute__((noreturn)) void task3(void *param)
{
    for((void) param;;) {
        task3Flag = 1;
        taskDelay(1);
        task3Flag = 0;
        taskDelay(1);
    }
}

int task4Flag = 0;
__attribute__((noreturn)) void task4(void *param)
{
    for((void) param;;) {
        task4Flag = 1;
        taskDelay(1);
        task4Flag = 0;
        taskDelay(1);
    }
}

void init_app(void)
{
    // 初始化任务堆栈
    initTaskStack(&task1_tcb, task1, taskStacks[0], STACK_SIZE, 0);
    initTaskStack(&task2_tcb, task2, taskStacks[1], STACK_SIZE, 1);
    initTaskStack(&task3_tcb, task3, taskStacks[2], STACK_SIZE, 1);
    initTaskStack(&task4_tcb, task4, taskStacks[3], STACK_SIZE, 1);
}
