#ifndef GF_RTOS_HOOKS_H
#define GF_RTOS_HOOKS_H
#include "os.h"

void hooks_cpu_idle(void);
void hooks_sys_tick(void);
void hooks_task_switch(TCB *old_task, TCB *new_task);
void hooks_task_init(TCB *task);

#endif //GF_RTOS_HOOKS_H
