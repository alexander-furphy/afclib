#include "af_array.h"
#include <stdlib.h>
#include <string.h>

#define ARRAY_INDEX(a, i) ((char*)(a)->data + ((i) * array->elementSize))

Array arrayCreate(size_t elementSize, size_t capacity) {
    if(elementSize == 0 || capacity == 0) {
        return ARRAY_NULL;
    }

    void* data = malloc(elementSize * capacity);

    if(data == NULL) {
        return ARRAY_NULL;
    }

    return (Array){data, elementSize, capacity};
}

void arrayFree(Array* array) {
    if(ARRAY_IS_NULL(array)) {
        return;
    }

    free(array->data);
    *array = ARRAY_NULL;
}

void* arrayGet(Array* array, size_t index) {
    if(ARRAY_IS_NULL(array) || index >= array->capacity) {
        return NULL;
    }

    return ARRAY_INDEX(array, index);
}

void arraySet(Array* array, size_t index, void* data) {
    if(ARRAY_IS_NULL(array) || index >= array->capacity || data == NULL) {
        return;
    }

    memcpy(ARRAY_INDEX(array, index), data, array->elementSize);
}