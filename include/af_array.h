/*

-------------------------------- Memory Model --------------------------------

Arrays store an internal element size and capacity, as well as an optional
destructor for use with collections that require memory cleanup. Items are
set by copying data into the array, and read by copying data from the array
into another memory location. All array data is initialised to zero on
creating, to add safety with the use of destructors and copying.

-------------------------------- Important --------------------------------

All data must be passed by reference to ensure the raw bytes of the data
are written into the array. If the array is of type int, references to
integers will be passed into array get/set methods. If the array is of
type int*, references to integer pointers will be passed into the array
get/set methods.

-------------------------------- Author --------------------------------

Written by Alexander Furphy.

17 March 2026.

*/

#ifndef AF_ARRAY_H
#define AF_ARRAY_H

//
// ---------------- Includes ----------------
//

#include <stddef.h>
#include <stdbool.h>

//
// ---------------- Data ----------------
//

/// Stores a generic array object (data, element size, capacity, destructor).
typedef struct Array {
    void* data;
    size_t elementSize;
    size_t capacity;
} Array;

/// Function pointer that gets a reference to an array.
typedef void (*ArrayFunc)(Array*);

//
// ---------------- Macros and Inline Functions ----------------
//

/// Represents a null/invalid array.
#define ARRAY_NULL ((Array){0})

/// Check to see if an array is invalid.
static inline bool arrayIsInvalid(const Array array) {
    return array.data == NULL;
}

/// Check to see if an array reference is null, or the array within it is invalid.
static inline bool arrayIsNull(const Array* array) {
    return array == NULL || arrayIsInvalid(*array);
}

//
// ---------------- Functions ----------------
//

/// Allocates a new array big enough to store capacity
/// elements of size elementSize. All data is initialised to zero.
Array arrayCreate(size_t elementSize, size_t capacity);

/// Deeply copy the data of a pre existing array into a new array.
Array arrayCopy(Array* other);

/// Copy a range of elements into a new array. Index B is exclusive.
Array arrayCopyRange(Array* other, size_t indexA, size_t indexB);

/// Copy an individual element of one array into a slot of another array.
void arrayCopyElement(const Array* source, Array* dest, size_t from, size_t to);

/// Reallocate the array if it is below new capacity.
/// Array will be freed and set to null if the operation fails.
void arrayReserve(Array* array, size_t newCapacity);

/// Free an array.
void arrayFree(Array* array);

/// Zero every byte in the array.
void arrayClear(Array* array);

/// Copy an individual element of the array into a destination (bounds checked).
void arrayGet(const Array* array, size_t index, void* dest);

/// Copy source data into an index of the array (bounds checked).
void arraySet(Array* array, size_t index, void* src);

#endif // AF_ARRAY_H