#ifndef AF_STACK_H
#define AF_STACK_H

//
// ---------------- Includes ----------------
//

#include <stdbool.h>
#include <stddef.h>
#include "af_array.h"

//
// ---------------- Data ----------------
//

typedef struct Stack Stack;

typedef void (*StackDestructor)(Stack* stack);

struct Stack {
    Array array;
    size_t count;
    StackDestructor destructor;
};

//
// ---------------- Macros and Inline Functions ----------------
//

/// Represents a null string.
#define STACK_NULL ((Stack){0})

/// Boolean check to see if a stack is invalid.
static inline bool stackIsInvalid(const Stack stack) {
    return arrayIsInvalid(stack.array);
}

/// Boolean check to see if a stack pointer or the data inside it is invalid.
static inline bool stackIsNull(const Stack* stack) {
    return stack == NULL || stackIsInvalid(*stack);
}

/// Tests if a stack is empty.
static inline bool stackIsEmpty(const Stack stack) {
    return stack.count == 0;
}

/// Get the size of a stack.
static inline size_t stackCount(const Stack stack) {
    return stack.count;
}

//
// ---------------- Functions ----------------
//

Stack stackCreate(size_t elementSize);
void stackSetDestructor(Stack* stack, StackDestructor destructor);
Stack stackCopy(Stack* other);
void stackFree(Stack* stack);
void stackClear(Stack* stack);

void stackPush(Stack* stack, void* src);
void stackPeek(Stack* stack, void* dest);
void stackPop(Stack* stack);

#endif // AF_STACK_H