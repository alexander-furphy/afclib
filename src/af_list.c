#include "af_list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

bool listIsInvalid(const List* list) {
    return list == NULL || 
           arrayIsInvalid(&list->array) || 
           list->count > list->array.capacity;
}

List listCreate(size_t elementSize, CompareFunc compare) {
    if(elementSize == 0) {
        return LIST_NULL;
    }

    Array array = arrayCreate(elementSize, COLLECTION_INITIAL_SIZE);
    if(arrayIsInvalid(&array)) {
        return LIST_NULL;
    }

    return (List){array, 0, compare};
}

List listCopy(List* src) {
    if(listIsInvalid(src)) {
        return LIST_NULL;
    }

    Array copy = arrayCopy(&src->array);
    if(arrayIsInvalid(&copy)) {
        return LIST_NULL;
    }

    return (List){copy, src->count, src->compare};
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
        bool success = arrayReserve(&list->array, list->array.capacity * COLLECTION_RESIZE_FACTOR);
        if(!success) {
            return;
        }
    }

    arraySet(&list->array, list->count++, data);
}

bool listRemove(List* list, void* data) {
    if(listIsInvalid(list) || data == NULL || list->count == 0) {
        return false;
    }

    // Loop through each element and compare it for being the same
    for(size_t i = 0; i < list->count; i++) {
        void* arrayData = arrayGetIndexPtr(&list->array, i);

        if(list->compare(arrayData, data)) {
            if(i == list->array.capacity - 1) {
                list->count--;
                return true;
            }
            
            void* dest = arrayGetIndexPtr(&list->array, i);
            arrayMoveRange(&list->array, dest, i + 1, list->count - i);
            list->count--;

            return true;
        }
    }

    return false;
}

bool listContains(List* list, void* data) {
    if(listIsInvalid(list) || data == NULL) {
        return false;
    }

    // Loop through each element and compare it for being the same
    for(size_t i = 0; i < list->count; i++) {
        void* arrayData = arrayGetIndexPtr(&list->array, i);
        if(list->compare(arrayData, data)) {
            return true;
        }
    }

    return false;
}

void listGet(List* list, size_t i, void* dest) {
    if(listIsInvalid(list) || i >= list->count || dest == NULL) {
        return;
    }

    arrayGet(&list->array, i, dest);
}
