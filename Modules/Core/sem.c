#include "sem.h"
#include "os.h"

//#if OS_USING_SEMAPHORE
#if OS_ENABLE_SEMAPHORE

void tSem_init(tSem *sem, uint32_t start_count, uint32_t max_count)
{
    tEvent_init(&sem->event, tEventTypeSem);
    sem->maxCount = max_count;

    if (max_count == 0)
        sem->maxCount = 0;
    else
        sem->count = start_count > max_count ? max_count : start_count;
}

uint32_t tSem_wait(tSem *sem, uint32_t timeout)
{
    uint32_t status = taskEnterCritical();
    // 如果信号量大于0，则直接消耗一个信号量，然后正常退出
    if (sem->count > 0) {
        --sem->count;
        taskExitCritical(status);
        return tErrorNoError;
    } else {
        // 否则，将当前任务挂起，并等待信号量的到来
        // 将任务插入事件队列中，等待信号量的到来
        tEvent_wait(&sem->event, currentTask, (void *)0, tEventTypeSem, timeout);
        taskExitCritical(status);

        schedule();
        // 当超时或者计数可用时，执行会返回这里，然后取出等待结果
        return currentTask->waitEventResult;
    }
}

void tSem_notify(tSem *sem)
{
    uint32_t status = taskEnterCritical();
    // 检查是否有任务在等待信号量
    if (tEvent_wait_count(&sem->event) > 0) {
        // 如果有任务在等待信号量，则唤醒一个任务（最先等待）
        TCB *task = tEvent_wakeup(&sem->event, (void *)0, tErrorNoError);

        // 如果唤醒的任务优先级高于当前任务，则重新调度
        if (task->priority > currentTask->priority)
            schedule();
    } else {
        ++sem->count; // 没有任务在等待信号量，则增加信号量计数

        // 如果信号量计数超过最大值，则将信号量计数设置为最大值
        if ((sem->maxCount != 0) && (sem->count > sem->maxCount))
            sem->count = sem->maxCount;
    }
    taskExitCritical(status);
}

uint32_t tSem_no_wait_get(tSem *sem)
{
    uint32_t status = taskEnterCritical();
    // 如果信号量大于0，则直接消耗一个信号量，然后正常退出
    if (sem->count > 0) {
        --sem->count;
        taskExitCritical(status);
        return tErrorNoError;
    } else {
        // 否则，返回资源不可用
        taskExitCritical(status);
        return tErrorResourceNotAvailable;
    }
}

uint32_t tSem_destroy(tSem *sem)
{
    uint32_t status = taskEnterCritical();
    // 清理事件控制块中的任务
    uint32_t count = tEvent_remove_all(&sem->event, (void *)0, tErrorDelete);
    taskExitCritical(status);

    if (count > 0)  // 清理过程可能有任务就绪，需要重新调度
        schedule();
    return count;
}

void tSem_get_info(tSem *sem, tSemInfo *info)
{
    uint32_t status = taskEnterCritical();

    // 复制信号量信息
    info->maxCount = sem->maxCount;
    info->count = sem->count;
    info->taskCount = tEvent_wait_count(&sem->event);

    taskExitCritical(status);
}

#endif // OS_USING_SEMAPHORE
