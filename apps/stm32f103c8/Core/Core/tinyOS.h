#ifndef TINYOS_H
#define TINYOS_H

#include <stdint.h>
#define NULL ((void *)0)



typedef uint32_t tTaskStack;

typedef struct {
    tTaskStack *stack;
//    uint32_t delayTicks;
}tTask;

extern tTask *currentTask;
extern tTask *nextTask;

void tTaskRunFirst(void);
void tTaskSwitch(void);
#endif

