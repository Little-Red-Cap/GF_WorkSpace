#ifndef GF_RTOS_MUTEX_H
#define GF_RTOS_MUTEX_H
#include "event.h"

typedef struct {
    tEvent event;
    TCB *owner;
    uint32_t lock_count;
    uint32_t owner_Original_priority;
} mutex_t;

typedef struct {
    uint32_t task_count;         // number of tasks waiting for the mutex
    uint32_t owner_priority;     // first priority of the task that owned the mutex
    uint32_t inheritor_priority; // current priority of the task that is waiting for the mutex
    uint32_t lock_count;         // number of times the mutex has been locked
    TCB *owner;                  // current owner of the mutex
} mutex_info_t;

void mutex_init(mutex_t *mutex);
uint32_t mutex_wait(mutex_t *mutex, uint32_t timeout);
uint32_t mutexNoWaitGet(mutex_t *mutex);
uint32_t mutex_notify(mutex_t *mutex);
uint32_t mutex_destroy(mutex_t *mutex);
void mutex_getInfo(mutex_t *mutex, mutex_info_t *info);

#endif // GF_RTOS_MUTEX_H
