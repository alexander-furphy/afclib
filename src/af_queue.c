#include "af_queue.h"

#define QUEUE_INITIAL_SIZE 4
#define QUEUE_RESIZE_FACTOR 2

Queue queueCreate(size_t elementSize) {
    if(elementSize == 0) {
        return QUEUE_NULL;
    }

    Array array = arrayCreate(elementSize, QUEUE_INITIAL_SIZE);
    if(arrayIsInvalid(array)) {
        return QUEUE_NULL;
    }

    return (Queue){array, 0, 0, 0};
}

static Array queueCopyToArray(Queue* queue) {
    if(queueIsNull(queue) || queueIsEmpty(queue)) {
        return ARRAY_NULL;
    }

    Array copy = arrayCreate(
        queue->array.elementSize, 
        queue->array.capacity * QUEUE_RESIZE_FACTOR
    );

    if(arrayIsInvalid(copy)) {
        return ARRAY_NULL;
    }

    for(size_t i = queue->ptrFront, j = 0; j < queueCount(queue); i = (i + 1) % queue->array.capacity, j++) {
        arrayCopyElement(&queue->array, &copy, i, j);
    }

    return copy;
}

Queue queueCopy(Queue* src) {
    if(queueIsNull(src)) {
        return QUEUE_NULL;
    }

    Array copy = queueCopyToArray(src);
    if(arrayIsInvalid(copy)) {
        return QUEUE_NULL;
    }

    return (Queue){copy, 0, src->count, src->count};
}

void queueFree(Queue* queue) {
    if(queueIsNull(queue)) {
        return;
    }

    arrayFree(&queue->array);
    *queue = QUEUE_NULL;
}

void queueClear(Queue* queue) {
    if(queueIsNull(queue)) {
        return;
    }

    // Reset the front and rear pointers
    queue->ptrFront = 0;
    queue->ptrRear = 0;
}

static bool queueResize(Queue* queue) {
    if(queueIsNull(queue)) {
        return false;
    }

    Array new = queueCopyToArray(queue);
    if(arrayIsInvalid(new)) {
        return false;
    }

    size_t count = queueCount(queue);
    arrayFree(&queue->array);

    queue->array = new;
    queue->ptrFront = 0;
    queue->ptrRear = count;

    return true;
}

void queuePush(Queue* queue, void* src) {
    if(queueIsNull(queue) || src == NULL) {
        return;
    }

    // Handle resize
    if(queue->count >= queue->array.capacity) {
        // Return on failure
        if(!queueResize(queue)) {
            return;
        }
    }

    // Set the memory and 
    arraySet(&queue->array, queue->ptrRear, src);

    // Increment the pointer and check if the queue is now full
    queue->ptrRear = (queue->ptrRear + 1) % queue->array.capacity;
    queue->count++;
}

void queuePeek(const Queue* queue, void* dest) {
    if(queueIsNull(queue) || queueIsEmpty(queue) || dest == NULL) {
        return;
    }

    // Retrieve the item at the front
    arrayGet(&queue->array, queue->ptrFront, dest);
}

void queuePop(Queue* queue) {
    if(queueIsNull(queue) || queueIsEmpty(queue)) {
        return;
    }

    // Move the front forward one and wrap
    queue->ptrFront = (queue->ptrFront + 1) % queue->array.capacity;
    queue->count--;
}

bool queueIsEmpty(const Queue* queue) {
    return queueCount(queue) == 0;
}

size_t queueCount(const Queue* queue) {
    if(queueIsNull(queue)) {
        return 0;
    }

    return queue->count;
}
