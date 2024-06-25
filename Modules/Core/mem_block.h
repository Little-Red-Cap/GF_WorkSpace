#ifndef RTOS_BY_JOHO_MEM_BLOCK_H
#define RTOS_BY_JOHO_MEM_BLOCK_H

#include "event.h"

typedef struct {
    tEvent event;           // event object
    void *memStart;         // start address of memory block
    uint32_t blockSize;     // size of memory block
    uint32_t maxBlockCount; // maximum number of memory blocks
    tList blockList;        // list of memory blocks
} tMenBlock;

typedef struct {
    uint32_t count;         // number of memory blocks
    uint32_t blockSize;     // size of each memory block
    uint32_t maxBlockCount; // maximum number of memory blocks
    uint32_t taskCount;     // waiting task count
} tMemBlockInfo;

void MemBlockInit(tMenBlock *memBlock, void *memStart, uint32_t blockSize, uint32_t maxBlockCount);
uint32_t tMenBlockWait(tMenBlock *memBlock, uint8_t **mem, uint32_t timeout);
uint32_t tMenBlockNoWaitGet(tMenBlock *memBlock, uint8_t **mem);
void tMenBlockNotify(tMenBlock *memBlock, uint8_t *mem);
uint32_t tMenBlockDestroy(tMenBlock *memBlock);
void tMemBlockGetInfo(tMenBlock *memBlock, tMemBlockInfo *info);

#endif // RTOS_BY_JOHO_MEM_BLOCK_H
