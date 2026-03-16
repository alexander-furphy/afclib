#include "af_array.h"
#include <assert.h>

void arrayTestLifecycle(void) {
    size_t cap = 10;
    Array arr = arrayCreate(sizeof(int), cap);
    
    // Test if creation worked
    assert(!ARRAY_INVALID(arr));
    assert(arr.elementSize == sizeof(int));
    assert(arr.capacity == cap);
    assert(arr.data != NULL);

    // Test freeing (assuming it zeros out the struct)
    arrayFree(&arr);
    assert(arr.data == NULL);
}

void arrayTestGetSet(void) {
    Array arr = arrayCreate(sizeof(int), 5);
    int val1 = 42;
    int val2 = 100;

    // Set values
    arraySet(&arr, 0, &val1);
    arraySet(&arr, 4, &val2);

    // Get and verify values
    int* res1 = (int*)arrayGet(&arr, 0);
    int* res4 = (int*)arrayGet(&arr, 4);

    assert(res1 != NULL);
    assert(*res1 == 42);

    assert(res4 != NULL);
    assert(*res4 == 100);

    arrayFree(&arr);

    // Create new array
    arr = arrayCreate(sizeof(double), 5);
    double da = 5.34;
    double db = 98.3;

    // Set values
    arraySet(&arr, 1, &da);
    arraySet(&arr, 6, &db);

    // Get values
    double* ref1 = arrayGet(&arr, 1);
    double* ref2 = arrayGet(&arr, 6);

    // Assert values
    assert(ref1 != NULL);
    assert(*ref1 == 5.34);

    assert(ref2 == NULL);

    arrayFree(&arr);
}

void arrayTestNullMacros() {    
    Array empty = ARRAY_NULL;
    assert(ARRAY_INVALID(empty));

    Array arr = arrayCreate(sizeof(char), 1);
    assert(!ARRAY_INVALID(arr));
    assert(!ARRAY_IS_NULL(&arr));

    arrayFree(&arr);
    assert(ARRAY_IS_NULL(&arr));
}

int main(void) {
    arrayTestLifecycle();
    arrayTestGetSet();
    arrayTestNullMacros();
    return 0;
}