#include "event.h"
#include "os.h"

void tEvent_init(tEvent *event, tEventType type)
{
    //    event->type = type;
    event->type = tEventTypeUnknown;
    tList_init(&event->waitList);
}

void tEvent_wait(tEvent *event, TCB *task, void *msg, uint32_t state, uint32_t timeout)
{
    uint32_t status = taskEnterCritical();

    // high 16 bits of state is the event type used for waiting
    task->state |= state << 16;            // 标记任务状态为等待某种事件的状态
    task->waitEvent = event;               // 记录等待的事件结构
    task->eventMsg  = msg;                 // 记录事件的消息
                                           // 因有时需要接收消息，所以需要接收区
    task->waitEventResult = tErrorNoError; // 清空等待事件的结果

    tTaskSchedUnready(task); // 标记任务为不可调度

    // 加入等待队列
    tList_add_last(&event->waitList, &task->linkNode);

    if (timeout)
        tTimeTaskWait(task, timeout);

    taskExitCritical(status);
}

TCB *tEvent_wakeup(tEvent *event, void *msg, uint32_t result)
{
    tNode *node;
    TCB *task       = (TCB *)0;
    uint32_t status = taskEnterCritical();

    // 遍历等待队列，找到第一个状态为等待事件状态的任务
    if ((node = tList_remove_first(&event->waitList)) != (tNode *)0) {
        // 转换为任务结构
        task = (TCB *)tNodeParent(node, TCB, linkNode);

        task->waitEventResult = result;
        task->eventMsg        = msg;
        task->waitEvent       = (tEvent *)0;
        task->state &= ~OS_TASK_WAIT_MASK;

        if (task->delayTicks != 0)
            tTimeTaskWakeUp(task);

        tTaskSchedReady(task); // 标记任务为可调度
    }

    taskExitCritical(status);
    return task;
}

TCB *tEvent_wakeupTask(tEvent *event, TCB *task, void *msg, uint32_t result)
{
    uint32_t status = taskEnterCritical();

    tList_remove(&event->waitList, &task->linkNode);

    task->waitEventResult = result;
    task->eventMsg        = msg;
    task->waitEvent       = (tEvent *)0;
    task->state &= ~OS_TASK_WAIT_MASK;

    if (task->delayTicks != 0)
        tTimeTaskWakeUp(task);

    tTaskSchedReady(task); // 标记任务为可调度

    taskExitCritical(status);
    return task;
}

void tEvent_remove_task(TCB *task, void *msg, uint32_t result)
{
    uint32_t status = taskEnterCritical();

    // 移除任务自身的等待事件
    tList_remove(&task->waitEvent->waitList, &task->linkNode); // 注意，这里没有检查waitEvent是否为空，因为从事件中移除，事件结构本身也会被销毁，所以这里不用检查

    task->waitEventResult = result;
    task->eventMsg        = msg;
    task->waitEvent       = (tEvent *)0;
    task->state &= ~OS_TASK_WAIT_MASK;

    taskExitCritical(status);
}

uint32_t tEvent_remove_all(tEvent *event, void *msg, uint32_t result)
{
    tNode *node;
    TCB *task;
    uint32_t status = taskEnterCritical();

    uint32_t count = tList_count(&event->waitList); // 获取等待队列中任务的数量

    // 遍历等待队列，找到所有状态为等待事件状态的任务
    while ((node = tList_remove_first(&event->waitList)) != (tNode *)0) {
        // 转换为任务结构
        task = (TCB *)tNodeParent(node, TCB, linkNode);

        task->waitEventResult = result;
        task->eventMsg        = msg;
        task->waitEvent       = (tEvent *)0;
        task->state &= ~OS_TASK_WAIT_MASK;

        if (task->delayTicks != 0)
            tTimeTaskWakeUp(task);

        tTaskSchedReady(task); // 标记任务为可调度
    }

    taskExitCritical(status);
    return count;
}

uint32_t tEvent_wait_count(tEvent *event)
{
    uint32_t status = taskEnterCritical();
    uint32_t count  = tList_count(&event->waitList);
    taskExitCritical(status);
    return count;
}
