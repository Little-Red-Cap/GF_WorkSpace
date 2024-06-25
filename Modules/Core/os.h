#ifndef GF_RTOS_OS_H
#define GF_RTOS_OS_H

#include <stdint.h>
#include "D:\Keil5\ARM\Packs\ARM\CMSIS\5.9.0\Device\ARM\ARMCM3\Include\ARMCM3.h"
#include "D:\Keil5\ARM\Packs\ARM\CMSIS\6.1.0\CMSIS\Core\Include\m-profile\cmsis_armclang_m.h"
#include "D:\Keil5\ARM\Packs\ARM\CMSIS\6.1.0\CMSIS\Core\Include\\core_cm3.h"
#include "os_config.h"
#include "list.h"
#include "event.h"
#include "task.h"
#include "mutex.h"
#include "sem.h"
#include "mbox.h"
#include "mem_block.h"
#include "flag_group.h"
#include "timer.h"
#include "hooks.h"

#define TICKS_PER_SECOND (1000 / OS_SYSTICK_MS)

typedef enum {
    tErrorNoError = 0,
    tErrorTimeout,
    tErrorResourceNotAvailable,
    tErrorDelete,
    tErrorResourceFull,
    tErrorOwner,
}tError;




#define MAX_TASKS 5
#define STACK_SIZE 128

extern TCB *currentTask; // 当前任务指针
extern TCB *nextTask; // 下一个任务指针

uint32_t taskEnterCritical(void);
void taskExitCritical(uint32_t primask);

void tTaskDelayListInit(void);
void tTimeTaskWait(TCB *task, uint32_t ticks);
void tTimeTaskWakeUp(TCB *task);

void tTaskSchedReady(TCB *task);
void tTaskSchedUnready(TCB *task);

void tTimeTaskRemove(TCB *task);
void tTaskSchedRemove(TCB *task);

float tCpuUsageGet();

void schedule(void);

void taskSystemTickHandler(void);
void setSysTickPeriod(uint32_t ms);
void taskDelay(uint32_t ticks);




void init_app(void);


#endif //GF_RTOS_OS_H
