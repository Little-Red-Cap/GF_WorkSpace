#ifndef GF_RTOS_OS_CONFIG_H
#define GF_RTOS_OS_CONFIG_H

#define OS_PRIORITY_BITMAP_SIZE 32
#define OS_SLICE_MAX            10
#define OS_IDLETASK_STACK_SIZE  128

#define OS_TIMERTASK_STACK_SIZE 128
#define OS_TIMERTASK_PERIOD     1

#define OS_SYSTICK_MS           10

#define OS_ENABLE_SEMAPHORE     1
#define OS_ENABLE_MBOX          1
#define OS_ENABLE_MUTEX         1
#define OS_ENABLE_TIMER         1
#define OS_ENABLE_FLAGGROUP     1
#define OS_ENABLE_MEM_BLOCK     1
#define OS_ENABLE_CPU_USAGE     1
#define OS_ENABLE_HOOKS         1

#define OS_ENABLE_QUEUE         0
#define OS_ENABLE_EVENT         0
#define OS_ENABLE_TASK          1
#define OS_ENABLE_IDLE          1

#define OS_ENABLE_DEBUG         0

#endif // GF_RTOS_OS_CONFIG_H
