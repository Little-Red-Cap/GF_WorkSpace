#ifndef GF_RTOS_MBOX_H
#define GF_RTOS_MBOX_H

#include "event.h"

#define tMBOXSendNormal 0x00
#define tMBOXSendFront  0x01

typedef struct {
    tEvent event;      // must be the first element in the structure
    uint32_t count;    // current message count
    uint32_t readPos;  // read pointer index
    uint32_t writePos; // write pointer index
    uint32_t maxCount; // max message count
    void **msgBuffer;  // message buffer
} tMbox;

typedef struct {
    uint32_t count;    // current message count
    uint32_t maxCount; // max message count
    uint32_t taskCount; // task count waiting for message
}tMboxInfo;

void tMboxInit(tMbox *mbox, void **msgBuffer, uint32_t maxCount);
uint32_t tMboxWait(tMbox *mbox, void **msg, uint32_t timeout);
uint32_t tMboxNotify(tMbox *mbox, void *msg, uint32_t notifyOption);
void tMboxFlush(tMbox *mbox);
uint32_t tMboxDestroy(tMbox *mbox);

#endif // GF_RTOS_MBOX_H
