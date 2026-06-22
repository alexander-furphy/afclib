# afclib

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
![CI](https://github.com/alexander-furphy/afclib/actions/workflows/ci.yml/badge.svg)

afclib is a C library that contains modules for **string handling** and
**generic data structures**. It serves as a lightweight utility library for 
**general-purpose C programming**, making common tasks easier and less prone to 
memory bugs.

## Installation

**macOS/Linux**
Type the following with the CWD set to the root dir.
```
make install
```
All files are created in **/usr/local/include/**.
Headers should be included with **angle brackets <>.**

Link the library to a program once installed with:
```
gcc main.c -lafclib -o program
```

**Windows**
Build using CMake to generate a Visual Studio solution:
```
cmake -B build
```

## Features
- **String Handling:** Buffered, heap allocated strings with all main operations included.
- **Generic Collections:** List, stack, and queue, implemented with void pointers.
- **Lightweight:** No external dependencies, and small build size.

## Examples

**String Handling**
```c
#include <af_string.h>

void stringExample(void) {
    printf("Testing string library:\n");

    String string = stringCreate("Hello World\n");
    stringToUpper(&string);
    stringPrint(&string);

    String delimiter = stringCreate(" ");
    StringArray array = stringSplit(&string, &delimiter);
    String newline = stringCreate("\n");
    String joined = stringJoinArray(&newline, &array);
    stringPrint(&joined);

    stringFree(&string);
    stringFree(&delimiter);
    stringFree(&newline);
    stringFree(&joined);
    stringArrayFreeDeep(&array);

    printf("\n");
}
```
```
Testing string library:
HELLO WORLD
HELLO
WORLD
```
**Collections**
```c
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
    while(!stackIsEmpty(intStack)) {
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
```
```
Testing collection library:
List element 0: 4
List element 1: 9
List element 2: 42

Stack top: 42
Stack top: 9
Stack top: 4

Queue front: 4
Queue front: 9
Queue front: 42
```

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
