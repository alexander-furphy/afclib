/*

-------------------------------- Memory Model --------------------------------

Arrays store an internal element size and capacity, as well as an optional
destructor for use with collections that require memory cleanup. Items are
set by copying data into the array, and read by copying data from the array
into another memory location. All array data is initialised to zero on
creating, to add safety with the use of destructors.

-------------------------------- Important --------------------------------

All data must be passed by reference to ensure the raw bytes of the data
are written into the array. If the array is of type int, references to
integers will be passed into array get/set methods. If the array is of
type int*, references to integer pointers will be passed into the array
get/set methods.

When writing custom destructors, it is ideal to get each element of the
array with the arrayGet method, then call your custom free functions on
that data.

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

//
// ---------------- Macros ----------------
//

/// Represents a null/invalid array.
#define ARRAY_NULL ((Array){0})

/// Check to see if an array is invalid.
#define ARRAY_INVALID(a) ((a).data == NULL)

/// Check to see if an array reference is null, or the array within it is invalid.
#define ARRAY_IS_NULL(a) ((a) == NULL || ARRAY_INVALID(*(a)))

//
// ---------------- Data ----------------
//

struct Array;

/// Function pointer that gets a reference to an array.
typedef void (*ArrayDestructor)(struct Array*);

/// Stores a generic array object (data, element size, capacity, destructor).
typedef struct Array {
    void* data;
    size_t elementSize;
    size_t capacity;
    ArrayDestructor destructor;
} Array;

//
// ---------------- Functions ----------------
//

/// Allocates a new array big enough to store capacity
/// elements of size elementSize. All data is initialised to zero.
Array arrayCreate(size_t elementSize, size_t capacity);

/// Deeply copy the data of a pre existing array into a new array.
Array arrayCopy(Array* other);

/// Set the destructor of an array. This destructor will be called
/// when arrayFree or arrayClear is called.
void arraySetDestructor(Array* array, ArrayDestructor destructor);

/// Reallocate the array if it is below new capacity.
/// Array will be freed and set to null if the operation fails.
void arrayReserve(Array* array, size_t newCapacity);

/// Free an array, calling the destructor if it has been set.
void arrayFree(Array* array);

/// Call the array destructor if it has been set.
void arrayClear(Array* array);

/// Copy an individual element of the array into a destination (bounds checked).
void arrayGet(Array* array, size_t index, void* dest);

/// Copy source data into an index of the array (bounds checked).
void arraySet(Array* array, size_t index, void* src);

#endif // AF_ARRAY_H