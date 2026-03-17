#ifndef AF_STACK_H
#define AF_STACK_H

#include <stdbool.h>
#include <stddef.h>
#include "af_array.h"

typedef struct Stack {
    Array array;
    size_t count;
} Stack;

#define STACK_NULL ((Stack){0})

static inline bool stackIsInvalid(const Stack stack) {
    return arrayIsInvalid(stack.array);
}

static inline bool stackIsNull(const Stack* stack) {
    return stack == NULL || stackIsInvalid(*stack);
}

#endif // AF_STACK_H