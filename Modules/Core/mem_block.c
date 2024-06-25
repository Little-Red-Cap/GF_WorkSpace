#include "mem_block.h"
#include "os.h"

#if OS_ENABLE_MEM_BLOCK

void MemBlockInit(tMenBlock *memBlock, void *memStart, uint32_t blockSize, uint32_t maxBlockCount)
{
    if (blockSize < sizeof(tNode))
        return;

    uint8_t *memBlockStart = (uint8_t *)memStart;
    uint8_t *memBlockEnd   = memBlockStart + (maxBlockCount * blockSize);

    tEvent_init(&memBlock->event, tEventTypeMemBlock);
    memBlock->memStart      = memStart;
    memBlock->blockSize     = blockSize;
    memBlock->maxBlockCount = maxBlockCount;
    tList_init(&memBlock->blockList);

    // add all memory blocks to the list
    while (memBlockStart < memBlockEnd) {
        tNode_init((tNode *)memBlockStart);
        tList_add_last(&memBlock->blockList, (tNode *)memBlockStart);
        memBlockStart += blockSize; // pointer move to the next memory block
    }
}

uint32_t tMenBlockWait(tMenBlock *memBlock, uint8_t **mem, uint32_t timeout)
{
    uint32_t status = taskEnterCritical();
    // check if there are any free memory blocks
    if (tList_count(&memBlock->blockList) > 0) {
        *mem = (uint8_t *)tList_remove_first(&memBlock->blockList);
        taskExitCritical(status);
        return tErrorNoError;
    } else {
        // insert current task to the list of waiting tasks
        tEvent_wait(&memBlock->event, currentTask, (void *)0, tEventTypeMemBlock, timeout);
        taskExitCritical(status);
        schedule();
        *mem = currentTask->eventMsg;
        return currentTask->waitEventResult;
    }
}

uint32_t tMenBlockNoWaitGet(tMenBlock *memBlock, uint8_t **mem)
{
    uint32_t status = taskEnterCritical();
    // check if there are any free memory blocks
    if (tList_count(&memBlock->blockList) > 0) {
        *mem = (uint8_t *)tList_remove_first(&memBlock->blockList);
        taskExitCritical(status);
        return tErrorNoError;
    } else {
        taskExitCritical(status);
        return tErrorResourceNotAvailable;
    }
}

void tMenBlockNotify(tMenBlock *memBlock, uint8_t *mem)
{
    uint32_t status = taskEnterCritical();
    if (tEvent_wait_count(&memBlock->event) > 0) {
        // if there are waiting tasks, wake up the first one
        TCB *task = tEvent_wakeup(&memBlock->event, (void *)0, tErrorNoError);
        // if the waiting task has higher priority than the current task, schedule
        if (task->priority < currentTask->priority)
            schedule();
    } else {
        // if not waiting tasks, add memory block back to the list
        tList_add_last(&memBlock->blockList, (tNode *)mem);
    }
    taskExitCritical(status);
}

uint32_t tMenBlockDestroy(tMenBlock *memBlock)
{
    uint32_t status = taskEnterCritical();
    uint32_t count  = tEvent_remove_all(&memBlock->event, (void *)0, tErrorDelete);
    taskExitCritical(status);
    if (count > 0)
        schedule();
    return count;
}

void tMemBlockGetInfo(tMenBlock *memBlock, tMemBlockInfo *info)
{
    uint32_t status     = taskEnterCritical();
    info->count         = tList_count(&memBlock->blockList);
    info->maxBlockCount = memBlock->maxBlockCount;
    info->blockSize     = memBlock->blockSize;
    info->taskCount     = tEvent_wait_count(&memBlock->event);
    taskExitCritical(status);
}

#endif // OS_ENABLE_MEM_BLOCK
