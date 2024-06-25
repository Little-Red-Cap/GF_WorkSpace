#include "switch.h"
#include "ARMCM3.h"

#define NVIC_INT_CTRL   0xE000ED04
#define NVIC_PENDSVSET  0x10000000
#define NVIC_SYSPRI2    0xE000ED22
#define NVIC_PENDSV_PRI 0x000000FF

#define MEM32(addr) (*(volatile unsigned long*)(addr))
#define MEM16(addr) (*(volatile uint16_t*)(addr))
#define MEM8(addr)  (*(volatile uint8_t*)(addr))

void PendSV_Handler(void)
{
    __asm volatile (
        "MRS R0, PSP \n"                // 获取进程堆栈指针
        "CBZ R0, PendSVHandler_nosave \n" // 如果R0为0，跳转到无保存分支
        "STMDB R0!, {R4-R11} \n"        // 保存R4-R11寄存器到堆栈
        "LDR R1, =currentTask \n"       // 加载currentTask的地址
        "LDR R1, [R1] \n"               // 加载currentTask的值
        "STR R0, [R1] \n"               // 保存PSP到当前任务堆栈指针

        "PendSVHandler_nosave: \n"
        "LDR R0, =currentTask \n"       // 加载currentTask的地址
        "LDR R1, =nextTask \n"          // 加载nextTask的地址
        "LDR R2, [R1] \n"               // 加载nextTask的值
        "STR R2, [R0] \n"               // 将nextTask更新为currentTask

        "LDR R0, [R2] \n"               // 加载nextTask的堆栈指针
        "LDMIA R0!, {R4-R11} \n"        // 恢复R4-R11寄存器

        "MSR PSP, R0 \n"                // 恢复PSP
        "ORR LR, LR, #0x04 \n"          // 使用PSP返回
        "BX LR \n"                      // 返回
    );
}

//void tTaskRunFirst()
//{
//    __set_PSP(0);

//    MEM8(NVIC_SYSPRI2)   = NVIC_PENDSV_PRI;
//    MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;
//}

//void tTaskSwitch()
//{
//    MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;
//}
