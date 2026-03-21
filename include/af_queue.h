/*

Written by Alexander Furphy.

18 March 2026.

*/

#ifndef AF_QUEUE_H
#define AF_QUEUE_H

//
// ---------------- Includes ----------------
//

#include <stdbool.h>
#include <stddef.h>
#include "af_collections.h"
#include "af_array.h"

//
// ---------------- Data ----------------
//

/// Stores a Queue data structure.
typedef struct Queue {
    Array array;
    size_t ptrFront;
    size_t ptrRear;
    size_t count;
} Queue;

/// Function pointer that takes a reference to a Queue.
typedef void (*QueueFunc)(Queue*);

//
// ---------------- Macros and Inline Functions ----------------
//

/// Represents a null/invalid Queue
#define QUEUE_NULL ((Queue){0})

//
// ---------------- Functions ----------------
//

/// Check to see if a Queue reference is null, or the Queue within it is invalid.
bool queueIsInvalid(const Queue* queue);

/// Create a queue with a specified element size.
Queue queueCreate(size_t elementSize);

/// Copy each element of a queue into a new queue.
Queue queueCopy(Queue* src);

/// Free the data of the queue.
void queueFree(Queue* queue);

/// Set the queue's size to zero.
void queueClear(Queue* queue);

/// Push an item to the back of the queue.
void queuePush(Queue* queue, void* src);

/// Retrieve the item at the front of the queue without removing it.
void queuePeek(const Queue* queue, void* dest);

/// Remove the item at the front of the queue.
void queuePop(Queue* queue);

/// Check if a queue is empty.
bool queueIsEmpty(const Queue* queue);

/// Get the amount of elements of a queue.
size_t queueCount(const Queue* queue);

#endif // AF_QUEUE_H