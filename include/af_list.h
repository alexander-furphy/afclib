#ifndef AF_LIST_H
#define AF_LIST_H

//
// ---------------- Includes ----------------
//

#include <stdbool.h>
#include <stddef.h>
#include "af_array.h"

//
// ---------------- Data ----------------
//

/// Represents the List data structure.
typedef struct List {
    Array array;
    size_t count;
} List;

/// Stores the needed data for iterating over a list.
typedef size_t ListIterator;

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
List listCreate(size_t elementSize);

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

/// Check to see if a list iterator is at the end of the list.
bool listAtEnd(List* list, ListIterator iterator);

/// Get the current item of a list from a list iterator.
void listGet(List* list, ListIterator iterator, void* dest);

#endif // AF_LIST_H