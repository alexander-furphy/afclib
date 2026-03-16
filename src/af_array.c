#include "af_array.h"
#include <stdlib.h>
#include <string.h>

#define ARRAY_INDEX(a, i) ((char*)(a)->data + ((i) * array->elementSize))

Array arrayCreate(size_t elementSize, size_t capacity) {
    if(elementSize == 0 || capacity == 0) {
        return ARRAY_NULL;
    }

    void* data = calloc(capacity, elementSize);

    if(data == NULL) {
        return ARRAY_NULL;
    }

    return (Array){data, elementSize, capacity, NULL};
}

Array arrayCopy(Array* other) {
    if(ARRAY_IS_NULL(other)) {
        return ARRAY_NULL;
    }

    Array copy = arrayCreate(other->elementSize, other->capacity);
    if(ARRAY_INVALID(copy)) {
        return ARRAY_NULL;
    }
    arraySetDestructor(&copy, other->destructor);

    memcpy(copy.data, other->data, other->elementSize * other->capacity);

    return copy;
}

void arraySetDestructor(Array* array, ArrayDestructor destructor) {
    if(ARRAY_IS_NULL(array) || destructor == NULL) {
        return;
    }

    array->destructor = destructor;
}

void arrayReserve(Array* array, size_t newCapacity) {
    if(ARRAY_IS_NULL(array) || newCapacity <= array->capacity) {
        return;
    }

    void* temp = realloc(array->data, newCapacity * array->elementSize);
    if(temp == NULL) {
        arrayFree(array);
        *array = ARRAY_NULL;
        return;
    }

    array->data = temp;
    array->capacity = newCapacity;
}

void arrayFree(Array* array) {
    if(ARRAY_IS_NULL(array)) {
        return;
    }

    if(array->destructor != NULL) {
        array->destructor(array);
    }

    free(array->data);
    *array = ARRAY_NULL;
}

void arrayClear(Array* array) {
    if(ARRAY_IS_NULL(array)) {
        return;
    }

    if(array->destructor != NULL) {
        array->destructor(array);
    }
}

void arrayGet(Array* array, size_t index, void* dest) {
    if(ARRAY_IS_NULL(array) || index >= array->capacity) {
        return;
    }

    memcpy(dest, ARRAY_INDEX(array, index), array->elementSize);
}

void arraySet(Array* array, size_t index, void* data) {
    if(ARRAY_IS_NULL(array) || index >= array->capacity || data == NULL) {
        return;
    }

    memcpy(ARRAY_INDEX(array, index), data, array->elementSize);
}
