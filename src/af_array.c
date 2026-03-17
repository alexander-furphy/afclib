#include "af_array.h"
#include <stdlib.h>
#include <string.h>

/// Calculates the pointer to a specific index of an array.
#define ARRAY_INDEX(a, i) ((char*)(a)->data + ((i) * array->elementSize))

Array arrayCreate(size_t elementSize, size_t capacity) {
    if(elementSize == 0 || capacity == 0) {
        return ARRAY_NULL;
    }

    // Allocate and validate data
    void* data = calloc(capacity, elementSize);
    if(data == NULL) {
        return ARRAY_NULL;
    }

    return (Array){data, elementSize, capacity, NULL};
}

Array arrayCopy(Array* other) {
    if(arrayIsNull(other)) {
        return ARRAY_NULL;
    }

    // Create a new array of the required size
    Array copy = arrayCreate(other->elementSize, other->capacity);
    if(arrayIsInvalid(copy)) {
        return ARRAY_NULL;
    }

    // Set the destructor and copy the data to the new array
    arraySetDestructor(&copy, other->destructor);
    memcpy(copy.data, other->data, other->elementSize * other->capacity);

    return copy;
}

void arraySetDestructor(Array* array, ArrayDestructor destructor) {
    if(arrayIsNull(array) || destructor == NULL) {
        return;
    }

    array->destructor = destructor;
}

void arrayReserve(Array* array, size_t newCapacity) {
    if(arrayIsNull(array) || newCapacity <= array->capacity) {
        return;
    }

    // Allocate and validate a new block of memory.
    void* temp = calloc(newCapacity, array->elementSize);
    if(temp == NULL) {
        // Free and null the old data on failure.
        arrayFree(array);
        *array = ARRAY_NULL;
        return;
    }

    // Copy the data into the new array
    memcpy(temp, array->data, array->capacity * array->elementSize);

    // Free the old data
    free(array->data);

    // Set the array to use new data
    array->data = temp;
    array->capacity = newCapacity;
}

void arrayFree(Array* array) {
    if(arrayIsNull(array)) {
        return;
    }

    // Call the destructor if it exists
    if(array->destructor != NULL) {
        array->destructor(array);
    }

    // Free the actual array
    free(array->data);
    *array = ARRAY_NULL;
}

void arrayClear(Array* array) {
    if(arrayIsNull(array)) {
        return;
    }

    // Call the array's destructor if it exists
    if(array->destructor != NULL) {
        array->destructor(array);
    }
}

void arrayGet(Array* array, size_t index, void* dest) {
    // Bounds and null check
    if(arrayIsNull(array) || index >= array->capacity || dest == NULL) {
        return;
    }

    // Copy the data into the destination
    memcpy(dest, ARRAY_INDEX(array, index), array->elementSize);
}

void arraySet(Array* array, size_t index, void* src) {
    // Bounds and null check
    if(arrayIsNull(array) || index >= array->capacity || src == NULL) {
        return;
    }

    // Copy the source into the array
    memcpy(ARRAY_INDEX(array, index), src, array->elementSize);
}
