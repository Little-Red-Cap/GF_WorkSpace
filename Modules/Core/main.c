
#include "os.h"
#include "bitmap.h"
#include "list.h"
#include "task.h"

#if 1

//TCB *taskList[PRIORITY_BITMAP_SIZE];   // 任务列表
tList taskList[OS_PRIORITY_BITMAP_SIZE];   // 任务列表
TCB *currentTask = ((void *)0); // 当前任务指针
TCB *nextTask = ((void *)0); // 下一个任务指针
TCB *idleTask = ((void *)0);
uint32_t taskCount = 0;    // 任务数量

uint32_t idleCount = 0;
uint32_t idleMaxCount;
uint32_t tickCount = 0;    // 系统时钟计数器
#if OS_ENABLE_CPU_USAGE
static void initCpuUsageState(void);
static void cpuUsageSyncWithSysTick(void);
static void checkCpuUsage(void);
#endif


uint8_t schedLockCount = 0;

bitmap_t taskPriorityBitmap; // 任务优先级位图

tList tTaskDelayList;   // 延时任务链表
void tTaskDelayListInit(void) {
    tList_init(&tTaskDelayList);
}

void tTimeTaskWait(TCB *task, uint32_t ticks)
{
    task->delayTicks = ticks;
    tList_add_last(&tTaskDelayList, &task->delayNode);
    task->state |= OS_TASK_STATE_DELAY;
}

void tTimeTaskWakeUp(TCB *task)
{
    tList_remove(&tTaskDelayList, &task->delayNode);
    task->state &= ~OS_TASK_STATE_DELAY;
}


void tTaskSchedReady(TCB *task) {
//    taskList[task->priority] = task;
    tList_add_first(&taskList[task->priority], &task->linkNode);
    bitmap_set(&taskPriorityBitmap, task->priority);
}

void tTaskSchedUnready(TCB *task) {
    tList_remove(&taskList[task->priority], &task->linkNode);
    if (tList_count(&taskList[task->priority]) == 0) {
        bitmap_clear(&taskPriorityBitmap, task->priority);
    }
}


void tTaskRequestDelete(TCB *task)
{
    uint32_t status = taskEnterCritical();
    task->requestDeleteFlag = 1;
    taskExitCritical(status);
}

uint8_t tTaskIsRequestedDelete()
{
    uint32_t status    = taskEnterCritical();
    uint8_t deleteFlag = currentTask->requestDeleteFlag;
    taskExitCritical(status);
    return deleteFlag;
}

void tTaskSchedRemove(TCB *task)
{
    tList_remove(&taskList[task->priority], &task->linkNode);
    if (tList_count(&taskList[task->priority]) == 0) {
        bitmap_clear(&taskPriorityBitmap, task->priority);
    }
}

void tTimeTaskRemove(TCB *task)
{
    tList_remove(&tTaskDelayList, &task->delayNode);
    task->state &= ~OS_TASK_STATE_DELAY;
}

void tTaskSetCleanCallFunc(TCB *task, void (*clean)(void *), void *cleanParam)
{
    task->clean      = clean;
    task->cleanParam = cleanParam;
}

void tTaskDeleteSelf(void)
{
    uint32_t status = taskEnterCritical();

    tTaskSchedRemove(currentTask);

    if (currentTask->clean)
        currentTask->clean(currentTask->cleanParam);

    schedule();

    taskExitCritical(status);
}

void tTaskForceDelete(TCB *task)
{
    uint32_t status = taskEnterCritical();

    if (task->state & OS_TASK_STATE_DELAY)    // 如果任务处于延时状态，则从延时任务链表中移除
        tTimeTaskRemove(task);
    else if (!(task->state & OS_TASK_STATE_SUSPEND))    // 如果任务没有挂起，说明任务处于就绪状态，则从就绪任务链表中移除
        tTaskSchedRemove(task);

    if (task->clean)         // 如果任务有清理函数，则调用清理函数
        task->clean(task->cleanParam);

    if (task == currentTask)    // 如果删除的任务是当前任务，则切换到下一个任务
        schedule();

    taskExitCritical(status);
}







TCB *taskHigherPrioReady(void) {
    uint32_t highestPrio = bitmap_getFirstSet(&taskPriorityBitmap);
//    return taskList[highestPrio];
    tNode *node = tList_first(&taskList[highestPrio]);
    return (TCB *)tNodeParent(node, TCB, linkNode);
}

uint32_t taskEnterCritical(void) {
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    return primask;
}

void taskExitCritical(uint32_t primask) {
    __set_PRIMASK(primask);
}


void schedule(void) {
    uint32_t status = taskEnterCritical();

    if (schedLockCount > 0) {  // 如果已经被锁定，则不进行调度
        taskExitCritical(status);
        return;
    }
    // 找到优先级最高的任务
    TCB *tempTask = taskHigherPrioReady();
    if (tempTask != currentTask)
    {
        nextTask = tempTask;
#if OS_ENABLE_HOOKS
        hooks_task_switch(currentTask, nextTask);
#endif
//        tTaskSwitch();
        // 触发PendSV中断来进行任务切换
        SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
    }

    taskExitCritical(status);
}


