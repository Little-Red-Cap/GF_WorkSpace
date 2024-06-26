#ifndef GF_RTOS_FLAG_GROUP_H
#define GF_RTOS_FLAG_GROUP_H

#include "event.h"

typedef struct {
    tEvent event;
    uint32_t flags;
} flag_group_t;

typedef struct {
    uint32_t flags;
    uint32_t taskCount;
}tFlagGroupInfo;

#define TFLAGGROUP_CLEAR     (0x0 << 0)
#define TFLAGGROUP_SET       (0x1 << 0)
#define TFLAGGROUP_ANY       (0x0 << 1)
#define TFLAGGROUP_ALL       (0x1 << 1)

#define TFLAGGROUP_SET_ALL   (TFLAGGROUP_SET | TFLAGGROUP_ALL)
#define TFLAGGROUP_SET_ANY   (TFLAGGROUP_SET | TFLAGGROUP_ANY)
#define TFLAGGROUP_CLEAR_ALL (TFLAGGROUP_CLEAR | TFLAGGROUP_ALL)
#define TFLAGGROUP_CLEAR_ANY (TFLAGGROUP_CLEAR | TFLAGGROUP_ANY)

#define TFLAGGROUP_CONSUME   (0x1 << 7)


void flag_group_init(flag_group_t *group, uint32_t flags);
static uint32_t tFlagGroupCheckAndConsume(flag_group_t *group, uint32_t waitType, uint32_t *flags);
uint32_t tFlagGroup_wait(flag_group_t *group, uint32_t waitType, uint32_t requestFlag, uint32_t *resultFlag, uint32_t timeout);
uint32_t tFlagGroupNoWaitGet(flag_group_t *group, uint32_t waitType, uint32_t requestFlag, uint32_t *resultFlag);
void tFlagGroupNotify(flag_group_t *group, uint8_t isSet, uint32_t flags);
uint32_t tFlagGroupDestroy(flag_group_t *group);
void tFlagGroupGetInfo(flag_group_t *group, tFlagGroupInfo *info);


#endif // GF_RTOS_FLAG_GROUP_H
