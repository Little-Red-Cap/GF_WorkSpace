#include "list.h"

#define firstNode head.nextNode
#define lastNode head.prevNode

void tNode_init(tNode *node) {
    node->nextNode = node;
    node->prevNode = node;
}

void tList_init(tList *list) {
    list->head.nextNode = &list->head;
    list->head.prevNode = &list->head;
    list->count = 0;
}

uint32_t tList_count(tList *list) {
    return list->count;
}

tNode *tList_first(tList *list) {
    tNode *node = (tNode *) 0;
    if (list->count != 0) {
        node = list->head.nextNode;
    }
    return node;
}

tNode *tList_last(tList *list) {
    tNode *node = (tNode *) 0;
    if (list->count != 0) {
        node = list->head.prevNode;
    }
    return node;
}

tNode *tList_next(tNode *list, tNode *node) {
    if (node->nextNode == node)
        return (tNode *) 0;
    else
        return node->nextNode;
}

tNode *tList_prev(tNode *list, tNode *node) {
    if (node->prevNode == node)
        return (tNode *) 0;
    else
        return node->prevNode;
}

void tList_remove_all(tList *list) {
    tNode *nextNode = list->head.nextNode;
    for (uint32_t i = list->count; i != 0; i--) {
        tNode *currentNode = nextNode;
        nextNode = nextNode->nextNode;

        currentNode->nextNode = currentNode;
        currentNode->prevNode = currentNode;
    }

    list->firstNode = &list->head;
    list->lastNode = &list->head;
    list->count = 0;
}

void tList_add_first(tList *list, tNode *node) {
    node->prevNode = list->firstNode->prevNode;
    node->nextNode = list->firstNode;

    list->firstNode->prevNode = node;
    list->firstNode = node;
    list->count++;
}

void tList_add_last(tList *list, tNode *node) {
    node->nextNode = &list->head;
    node->prevNode = list->lastNode;

    list->lastNode->nextNode = node;
    list->lastNode = node;
    list->count++;
}

tNode *tList_remove_first(tList *list) {
    tNode *node = (tNode *) 0;
    if (list->count != 0) {
        node = list->firstNode;

        node->nextNode->prevNode = &list->head;
        list->firstNode = node->nextNode;
        list->count--;
    }
    return node;
}

void tList_insert_after(tList *list, tNode *nodeAfter, tNode *nodeToInsert) {
    nodeToInsert->nextNode = nodeAfter;
    nodeToInsert->prevNode = nodeAfter->nextNode;

    nodeAfter->nextNode->prevNode = nodeToInsert;
    nodeAfter->nextNode = nodeToInsert;

    list->count++;
}

void tList_remove(tList *list, tNode *node) {
    node->prevNode->nextNode = node->nextNode;
    node->nextNode->prevNode = node->prevNode;
    list->count--;
}











_Bool tList_empty(tList *list) {
    return (list->count == 0);
}

void tList_insert(tList *list, tNode *node, tNode *refNode) {
    node->nextNode = refNode;
    node->prevNode = refNode->prevNode;
    refNode->prevNode->nextNode = node;
    refNode->prevNode = node;
    list->count++;
}

void tList_add(tList *list, tNode *node) {
    node->nextNode = &list->head;
    node->prevNode = list->head.prevNode;
    list->head.prevNode->nextNode = node;
    list->head.prevNode = node;
    list->count++;
}



void tList_clear(tList *list) {
    tNode *node = list->head.nextNode;
    while (node!= &list->head) {
        tNode *next = node->nextNode;
        tNode_init(node);
        node = next;
    }
    list->count = 0;
}







