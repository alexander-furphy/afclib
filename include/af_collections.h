/*

Written by Alexander Furphy.

21 March 2026.

*/

#ifndef AF_COLLECTIONS_H
#define AF_COLLECTIONS_H

/// Function for comparing two blocks of memory for equality.
typedef bool (*CompareFunc)(void*, void*);

/// Multiplier for size of collections when a resize is triggered.
#define COLLECTION_RESIZE_FACTOR 2

/// Initial size collections are initialised to.
#define COLLECTION_INITIAL_SIZE 8

#endif // AF_COLLECTIONS_H