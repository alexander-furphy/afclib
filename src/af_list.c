#include "af_list.h"
#include <stdlib.h>
#include <string.h>

#define LIST_INITIAL_SIZE 4
#define LIST_RESIZE_FACTOR 2

bool listIsInvalid(const List* list) {
    return list == NULL || 
           arrayIsInvalid(&list->array) || 
           list->count >= list->array.capacity;
}

List listCreate(size_t elementSize) {
    if(elementSize == 0) {
        return LIST_NULL;
    }

    Array array = arrayCreate(elementSize, LIST_INITIAL_SIZE);
    if(arrayIsInvalid(&array)) {
        return LIST_NULL;
    }

    return (List){array, 0};
}

List listCopy(List* src) {
    if(listIsInvalid(src)) {
        return LIST_NULL;
    }

    Array copy = arrayCopy(&src->array);
    if(arrayIsInvalid(&copy)) {
        return LIST_NULL;
    }

    return (List){copy, src->count};
}

void listFree(List* list) {
    if(listIsInvalid(list)) {
        return;
    }

    arrayFree(&list->array);
    *list = LIST_NULL;
}

void listClear(List* list) {
    list->count = 0;
}

void listAdd(List* list, void* data) {
    if(listIsInvalid(list) || data == NULL) {
        return;
    }

    if(list->count >= list->array.capacity) {
        bool success = arrayReserve(&list->array, list->array.capacity * LIST_RESIZE_FACTOR);
        if(!success) {
            return;
        }
    }
}

bool listRemove(List* list, void* data) {
    if(listIsInvalid(list) || data == NULL) {
        return false;
    }

    // Temporary memory for checking equality
    void* temp = malloc(list->array.elementSize);
    if(temp == NULL) {
        return false;
    }

    // Loop through each element and compare it for being the same
    for(size_t i = 0; i < list->count; i++) {
        arrayGet(&list->array, i, &temp);
        if(memcmp(temp, data, list->array.elementSize) == 0) {
            free(temp);

            if(i == list->array.capacity - 1) {
                return true;
            }
            
            void* dest = arrayGetIndexPtr(&list->array, i);
            arrayMoveRange(&list->array, dest, i + 1, list->count - i);

            return true;
        }
    }

    free(temp);
    return false;
}

bool listContains(List* list, void* data) {
    if(listIsInvalid(list) || data == NULL) {
        return false;
    }

    // Temporary memory for checking equality
    void* temp = malloc(list->array.elementSize);
    if(temp == NULL) {
        return false;
    }

    // Loop through each element and compare it for being the same
    for(size_t i = 0; i < list->count; i++) {
        arrayGet(&list->array, i, &temp);
        if(memcmp(temp, data, list->array.elementSize) == 0) {
            free(temp);
            return true;
        }
    }

    free(temp);
    return false;
}

bool listAtEnd(List* list, ListIterator iterator) {
    if(listIsInvalid(list) || list->count == 0) {
        return true;
    }

    return iterator >= list->count - 1ULL;
}

void listGet(List* list, ListIterator iterator, void* dest) {
    if(listIsInvalid(list) || iterator >= list->count || dest == NULL) {
        return;
    }

    arrayGet(&list->array, iterator, dest);
}
