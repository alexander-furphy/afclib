#include "af_stack.h"

#define STACK_INITIAL_SIZE 4
#define STACK_GROWTH_FACTOR 2

bool stackIsInvalid(const Stack* stack) {
    return stack == NULL ||
           stack->count > stack->array.capacity ||
           arrayIsInvalid(&stack->array);
}

bool stackIsEmpty(const Stack stack) {
    return stack.count == 0;
}

size_t stackCount(const Stack stack) {
    return stack.count;
}

Stack stackCreate(size_t elementSize) {
    if(elementSize == 0) {
        return STACK_NULL;
    }

    Array array = arrayCreate(elementSize, STACK_INITIAL_SIZE);
    if(arrayIsInvalid(&array)) {
        return STACK_NULL;
    }

    return (Stack){array, 0};
}

Stack stackCopy(Stack* other) {
    if(stackIsInvalid(other)) {
        return STACK_NULL;
    }

    // Create a new stack if the count is 0
    if(other->count == 0) {
        return stackCreate(other->array.elementSize);
    }

    // Copy the range of
    Array copy = arrayCopyRange(&other->array, 0, other->count);
    if(arrayIsInvalid(&copy)) {
        return STACK_NULL;
    }

    return (Stack){copy, other->count};
}

void stackFree(Stack* stack) {
    if(stackIsInvalid(stack)) {
        return;
    }

    arrayFree(&stack->array);
    *stack = STACK_NULL;
}

void stackClear(Stack* stack) {
    if(stackIsInvalid(stack)) {
        return;
    }

    stack->count = 0;
}

static bool stackResize(Stack* stack) {
    if(stackIsInvalid(stack)) {
        return false;
    }

    // Array reserve returns false on failure
    return arrayReserve(&stack->array, stack->array.capacity * STACK_GROWTH_FACTOR);
}

void stackPush(Stack* stack, void* src) {
    if(stackIsInvalid(stack) || src == NULL) {
        return;
    }

    if(stack->count >= stack->array.capacity) {
        bool success = stackResize(stack);
        if(!success) {
            return;
        }
    }

    arraySet(&stack->array, stack->count++, src);
}

void stackPeek(Stack* stack, void* dest) {
    if(stackIsInvalid(stack) || stack->count == 0 || dest == NULL) {
        return;
    }

    arrayGet(&stack->array, stack->count - 1, dest);
}

void stackPop(Stack* stack) {
    if(stackIsInvalid(stack)) {
        return;
    }

    if(stack->count > 0) {
        stack->count--;
    }
}
