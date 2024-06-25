#include "flag_group.h"
#include "os.h"

#if OS_ENABLE_FLAGGROUP

void flag_group_init(flag_group_t *group, uint32_t flags)
{
    tEvent_init(&group->event, tEventTypeFlagGroup);
    group->flags = flags;
}

static uint32_t tFlagGroupCheckAndConsume(flag_group_t *group, uint32_t waitType, uint32_t *flags)
{
    uint32_t srcFlag   = *flags;
    uint32_t isSet     = waitType & TFLAGGROUP_SET;
    uint32_t isAll     = waitType & TFLAGGROUP_ALL;
    uint32_t isConsume = waitType & TFLAGGROUP_CONSUME;

    uint32_t calFlag = isSet ? (group->flags & srcFlag) : (~group->flags & srcFlag);

    if (((isAll != 0) && (calFlag == srcFlag)) || ((isAll == 0) && (calFlag != 0))) {
        if (isConsume) {
            if (isSet)
                group->flags &= ~srcFlag;
            else
                group->flags |= srcFlag;
        }
        *flags = calFlag;
        return tErrorNoError;
    }
    *flags = calFlag;
    return tErrorResourceNotAvailable;
}

uint32_t tFlagGroup_wait(flag_group_t *group, uint32_t waitType, uint32_t requestFlag, uint32_t *resultFlag, uint32_t timeout)
{
    uint32_t result = 0;
    uint32_t flags  = requestFlag;

    uint32_t status = taskEnterCritical();
    result          = tFlagGroupCheckAndConsume(group, waitType, &flags);
    if (result != tErrorNoError) {
        currentTask->waitFlagsType = waitType;
        currentTask->eventFlags    = requestFlag;
        tEvent_wait(&group->event, currentTask, (void *)0, tEventTypeFlagGroup, timeout);

        taskExitCritical(status);
        schedule();
        *resultFlag = currentTask->eventFlags;
        result      = currentTask->waitEventResult;
    } else {
        *resultFlag = flags;
        taskExitCritical(status);
    }

    return result;
}

uint32_t tFlagGroupNoWaitGet(flag_group_t *group, uint32_t waitType, uint32_t requestFlag, uint32_t *resultFlag)
{
    uint32_t flags = requestFlag;

    uint32_t status = taskEnterCritical();
    uint32_t result = tFlagGroupCheckAndConsume(group, waitType, &flags);
    taskExitCritical(status);

    *resultFlag = flags;
    return status;
}

void tFlagGroupNotify(flag_group_t *group, uint8_t isSet, uint32_t flags)
{
    tList *waitList;
    tNode *node;
    tNode *nextNode;
    uint8_t sched = 0;

    uint32_t status = taskEnterCritical();

    if (isSet)
        group->flags |= flags;
    else
        group->flags &= ~flags;

    waitList = &group->event.waitList;
    for (node = waitList->head.nextNode; node != &waitList->head; node = nextNode) {
        uint32_t result = 0;
        TCB *task       = tNodeParent(node, TCB, linkNode);
        uint32_t flags  = task->eventFlags;
        nextNode        = node->nextNode;

        result = tFlagGroupCheckAndConsume(group, task->waitFlagsType, &flags);
        if (result == tErrorNoError) {
            task->eventFlags = flags;
            tEvent_wakeupTask(&group->event, task, (void *)0, tErrorNoError);
            sched = 1;
        }
    }
    if (sched)
        schedule();

    taskExitCritical(status);
}

uint32_t tFlagGroupDestroy(flag_group_t *group)
{
    uint32_t status = taskEnterCritical();
    uint32_t count = tEvent_remove_all(&group->event, (void *)0, tErrorDelete);
    taskExitCritical(status);
    if (count > 0)
        schedule();
    return count;
}

void tFlagGroupGetInfo(flag_group_t *group, tFlagGroupInfo *info)
{
    uint32_t status = taskEnterCritical();
    info->flags = group->flags;
    info->taskCount = tEvent_wait_count(&group->event);
    taskExitCritical(status);
}

#endif
