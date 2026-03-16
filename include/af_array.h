#ifndef AF_ARRAY_H
#define AF_ARRAY_H

#include <stddef.h>

#define ARRAY_NULL ((Array){0})
#define ARRAY_IS_NULL(a) ((a) == NULL || (a)->data == NULL)
#define ARRAY_INVALID(a) ((a).data == NULL)

struct Array;

typedef void (*ArrayDestructor)(struct Array*);

typedef struct Array {
    void* data;
    size_t elementSize;
    size_t capacity;
    ArrayDestructor destructor;
} Array;

Array arrayCreate(size_t elementSize, size_t capacity);
Array arrayCopy(Array* other);
void arraySetDestructor(Array* array, ArrayDestructor destructor);
void arrayReserve(Array* array, size_t newCapacity);
void arrayFree(Array* array);
void arrayClear(Array* array);
void arrayGet(Array* array, size_t index, void* dest);
void arraySet(Array* array, size_t index, void* data);

#endif // AF_ARRAY_H