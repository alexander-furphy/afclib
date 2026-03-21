/*

Written by Alexander Furphy.

20 March 2026.

*/

#ifndef AF_LIST_H
#define AF_LIST_H

//
// ---------------- Includes ----------------
//

#include <stdbool.h>
#include <stddef.h>
#include "af_array.h"
#include "af_collections.h"

//
// ---------------- Data ----------------
//

/// Represents the List data structure.
typedef struct List {
    Array array;
    size_t count;
    CompareFunc compare;
} List;

//
// ---------------- Macros and Inline Functions ----------------
//

/// Represents a null/invalid List
#define LIST_NULL ((List){0})

//
// ---------------- Functions ----------------
//

/// Check to see if a List reference is null, or the List within it is invalid.
bool listIsInvalid(const List* list);

/// Create a list with a specified element size.
List listCreate(size_t elementSize, CompareFunc compare);

/// Create a copy of the list.
List listCopy(List* src);

/// Free the memory the list occupies.
void listFree(List* list);

/// Set the list's count to 0.
void listClear(List* list);

/// Add an item to the list.
void listAdd(List* list, void* data);

/// Remove the first occurance an element of a list.
/// Returns true if an item was found and removed.
bool listRemove(List* list, void* data);

/// Check to see if a list contains an element.
bool listContains(List* list, void* data);

/// Get the current item of a list at a specified index.
void listGet(List* list, size_t i, void* dest);

#endif // AF_LIST_H