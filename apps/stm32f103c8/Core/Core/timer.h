#ifndef GF_RTOS_TIMER_H
#define GF_RTOS_TIMER_H
#include "list.h"

typedef enum {
    tTimerCreated,
    TTimerStarted,
    tTimerRunning,
    tTimerStopped,
    tTimerDestroyed,
} tTimerState;

typedef struct {
    tNode linkNode;
    uint32_t startDelayTime; // first start time of the timer
    uint32_t durationTime;
    uint32_t delayTime;
    void (*callback)(void *);
    void *arg;
    uint32_t config;
    tTimerState state;
} timer_t;

typedef struct {
    uint32_t startDelayTime; // first start time of the timer
    uint32_t durationTime;
    void (*callback)(void *);
    void *arg;
    uint32_t config;
    tTimerState state;
}timer_info_t;

#define TIMER_CONFIG_TYPE_HARD (1 << 0)
#define TIMER_CONFIG_TYPE_SOFT (0 << 0)

void tTimerModuleInit(void);
void timer_init(timer_t *timer, uint32_t delayTicks, uint32_t durationTicks,void (*callback)(void*), void *arg, uint32_t config);
void timer_start(timer_t *timer);
void timer_stop(timer_t *timer);
void tTimerModuleTickNotify(void);  // call this function in timer interrupt handler
void timer_destroy(timer_t *timer);
void timer_get_Info(timer_t *timer, timer_info_t *info);
void tTimerInitTask(void);

#endif // GF_RTOS_TIMER_H
