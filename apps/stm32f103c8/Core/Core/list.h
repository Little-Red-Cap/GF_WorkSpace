#ifndef GF_RTOS_LIST_H
#define GF_RTOS_LIST_H

#include <stdint.h>
typedef struct _tNode {
    struct _tNode *prevNode;
    struct _tNode *nextNode;
}tNode;

//typedef struct  _tTask {
//    tNode delayNode;
//
//    uint32_t priority;
//    uint32_t state;
//}tTask;

typedef struct _tList {
    tNode head;
    tNode tail;
    uint32_t count;
}tList;

#define tNodeParent(node, parent, name) \
    ((parent *)((uint8_t *)(node) - (uint8_t *)&((parent *)0)->name))



void tNode_init(tNode *node);
void tList_init(tList *list);
uint32_t tList_count(tList *list);
tNode *tList_first(tList *list);
tNode *tList_last(tList *list);
tNode *tList_next(tNode *list, tNode *node);
tNode *tList_prev(tNode *list, tNode *node);
void tList_remove_all(tList *list);
void tList_add_first(tList *list, tNode *node);
void tList_add_last(tList *list, tNode *node);
tNode *tList_remove_first(tList *list);
void tList_insert_after(tList *list, tNode *nodeAfter, tNode *nodeToInsert);
void tList_remove(tList *list, tNode *node);


#endif //GF_RTOS_LIST_H
