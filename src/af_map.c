#include "af_map.h"
#include <stdint.h>
#include <stdlib.h>

size_t hashMem(size_t seed, void* data, size_t length) {
    // Special constants (seperate for 32 bit and 64 bit systems)
#if SIZE_MAX > 0xFFFFFFFFU
    static const size_t FNV_OFFSET_BASIS = 14695981039346656037ULL;
    static const size_t FNV_PRIME = 1099511628211ULL;
#else
    static const size_t FNV_OFFSET_BASIS = 2166136261U;
    static const size_t FNV_PRIME = 16777619U;
#endif

    // Start the hash by adding the offset to the current seed
    size_t hash = FNV_OFFSET_BASIS ^ seed;
    const uint8_t* ptr = (const uint8_t*)data;

    // XOR the hash against each byte and multiply it against the FNV_PRIME
    for(size_t i = 0; i < length; i++) {
        hash ^= ptr[i];
        hash *= FNV_PRIME;
    }

    return hash;
}

Map mapCreate(size_t keySize, size_t valueSize, HashFunc hash, CompareFunc comp) {
    MapBucket* buckets = calloc(sizeof(MapBucket), COLLECTION_INITIAL_SIZE);

    return (Map){

        .bucketCount = COLLECTION_INITIAL_SIZE,
        .compare = comp,
        .hash = hash,
        .keySize = keySize,
        .valueSize = valueSize,
        .size = 0,
    };
}

void mapFree(Map* map) {

}

void mapClear(Map* map) {

}

Map mapCopy(Map* other) {

}

void mapPut(Map* map, void* key, void* value) {

}

void mapGet(Map* map, void* key, void* dest) {

}

void mapRemove(Map* map, void* key) {

}

bool mapContainsKey(Map* map, void* key) {

}

bool mapContainsValue(Map* map, void* value) {

}
