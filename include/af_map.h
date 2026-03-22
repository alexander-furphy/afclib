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

typedef size_t (*HashFunc)(void* data);

typedef struct Map {
    Array array;
    HashFunc hash;
    CompareFunc compare;
} Map;

#endif // AF_MAP_H