#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "af_queue.h"

void queueTestLifecycle(void) {
    Queue intQueue = queueCreate(sizeof(int));
    assert(queueCount(&intQueue) == 0);
    assert(queueIsEmpty(&intQueue));
    
    queueFree(&intQueue);
    assert(queueIsInvalid(intQueue));
}

void queueTestBasic(void) {
    Queue intQueue = queueCreate(sizeof(int));

    // Add some values
    int val1 = 5, val2 = 17, val3 = 42;
    queuePush(&intQueue, &val1);
    queuePush(&intQueue, &val2);
    queuePush(&intQueue, &val3);
    assert(queueCount(&intQueue) == 3);

    // Validate the retrieval
    int retrieved;
    queuePeek(&intQueue, &retrieved);
    queuePop(&intQueue);
    assert(retrieved == 5);
    assert(queueCount(&intQueue) == 2);

    queuePeek(&intQueue, &retrieved);
    queuePop(&intQueue);
    assert(retrieved == 17);
    assert(queueCount(&intQueue) == 1);

    queuePeek(&intQueue, &retrieved);
    queuePop(&intQueue);
    assert(retrieved == 42);
    assert(queueCount(&intQueue) == 0);

    // Cleanup
    queueFree(&intQueue);
}

void queueTestCopy(void) {
    Queue intQueue = queueCreate(sizeof(int));
    for(int i = 0; i < 16; i++) {
        queuePush(&intQueue, &i);
    }

    Queue copy = queueCopy(&intQueue);
    assert(!queueIsInvalid(copy));

    for(int i = 0; i < 16; i++) {
        int val;
        queuePeek(&copy, &val);
        queuePop(&copy);
        assert(val == i);
    }

    queueFree(&intQueue);
    queueFree(&copy);
}

void queueTestResizing(void) {
    Queue intQueue = queueCreate(sizeof(int));
    for(int i = 0; i < 16; i++) {
        queuePush(&intQueue, &i);
    }
    assert(queueCount(&intQueue) == 16);

    for(int i = 0; i < 16; i++) {
        int retrieved;
        queuePeek(&intQueue, &retrieved);
        queuePop(&intQueue);
        assert(retrieved == i);
    }
    assert(queueCount(&intQueue) == 0);
    
    queueFree(&intQueue);
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);

    queueTestLifecycle();
    queueTestBasic();
    queueTestCopy();
    queueTestResizing();

    return 0;
}