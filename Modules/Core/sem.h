#ifndef GF_RTOS_SEM_H
#define GF_RTOS_SEM_H
#include "event.h"

typedef struct {
    tEvent event;       // 特地放到起始位置，以实现信号量同时作为事件使用
    uint32_t count;     // current count
    uint32_t maxCount;  // maximum count
}tSem;

typedef struct {
    uint32_t count;     // current count
    uint32_t maxCount;  // maximum count
    uint32_t taskCount; // number of tasks waiting on the semaphore
} tSemInfo;

void tSem_init(tSem *sem, uint32_t start_count, uint32_t max_count); // 初始化信号量
uint32_t tSem_wait(tSem *sem, uint32_t timeout);                     // 等待信号量，超时时间单位为ms
void tSem_notify(tSem *sem);                                         // 通知等待者
uint32_t tSem_no_wait_get(tSem *sem);                                // 获取信号量当前值，不等待
void tSem_get_info(tSem *sem, tSemInfo *info);                       // 获取信号量信息
uint32_t tSem_destroy(tSem *sem);                                    // 销毁信号量

#endif //GF_RTOS_SEM_H