void taskSchedInit(void) {
    schedLockCount = 0;
    bitmap_init(&taskPriorityBitmap);

    for (int i = 0; i < OS_PRIORITY_BITMAP_SIZE; i++) {
        tList_init(&taskList[i]);
    }
}

void taskSchedDisable(void) {
    uint32_t status = taskEnterCritical();

    if (schedLockCount < 255)
        schedLockCount++;


    taskExitCritical(status);
}

void taskSchedEnable(void) {
    uint32_t status = taskEnterCritical();

    if (schedLockCount > 0)
        if (--schedLockCount == 0)
            schedule();

    taskExitCritical(status);
}

#if OS_ENABLE_CPU_USAGE
static float cpuUsage = 0.0f;
static uint32_t enableCpuUsageState = 0;

static void initCpuUsageState(void) {
    idleCount = 0;
    idleMaxCount = 0;
    cpuUsage = 0.0f;
    enableCpuUsageState = 0;
}

static void cpuUsageSyncWithSysTick(void) {
    while (enableCpuUsageState == 0) {
        ;
    }
}

static void checkCpuUsage(void) {
    if (enableCpuUsageState == 0) {
        enableCpuUsageState = 1;
        idleCount = 0;
        return;
    }
    if (tickCount == TICKS_PER_SECOND) {
        idleMaxCount = idleCount;
        idleCount = 0;
        taskSchedEnable();
    } else if (tickCount % TICKS_PER_SECOND == 0) {
        cpuUsage = 100 - (idleCount * 100.0f / idleMaxCount);
        idleCount = 0;
    }
}

float tCpuUsageGet()
{
    uint32_t status = taskEnterCritical();
    float usage = cpuUsage;
    taskExitCritical(status);
    return usage;
}
#endif

void tTimeTickInit(void) {
    tickCount = 0;
}

__attribute__((noreturn)) void idleTaskEntry(void *param)
{
    taskSchedDisable();
    init_app();
#if OS_ENABLE_TIMER
    tTimerInitTask();
#endif
    setSysTickPeriod(OS_SYSTICK_MS);
#if OS_ENABLE_CPU_USAGE
    cpuUsageSyncWithSysTick();
#endif
    for ((void)param;;) {
        uint32_t status = taskEnterCritical();
        idleCount++;
        taskExitCritical(status);
#if OS_ENABLE_HOOKS
        hooks_cpu_idle();
#endif
    }
}

void taskSystemTickHandler(void) {
    uint32_t status = taskEnterCritical();

    // 检查所有延时任务是否超时， 如果deadline不为0则减1，否则任务就绪，则唤醒任务
    for (tNode *node = tTaskDelayList.head.nextNode; node != &(tTaskDelayList.head); node = node->nextNode) {
        TCB *task = (TCB *)tNodeParent(node, TCB, delayNode);
        if (--task->delayTicks == 0) {
            // 如果任务还处于等待事件状态，则将其从事件等待队列中唤醒
            if (task->waitEvent)
                tEvent_remove_task(task, (void *)0, tErrorTimeout); // 此时，消息为空，等待结果为超时

            // 将任务从延时任务链表中移除
            tTimeTaskWakeUp(task);
            // 将任务恢复到就绪状态
            tTaskSchedReady(task);
        }
    }

    if (--currentTask->slice == 0) {
        if (tList_count(&taskList[currentTask->priority]) > 0) {
            // 任务时间片用完，将任务从就绪任务链表中移除
            tList_remove_first(&taskList[currentTask->priority]);
            tList_add_last(&taskList[currentTask->priority], &currentTask->linkNode);

            currentTask->slice = OS_SLICE_MAX;
        }
    }
    tickCount++;
#if OS_ENABLE_CPU_USAGE
    checkCpuUsage();
#endif
    taskExitCritical(status);
#if OS_ENABLE_TIMER
    tTimerModuleTickNotify();
#endif
#if OS_ENABLE_HOOKS
    hooks_sys_tick();
#endif
    schedule();
}

void taskDelay(uint32_t ticks)
{
    uint32_t status = taskEnterCritical();

    tTimeTaskWait(currentTask, ticks);
    tTaskSchedUnready(currentTask);

    taskExitCritical(status);
    schedule();
}




TCB idle_tcb;
uint32_t idle_stack[OS_IDLETASK_STACK_SIZE];



int main(void) {
    taskSchedInit();    // 初始化调度锁
    tTaskDelayListInit(); // 初始化延时任务链表

#if OS_ENABLE_TIMER
    tTimerModuleInit();  // 初始化定时器模块
    tTimeTickInit();
#endif
#if OS_ENABLE_CPU_USAGE
    initCpuUsageState();
#endif

    initTaskStack(&idle_tcb, idleTaskEntry, idle_stack, STACK_SIZE, OS_PRIORITY_BITMAP_SIZE - 1);
    idleTask = &idle_tcb;

    // 设置初始任务
    nextTask = taskHigherPrioReady();

    __set_PSP(0);                        // 启动第一个任务，使用PSP，进入线程模式
    SCB->SHPR[10] = 0xFF;                // 最高优先级
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk; // 触发PendSV中断
    return 0;
}
#endif

