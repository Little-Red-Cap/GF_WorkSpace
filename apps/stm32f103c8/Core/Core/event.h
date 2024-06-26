#ifndef GF_RTOS_EVENT_H
#define GF_RTOS_EVENT_H

#include "list.h"
#include "task.h"

typedef enum {
    tEventTypeUnknown = 0,

    tEventTypeSem,
    tEventTypeMbox,
    tEventTypeMutex,
    tEventTypeMemBlock,
    tEventTypeFlagGroup,

    tEventTypeSignal,
    tEventTypeMessage,
    tEventTypeTimer,
}tEventType ;

typedef struct _tEvent {
    tEventType type;
    tList waitList;
} tEvent;


// 事件相关函数
void tEvent_init(tEvent *event, tEventType type);
void tEvent_wait(tEvent *event, TCB *task, void *msg, uint32_t state, uint32_t timeout);
TCB * tEvent_wakeup(tEvent *event, void *msg, uint32_t result);
TCB * tEvent_wakeupTask(tEvent *event,TCB *task, void *msg, uint32_t result);
void tEvent_remove_task(TCB *task, void *msg, uint32_t result);
uint32_t tEvent_remove_all(tEvent *event, void *msg, uint32_t result);
uint32_t tEvent_wait_count(tEvent *event);
//void event_signal(tEvent *event);
//void event_wait(tEvent *event, tList *waitList);
//void event_notify(tList *waitList);

#endif //GF_RTOS_EVENT_H
