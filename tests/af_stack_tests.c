#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "af_stack.h"

void stackTestLifecycle(void) {
    Stack byteStack = stackCreate(sizeof(uint8_t));
    assert(!stackIsInvalid(&byteStack));
    assert(byteStack.count == 0);
    assert(byteStack.array.elementSize == sizeof(uint8_t));

    stackFree(&byteStack);
    assert(stackIsInvalid(&byteStack));
}

void stackTestBasic(void) {
    Stack shortStack = stackCreate(sizeof(short));
    assert(stackIsEmpty(shortStack));
    
    // Values
    short value1 = 10;
    short value2 = 15;
    stackPush(&shortStack, &value1);
    stackPush(&shortStack, &value2);
    assert(stackCount(shortStack) == 2);

    // Check the top
    short top;
    stackPeek(&shortStack, &top);
    assert(top == 15);
    stackPop(&shortStack);
    stackPeek(&shortStack, &top);
    assert(top == 10);
    stackPop(&shortStack);
    stackPop(&shortStack);
    assert(stackCount(shortStack) == 0);

    // Test resizing
    short num = 5;
    for(int i = 0; i < 32; i++) {
        stackPush(&shortStack, &num);
    }
    assert(!stackIsInvalid(&shortStack));
    assert(stackCount(shortStack) == 32);

    stackFree(&shortStack);
}

void intStackDestructor(Stack* stack) {
    while(!stackIsEmpty(*stack)) {
        int* top;
        stackPeek(stack, &top);
        if(top != NULL) {
            free(top);
        }
        stackPop(stack);
    }
}

void stackTestDestructors(void) {
    Stack intPtrStack = stackCreate(sizeof(int*));

    for(int i = 0; i < 10; i++) {
        int* data = malloc(sizeof(int));
        if(data != NULL) {
            *data = i;
            stackPush(&intPtrStack, &data);
        }
    }

    int* top;
    stackPeek(&intPtrStack, &top);
    assert(top != NULL && *top == 9);

    intStackDestructor(&intPtrStack);
    stackFree(&intPtrStack);
}

void stackTestCopy(void) {
    Stack stack = stackCreate(sizeof(char));
    char c = 5;
    for(int i = 0; i < 10; i++) {
        stackPush(&stack, &c);
    }

    Stack copy = stackCopy(&stack);
    assert(!stackIsInvalid(&copy));
    assert(stackCount(copy) == 10);
    assert(copy.array.capacity == 10);

    stackFree(&stack);
    stackFree(&copy);
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);

    stackTestLifecycle();
    stackTestBasic();
    stackTestDestructors();
    stackTestCopy();

    return 0;
}