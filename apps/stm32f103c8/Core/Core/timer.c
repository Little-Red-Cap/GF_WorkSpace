#include "timer.h"
#include "os.h"

#if OS_ENABLE_TIMER

static tList tTimerSoftList;
static tList tTimerHardList;
static tSem tTimerProtectSem;
static tSem tTimerTickSem;

static TCB tTimerTask;
static uint32_t tTimerTaskStack[OS_TIMERTASK_STACK_SIZE];

void timer_init(timer_t *timer, uint32_t delayTicks, uint32_t durationTicks, void (*callback)(void *), void *arg, uint32_t config)
{
    tNode_init(&timer->linkNode);

    timer->startDelayTime = delayTicks;
    timer->durationTime   = durationTicks;
    timer->callback       = callback;
    timer->arg            = arg;
    timer->config         = config;

    if (delayTicks == 0)
        timer->delayTime = durationTicks;
    else
        timer->delayTime = timer->startDelayTime;

    timer->state = tTimerCreated;
}

void timer_start(timer_t *timer)
{
    switch (timer->state) {
        case tTimerCreated:
        case tTimerStopped:
            timer->state     = TTimerStarted;
            timer->delayTime = timer->startDelayTime ? timer->startDelayTime : timer->durationTime;

            if (timer->config & TIMER_CONFIG_TYPE_HARD) {
                uint32_t status = taskEnterCritical();
//                tList_add_last(&tTimerHardList, &timer->linkNode);
                tList_add_first(&tTimerHardList, &timer->linkNode);
                taskExitCritical(status);
            } else {
                tSem_wait(&tTimerProtectSem, 0);
                tList_add_last(&tTimerSoftList, &timer->linkNode);
                tSem_notify(&tTimerProtectSem);
            }
            break;
        default:
            break;
    }
}

void timer_stop(timer_t *timer)
{
    switch (timer->state) {
        case TTimerStarted:
        case tTimerRunning:
            if (timer->config & TIMER_CONFIG_TYPE_HARD) {
                uint32_t status = taskEnterCritical();
                tList_remove(&tTimerHardList, &timer->linkNode);
                taskExitCritical(status);
            } else {
                tSem_wait(&tTimerProtectSem, 0);
                tList_remove(&tTimerSoftList, &timer->linkNode);
                tSem_notify(&tTimerProtectSem);
            }
            timer->state = tTimerStopped;
            break;
        default:
            break;
    }
}

static void tTimerCallFuncList(tList *timerList)
{
    tNode *node;
    for(node = timerList->head.nextNode; node!=&(timerList->head); node = node->nextNode)
    {
        timer_t *timer = tNodeParent(node, timer_t, linkNode);
        if (timer->delayTime == 0 || (--timer->delayTime == 0)) {
            timer->state = tTimerRunning;
            timer->callback(timer->arg);
            timer->state = TTimerStarted;

            if (timer->durationTime > 0) {
                timer->delayTime = timer->durationTime;
            } else {
                tList_remove(timerList, &timer->linkNode);
                timer->state = tTimerStopped;
            }
        }
    }
}

__attribute__((noreturn))
static void tTimerSoftTask(void *arg)
{
    for (;;) {
        tSem_wait(&tTimerTickSem, 0);
        tSem_wait(&tTimerProtectSem, 0);
        tTimerCallFuncList(&tTimerSoftList);
        tSem_notify(&tTimerProtectSem);
    }
}

void tTimerModuleInit(void)
{
    tList_init(&tTimerHardList);
    tList_init(&tTimerSoftList);
    tSem_init(&tTimerProtectSem, 1, 1);
    tSem_init(&tTimerTickSem, 0, 0);
}

void tTimerInitTask(void)
{
#if OS_TIMERTASK_PERIOD >= (OS_PRIORITY_BITMAP_SIZE - 1)
#error "OS_TIMERTASK_PERIOD is too large\n must be less than (OS_PRIORITY_BITMAP_SIZE - 1)"
#endif
    //    initTaskStack(&tTimerTask, tTimerSoftTask, (void *)0, OS_TIMERTASK_PERIOD, tTimerTaskStack);
    initTaskStack(&tTimerTask, tTimerSoftTask, tTimerTaskStack, OS_TIMERTASK_STACK_SIZE, OS_TIMERTASK_PERIOD);
}

// call this function in timer interrupt handler
void tTimerModuleTickNotify(void)
{
    uint32_t status = taskEnterCritical();

    tTimerCallFuncList(&tTimerHardList);

    taskExitCritical(status);

    tSem_notify(&tTimerTickSem);
}

void timer_destroy(timer_t *timer)
{
    timer_stop(timer);
    timer->state = tTimerDestroyed;
}

void timer_get_Info(timer_t *timer, timer_info_t *info)
{
    uint32_t status = taskEnterCritical();

    info->startDelayTime = timer->startDelayTime;
    info->durationTime   = timer->durationTime;
    info->callback       = timer->callback;
    info->arg            = timer->arg;
    info->config         = timer->config;
    info->state          = timer->state;

    taskExitCritical(status);
}

#endif // OS_ENABLE_TIMER
