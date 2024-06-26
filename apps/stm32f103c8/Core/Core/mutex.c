#include "mutex.h"
#include "os.h"

#if OS_ENABLE_MUTEX

void mutex_init(mutex_t *mutex)
{
    tEvent_init(&mutex->event, tEventTypeMutex);

    mutex->owner                   = (TCB *)0;
    mutex->lock_count              = 0;
    mutex->owner_Original_priority = OS_PRIORITY_BITMAP_SIZE;
}

uint32_t mutex_wait(mutex_t *mutex, uint32_t timeout)
{
    uint32_t status = taskEnterCritical();
    if (mutex->lock_count <= 0) {
        mutex->owner = currentTask;
        mutex->lock_count++;
        mutex->owner_Original_priority = currentTask->priority;

        taskExitCritical(status);
        return tErrorNoError;
    } else {
        if (mutex->owner == currentTask) {
            mutex->lock_count++;
            taskExitCritical(status);
            return tErrorNoError;
        } else {
            if (currentTask->priority < mutex->owner->priority) {
                TCB *owner = mutex->owner;
                if (owner->state == OS_TASK_STATE_READY) {
                    tTaskSchedUnready(owner);
                    owner->priority = currentTask->priority;
                    tTaskSchedReady(owner);
                } else {
                    owner->priority = currentTask->priority;
                }
            }

            tEvent_wait(&mutex->event, currentTask, (void *)0, tEventTypeMutex, timeout);
            taskExitCritical(status);

            schedule();
            return currentTask->waitEventResult;
        }
    }
}

uint32_t mutexNoWaitGet(mutex_t *mutex)
{
    uint32_t status = taskEnterCritical();
    if (mutex->lock_count <= 0) {
        mutex->owner = currentTask;
        mutex->lock_count++;
        mutex->owner_Original_priority = currentTask->priority;

        taskExitCritical(status);
        return tErrorNoError;
    } else {
        if (mutex->owner == currentTask) {
            mutex->lock_count++;
            taskExitCritical(status);
            return tErrorNoError;
        }
        taskExitCritical(status);
        return tErrorResourceNotAvailable;
    }
}

uint32_t mutex_notify(mutex_t *mutex)
{
    uint32_t status = taskEnterCritical();
    if (mutex->lock_count <= 0) {
        taskExitCritical(status);
        return tErrorNoError;
    }
    if (mutex->owner != currentTask) {
        taskExitCritical(status);
        return tErrorOwner;
    }
    if (--mutex->lock_count > 0) {
        taskExitCritical(status);
        return tErrorNoError;
    }
    if (mutex->owner_Original_priority != mutex->owner->priority) {
        if (mutex->owner->state == OS_TASK_STATE_READY) {
            tTaskSchedUnready(mutex->owner);
            mutex->owner->priority = mutex->owner_Original_priority;
            tTaskSchedReady(mutex->owner);
        } else {
            currentTask->priority = mutex->owner_Original_priority;
        }
    }
    if (tEvent_wait_count(&mutex->event) > 0) {
        // warning: this is not a safe operation, as the task may have been
        TCB *task    = tEvent_wakeup(&mutex->event, (void *)0, tErrorNoError);
        mutex->owner = task;
        mutex->lock_count++;
        mutex->owner_Original_priority = task->priority;
        if (task->priority < currentTask->priority)
            schedule();
    }
    taskExitCritical(status);
    return tErrorNoError;
}

uint32_t mutex_destroy(mutex_t *mutex)
{
    uint32_t status = taskEnterCritical();
    uint32_t count  = 0;

    if (mutex->lock_count > 0) {
        if (mutex->owner_Original_priority != mutex->owner->priority) {
            // restore priority
            if (mutex->owner->state == OS_TASK_STATE_READY) {
                // task is readying, set priority
                tTaskSchedUnready(mutex->owner);
                mutex->owner->priority = mutex->owner_Original_priority;
//                currentTask->priority = mutex->owner_Original_priority;
                tTaskSchedReady(mutex->owner);
            } else {
                // other state, just set priority
                mutex->owner->priority = mutex->owner_Original_priority;
//                currentTask->priority = mutex->owner_Original_priority;
            }
        }
        count = tEvent_remove_all(&mutex->event, (void *)0, tErrorDelete);
        if (count > 0) {
            schedule();
        }
    }
    taskExitCritical(status);
    return count;
}

void mutex_getInfo(mutex_t *mutex, mutex_info_t *info)
{
    uint32_t status          = taskEnterCritical();
    info->task_count         = tEvent_wait_count(&mutex->event);
    info->owner_priority     = mutex->owner_Original_priority;
    info->owner              = mutex->owner;
    info->lock_count         = mutex->lock_count;
    if (mutex->owner != (TCB *)0) {  // has owner
        info->inheritor_priority = mutex->owner->priority;
    } else {    // no owner, set inheritor priority to error value
        info->inheritor_priority = OS_PRIORITY_BITMAP_SIZE;
    }
    taskExitCritical(status);
}

#endif
