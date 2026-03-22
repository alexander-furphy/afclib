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

typedef struct MapEntry {
    void* key;
    void* value;
    struct MapEntry* next;
} MapEntry;

typedef struct MapBucket {
    struct MapEntry* head;
    struct MapEntry* tail;
} MapBucket;

typedef struct Map {
    MapBucket* buckets;
    size_t size;
    size_t bucketCount;
    size_t keySize;
    size_t valueSize;
    HashFunc hash;
    CompareFunc compare;
} Map;

//
// ---------------- Macros and Inline Functions ----------------
//

/// Represents a null
#define MAP_NULL ((Map){0})

//
// ---------------- Functions ----------------
//

Map mapCreate(size_t keySize, size_t valueSize, HashFunc hash, CompareFunc comp);
void mapFree(Map* map);
void mapClear(Map* map);
Map mapCopy(Map* other);

void mapPut(Map* map, void* key, void* value);
void mapGet(Map* map, void* key, void* dest);
void mapRemove(Map* map, void* key);
bool mapContainsKey(Map* map, void* key);
bool mapContainsValue(Map* map, void* value);

#endif // AF_MAP_H