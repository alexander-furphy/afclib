#ifndef AF_MAP_H
#define AF_MAP_H

//
// ---------------- Includes ----------------
//

#include "af_array.h"
#include "af_collections.h"
#include <stddef.h>

//
// ---------------- Data ----------------
//

/// Represents a function that hashes memory into a size_t.
typedef size_t (*HashFunc)(void* data);

/// Represents the Hash Map data structure.
typedef struct Map {
    Array array;
    HashFunc hash;
    CompareFunc compare;
} Map;

//
// ---------------- Macros and Inline Functions ----------------
//

/// Represents a null/invalid map.
#define MAP_NULL ((Map){0})

//
// ---------------- Functions ----------------
//

size_t hashMem(void* mem, size_t length);

#endif // AF_MAP_H