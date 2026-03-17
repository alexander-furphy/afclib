#include "af_stack.h"

#define STACK_INITIAL_SIZE 4
#define STACK_GROWTH_FACTOR 2

Stack stackCreate(size_t elementSize) {
    if(elementSize == 0) {
        return STACK_NULL;
    }

    Array array = arrayCreate(elementSize, STACK_INITIAL_SIZE);
    if(arrayIsInvalid(array)) {
        return STACK_NULL;
    }

    return (Stack){array, 0};
}

Stack stackCopy(Stack* other) {
    if(stackIsNull(other)) {
        return STACK_NULL;
    }

    Array copy = arrayCopy(&other->array);
    if(arrayIsInvalid(copy)) {
        return STACK_NULL;
    }

    return (Stack){copy, other->count};
}

void stackFree(Stack* stack) {
    if(stackIsNull(stack)) {
        return;
    }

    arrayFree(&stack->array);
    *stack = STACK_NULL;
}

void stackClear(Stack* stack) {
    if(stackIsNull(stack)) {
        return;
    }

    stack->count = 0;
}

void stackPush(Stack* stack, void* src) {
    if(stackIsNull(stack) || src == NULL) {
        return;
    }

    if(stack->count >= stack->array.capacity) {
        arrayReserve(&stack->array, stack->array.capacity * STACK_GROWTH_FACTOR);

        if(arrayIsInvalid(stack->array)) {
            *stack = STACK_NULL;
            return;
        }
    }

    arraySet(&stack->array, stack->count++, src);
}

void stackPeek(Stack* stack, void* dest) {
    if(stackIsNull(stack) || stack->count == 0 || dest == NULL) {
        return;
    }

    arrayGet(&stack->array, stack->count - 1, dest);
}

void stackPop(Stack* stack) {
    if(stackIsNull(stack)) {
        return;
    }

    if(stack->count > 0) {
        stack->count--;
    }
}
