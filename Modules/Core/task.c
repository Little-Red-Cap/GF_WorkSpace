#include "task.h"
#include "os.h"
#include <string.h>


void initTaskStack(TCB *task, void (*taskFunction)(void *), uint32_t *stack, uint32_t stackSize, uint32_t priority)
{
    // 设置任务堆栈指针和基地址
    task->stackPointer = stack + stackSize - 1; // Cortex-M from highest address to lowest address (stack grows down)
    task->stackBase = stack;    // stack top
    task->stackSize = stackSize;
    memset(stack, 0, stackSize);

    // 初始化堆栈
    uint32_t *stackTop = task->stackPointer; // stackTop = stackBase + stackSize / sizeof(tTaskStack);

    *(--stackTop) = (1U << 24);          // xPSR (Thumb bit set)
    *(--stackTop) = (uint32_t)taskFunction; // PC
    *(--stackTop) = 0xFFFFFFFD;          // LR (Return to Thread mode, using PSP)
    *(--stackTop) = 0x12;                // R12
    *(--stackTop) = 0x3;                 // R3
    *(--stackTop) = 0x2;                 // R2
    *(--stackTop) = 0x1;                 // R1
    *(--stackTop) = 0x0;                 // R0

    // R4-R11 (callee-saved registers)
    *(--stackTop) = 0x11;                // R11
    *(--stackTop) = 0x10;                // R10
    *(--stackTop) = 0x9;                 // R9
    *(--stackTop) = 0x8;                 // R8
    *(--stackTop) = 0x7;                 // R7
    *(--stackTop) = 0x6;                 // R6
    *(--stackTop) = 0x5;                 // R5
    *(--stackTop) = 0x4;                 // R4

    task->stackPointer = stackTop;
    task->delayTicks = 0;
    task->priority = priority;
    task->state = OS_TASK_STATE_READY;
    task->slice = OS_SLICE_MAX;
    task->clean = (void (*)(void *)) 0;
    task->cleanParam = (void *) 0;
    task->requestDeleteFlag = 0;
    task->suspendCount = 0;

    tNode_init(&task->delayNode);
    tNode_init(&task->linkNode);

    tTaskSchedReady(task);

#if OS_ENABLE_HOOKS
    hooks_task_init(task);
#endif
}

void tTaskSuspend(TCB *task)
{
    uint32_t status = taskEnterCritical();

    // 不允许对已经进入延时状态的任务挂起
    if (!(task->state & OS_TASK_STATE_DELAY))
    {
        if (++task->suspendCount <= 1) {
            task->state |= OS_TASK_STATE_SUSPEND;

            tTaskSchedUnready(task);

            if (task == currentTask)
                schedule();
        }
    }

    taskExitCritical(status);
}

void tTaskWakeUp(TCB *task)
{
    uint32_t status = taskEnterCritical();

    // 检查任务是否已经挂起
    if (task->state & OS_TASK_STATE_SUSPEND)
    {
        if (--task->suspendCount == 0) {
            task->state &= ~OS_TASK_STATE_SUSPEND;  // 清除挂起标志

            tTaskSchedReady(task);  // 将任务放回就绪队列中

            schedule(); // 唤醒过程中，可能有更高优先级的任务就绪，因此需要重新调度
        }
    }

    taskExitCritical(status);
}

void tTaskGetInfo(TCB *task, os_task_info_t *info)
{
    uint32_t status = taskEnterCritical();

    info->delayTicks = task->delayTicks;
    info->priority = task->priority;
    info->state = task->state;
    info->slice = task->slice;
    info->suspendCount = task->suspendCount;
    info->stackSize = task->stackSize;

    info->stackFree = 0;

    uint32_t *stackEnd = task->stackBase;

    while ((*stackEnd++ == 0) && (stackEnd <= task->stackBase + task->stackSize / sizeof(uint32_t))) {
        info->stackFree++;
    }
    info->stackFree *= sizeof(tTaskStack);

    taskExitCritical(status);
}
