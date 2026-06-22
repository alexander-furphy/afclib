#include "af_list.h"
#include "af_stack.h"
#include "af_queue.h"
#include "stdio.h"

bool intCompare(void* a, void* b) {
    return *((int*)a) == *((int*)b);
}

void collectionsExample(void) {
    printf("Testing collection library:\n");

    List intList = listCreate(sizeof(int), intCompare);
    int a = 4, b = 9, c = 42;
    listAdd(&intList, &a);
    listAdd(&intList, &b);
    listAdd(&intList, &c);
    for(int i = 0; i < intList.count; i++) {
        int retrieved = 0;
        listGet(&intList, i, &retrieved);
        printf("List element %d: %d\n", i, retrieved);
    }
    listFree(&intList);
    printf("\n");

    Stack intStack = stackCreate(sizeof(int));
    stackPush(&intStack, &a);
    stackPush(&intStack, &b);
    stackPush(&intStack, &c);
    while(!stackIsEmpty(&intStack)) {
        int retrieved = 0;
        stackPeek(&intStack, &retrieved);
        stackPop(&intStack);
        printf("Stack top: %d\n", retrieved);
    }
    stackFree(&intStack);
    printf("\n");

    Queue intQueue = queueCreate(sizeof(int));
    queuePush(&intQueue, &a);
    queuePush(&intQueue, &b);
    queuePush(&intQueue, &c);
    while(!queueIsEmpty(&intQueue)) {
        int retrieved = 0;
        queuePeek(&intQueue, &retrieved);
        queuePop(&intQueue);
        printf("Queue front: %d\n", retrieved);
    }
    queueFree(&intQueue);
    printf("\n");
}