/*

Written by Alexander Furphy.

18 March 2026.

*/

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

/// Stores a Stack data structure.
typedef struct Stack {
    Array array;
    size_t count;
} Stack;

/// Function pointer that takes a reference to a Stack.
typedef void (*StackFunc)(Stack* stack);

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

/// Create a stack with a specified element size.
Stack stackCreate(size_t elementSize);

/// Copy a stack's data into another stack.
Stack stackCopy(Stack* other);

/// Free a stack.
void stackFree(Stack* stack);

/// Set a stack's size to zero.
void stackClear(Stack* stack);

/// Push an item to the stack.
void stackPush(Stack* stack, void* src);

/// Copy the top of the stack into another location.
void stackPeek(Stack* stack, void* dest);

/// Pop the stack (reduce count by 1).
void stackPop(Stack* stack);

#endif // AF_STACK_H