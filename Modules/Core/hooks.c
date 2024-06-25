#include "hooks.h"

#if OS_ENABLE_HOOKS
void hooks_cpu_idle(void)
{

}

void hooks_sys_tick(void)
{

}

void hooks_task_switch(TCB *old_task, TCB *new_task)
{

}

void hooks_task_init(TCB *task)
{

}

#endif // OS_ENABLE_HOOKS
