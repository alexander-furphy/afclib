#include "af_array.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void arrayTestLifecycle(void) {
    size_t cap = 10;
    Array intArray = arrayCreate(sizeof(int), cap);
    
    // Test if creation worked
    assert(!ARRAY_INVALID(intArray));
    assert(intArray.elementSize == sizeof(int));
    assert(intArray.capacity == cap);
    assert(intArray.data != NULL);
    assert(intArray.destructor == NULL);

    arrayReserve(&intArray, 20);
    assert(!ARRAY_INVALID(intArray));
    assert(intArray.capacity == 20);

    // Test freeing (assuming it zeros out the struct)
    arrayFree(&intArray);
    assert(intArray.data == NULL);
}

void arrayTestGetSet(void) {
    Array intArray = arrayCreate(sizeof(int), 5);
    int val1 = 42;
    int val2 = 100;

    // Set values
    arraySet(&intArray, 0, &val1);
    arraySet(&intArray, 4, &val2);

    // Get and verify values
    int* res1 = (int*)arrayGet(&intArray, 0);
    int* res4 = (int*)arrayGet(&intArray, 4);

    assert(res1 != NULL);
    assert(*res1 == 42);

    assert(res4 != NULL);
    assert(*res4 == 100);

    arrayFree(&intArray);

    // Create new array
    Array doubleArray = arrayCreate(sizeof(double), 5);
    double da = 5.34;
    double db = 98.3;

    // Set values
    arraySet(&doubleArray, 1, &da);
    arraySet(&doubleArray, 6, &db);

    // Get values
    double* ref1 = arrayGet(&doubleArray, 1);
    double* ref2 = arrayGet(&doubleArray, 6);

    // Assert values
    assert(ref1 != NULL);
    assert(*ref1 == 5.34);

    assert(ref2 == NULL);

    arrayFree(&doubleArray);
}

void arrayTestNullMacros(void) {    
    Array empty = ARRAY_NULL;
    assert(ARRAY_INVALID(empty));

    Array charArray = arrayCreate(sizeof(char), 1);
    assert(!ARRAY_INVALID(charArray));
    assert(!ARRAY_IS_NULL(&charArray));

    arrayFree(&charArray);
    assert(ARRAY_IS_NULL(&charArray));
}

void intArrayDestructor(Array* array) {
    for(size_t i = 0; i < array->capacity; i++) {
        int** element = arrayGet(array, i);
        if(*element != NULL) {
            free(*element);
            *element = NULL;
        }
    }
}

void arrayTestDestructors(void) {
    // Create a int* array with destructor
    Array intPtrArray = arrayCreate(sizeof(int*), 5);
    arraySetDestructor(&intPtrArray, intArrayDestructor);
    assert(intPtrArray.destructor == intArrayDestructor);

    // Add element
    int* element = malloc(sizeof(int));
    assert(element != NULL);
    *element = 5;
    arraySet(&intPtrArray, 0, &element);

    // Ensure retrieved item is correct
    int** retrieved = arrayGet(&intPtrArray, 0);
    assert(retrieved != NULL);
    assert(**retrieved == 5);

    // Add another element
    element = malloc(sizeof(int));
    assert(element != NULL);
    *element = 10;
    arraySet(&intPtrArray, 3, &element);

    // Ensure retrieved item is correct
    retrieved = arrayGet(&intPtrArray, 3);
    assert(retrieved != NULL);
    assert(**retrieved == 10);

    arrayClear(&intPtrArray);
    printf("%p\n", arrayGet(&intPtrArray, 0));
    assert(*((int**)arrayGet(&intPtrArray, 0)) == NULL);

    // Free the array (uses destructor)
    // Memory leaks/bad accessing will be caught by tools
    arrayFree(&intPtrArray);
}

void arrayTestCopy(void) {
    size_t capacity = 5;
    Array array = arrayCreate(sizeof(int), capacity);
    for(int i = 0; i < (int)capacity; i++) {
        arraySet(&array, i, &i);
        assert(*((int*)arrayGet(&array, i)) == i);
    }

    Array copy = arrayCopy(&array);
    assert(copy.capacity == capacity);
    assert(copy.elementSize == sizeof(int));
    for(int i = 0; i < (int)capacity; i++) {
        assert(*((int*)arrayGet(&copy, i)) == i);
    }

    arrayFree(&array);
    assert(!ARRAY_INVALID(copy));
    arrayFree(&copy);
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);

    arrayTestLifecycle();
    arrayTestGetSet();
    arrayTestNullMacros();
    arrayTestDestructors();
    arrayTestCopy();

    return 0;
}