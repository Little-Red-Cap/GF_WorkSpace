#include "mbox.h"
#include "os.h"

#if OS_ENABLE_MBOX

void tMboxInit(tMbox *mbox, void **msgBuffer, uint32_t maxCount)
{
    tEvent_init(&mbox->event, tEventTypeMbox);

    mbox->msgBuffer = msgBuffer;
    mbox->maxCount  = maxCount;
    mbox->readPos   = 0;
    mbox->writePos  = 0;
    mbox->count     = 0;
}

uint32_t tMboxWait(tMbox *mbox, void **msg, uint32_t timeout)
{
    uint32_t status = taskEnterCritical();

    if (mbox->count > 0) {
        --mbox->count;
        //        *msg = mbox->msgBuffer[mbox->readPos];
        //        mbox->readPos = (mbox->readPos + 1) % mbox->maxCount;
        // 同时读取索引前移，避免读写冲突，如果超出边界则回环
        *msg = mbox->msgBuffer[mbox->readPos++];
        if (mbox->readPos >= mbox->maxCount) {
            mbox->readPos = 0;
        }
        taskExitCritical(status);
        return tErrorNoError;
    } else {
        // 等待消息，将任务插入事件等待队列中
        tEvent_wait(&mbox->event, currentTask, (void *)0, tEventTypeSem, timeout);
        taskExitCritical(status);
        schedule();
        // 再切换回来后，从当前任务的eventMsg中取出消息
        *msg = currentTask->eventMsg;
        // 取出等待消息的结果
        return currentTask->waitEventResult;
    }
}

uint32_t tMboxNotify(tMbox *mbox, void *msg, uint32_t notifyOption)
{
    uint32_t status = taskEnterCritical();
    // 检查是否有任务等待消息
    if (tEvent_wait_count(&mbox->event) > 0) {
        // 事件等待队列中有任务，则唤醒一个任务（最先等待的任务）
        TCB *task = tEvent_wakeup(&mbox->event, (void *)msg, tErrorNoError);
        // 如果这个任务的优先级高于当前任务，则需要重新调度切换到这个任务
        if (task->priority < currentTask->priority)
            schedule();
    } else {
        // 没有任务等待消息，则直接写入消息
        // 如果消息队列已满，则返回资源已满错误
        if (mbox->count >= mbox->maxCount) {
            taskExitCritical(status);
            return tErrorResourceFull;
        }
        // 如果没有满，则写入消息

        if (notifyOption & tMBOXSendFront) {    // 插入到缓冲区前面
            if (mbox->readPos <= 0)
                mbox->readPos = mbox->maxCount - 1; // 回环，调整到缓冲区末尾
            else
                --mbox->readPos;
            mbox->msgBuffer[mbox->readPos] = msg;
        } else {                                // 插入到缓冲区后面 (Normal)
            //            mbox->msgBuffer[mbox->writePos] = msg;
            //            mbox->writePos = (mbox->writePos + 1) % mbox->maxCount;
            mbox->msgBuffer[mbox->writePos++] = msg;
            if (mbox->writePos >= mbox->maxCount)
                mbox->writePos = 0;
        }
        ++mbox->count;
    }
    taskExitCritical(status);
    return tErrorNoError;
}

void tMboxFlush(tMbox *mbox)
{
    uint32_t status = taskEnterCritical();
    // 如果队列中有任务等待，说明邮箱已经是空的，不需要再清空
    if (tEvent_wait_count(&mbox->event) == 0) {
        mbox->readPos  = 0;
        mbox->writePos = 0;
        mbox->count    = 0;
    }
    taskExitCritical(status);
}

uint32_t tMboxDestroy(tMbox *mbox)
{
    uint32_t status = taskEnterCritical();
    uint32_t count = tEvent_remove_all(&mbox->event, (void *)0, tErrorDelete);
    taskExitCritical(status);
    if (count > 0)  // 清理时可能有任务就绪，需要重新调度
        schedule();
    return count;
}

void tMboxGetInof(tMbox *mbox, tMboxInfo *info)
{
    uint32_t status = taskEnterCritical();
    info->maxCount = mbox->maxCount;
    info->count    = mbox->count;
    info->taskCount  = tEvent_wait_count(&mbox->event);
    taskExitCritical(status);
}

#endif
