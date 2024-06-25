#ifndef GF_RTOS_TASK_H
#define GF_RTOS_TASK_H

#include <stdint.h>
#include "list.h"
//#include "event.h"

#define OS_TASK_STATE_READY     0
#define OS_TASK_STATE_DESTORYED (1 << 1)
#define OS_TASK_STATE_DELAY     (1 << 2)
#define OS_TASK_STATE_SUSPEND   (1 << 3)

#define OS_TASK_WAIT_MASK       (0xFF << 16)    // 把高16位用于等待标志位
//#define OS_TASK_WAIT_EVENT      (1 << 16)      // 等待事件
//#define OS_TASK_WAIT_MESSAGE    (1 << 17)      // 等待消息
//#define OS_TASK_WAIT_TIMEOUT    (1 << 18)      // 等待超时
//#define OS_TASK_WAIT_SIGNAL     (1 << 19)      // 等待信号
//#define OS_TASK_WAIT_QUEUE      (1 << 20)      // 等待队列
//#define OS_TASK_WAIT_SEMAPHORE  (1 << 21)      // 等待信号量
//#define OS_TASK_WAIT_MUTEX      (1 << 22)      // 等待互斥锁
//#define OS_TASK_WAIT_COND       (1 << 23)      // 等待条件变量


// 注意：前项引用声明，只适用于定义 指针变量
struct _tEvent; // defined in event.h

typedef uint32_t tTaskStack;

typedef struct tcb {
    uint32_t *stackPointer;     // 任务堆栈指针
    uint32_t *stackBase;        // 任务堆栈基地址
    uint32_t stackSize;         // 任务堆栈大小
    struct tcb *next;           // 指向下一个任务的指针

    tNode linkNode;             // 任务链表节点
    tNode delayNode;            // 延时任务链表节点

    void (*clean) (void *param);// 任务清理函数指针W
    void *cleanParam;           // 任务清理函数参数
    uint8_t requestDeleteFlag;  // 任务删除标志

//    tEvent *waitEvent;          // 等待事件指针
    struct _tEvent *waitEvent;          // 等待事件指针
    void *eventMsg;             // 等待事件消息指针
    uint32_t waitEventResult;   // 等待事件结果

    uint32_t waitFlagsType;     // 等待标志位类型
    uint32_t eventFlags;        // 事件标志位

    uint32_t delayTicks;        // 延时计数器
    uint32_t priority;          // 任务优先级
    uint32_t state;             // 任务状态
    uint32_t slice;             // 任务时间片
    uint32_t suspendCount;      // 任务挂起计数器
} TCB;



typedef struct _os_task_inof{
    uint32_t delayTicks;    // 延时计数器
    uint32_t priority;      // 任务优先级
    uint32_t state;         // 任务状态
    uint32_t suspendCount;   // 任务挂起计数器
    uint32_t slice;         // 任务时间片
    uint32_t stackSize;
    uint32_t stackFree;
} os_task_info_t;


void initTaskStack(TCB *task, void (*taskFunction)(void *), uint32_t *stack, uint32_t stackSize, uint32_t priority);
// 任务挂起与唤醒相关函数
void tTaskSuspend(TCB *task);
void tTaskWakeUp(TCB *task);
// 任务删除相关函数
void tTaskSetCleanCallFunc(TCB *task, void (*clean)(void *), void *cleanParam);
void tTaskForceDelete(TCB *task);
void tTaskRequestDelete(TCB *task);
uint8_t tTaskIsRequestedDelete();
void tTaskDeleteSelf(void);
// 任务信息获取相关函数
void tTaskGetInfo(TCB *task, os_task_info_t *info);

#endif //GF_RTOS_TASK_H
