#include "cpu.h"
#include "os.h"

#include <stdio.h>
#include <stdint.h>
#include "ARMCM3.h"
//#include "D:\Keil5\ARM\Packs\ARM\CMSIS\6.1.0\CMSIS\Core\Include\\core_cm3.h"






void setSysTickPeriod(uint32_t ms) {
    SysTick->LOAD = ms * SystemCoreClock / 1000 - 1;
    NVIC_SetPriority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void) {
    taskSystemTickHandler();
}











// printf重定向到软件仿真的输出接口
__attribute__((used)) int fputc(int ch, FILE *f)
{
    (void)f;
    ITM_SendChar(ch);
    return 0;
}

__attribute__((used)) __attribute__((noreturn))
void hard_fault_handler_c(const uint32_t *stack_address) {
    uint32_t r0 = stack_address[0];
    uint32_t r1 = stack_address[1];
    uint32_t r2 = stack_address[2];
    uint32_t r3 = stack_address[3];
    uint32_t r12 = stack_address[4];
    uint32_t lr = stack_address[5];
    uint32_t pc = stack_address[6];
    uint32_t psr = stack_address[7];

    // 调试输出
    printf("Hard Fault: \n");
    printf("R0  = 0x%08X\n", r0);
    printf("R1  = 0x%08X\n", r1);
    printf("R2  = 0x%08X\n", r2);
    printf("R3  = 0x%08X\n", r3);
    printf("R12 = 0x%08X\n", r12);
    printf("LR  = 0x%08X\n", lr);
    printf("PC  = 0x%08X\n", pc);
    printf("PSR = 0x%08X\n", psr);

    // 死循环等待调试
    while (1);
}

__attribute__((naked)) void HardFault_Handler(void) {
    __asm volatile (
        "TST lr, #4 \n"                      // Test EXC_RETURN bit 2
        "ITE EQ \n"                          // If-Then-Else (Equal)
        "MRSEQ r0, MSP \n"                   // Main Stack Pointer
        "MRSNE r0, PSP \n"                   // Process Stack Pointer
        "B hard_fault_handler_c \n"          // Branch to C handler
    );
}

