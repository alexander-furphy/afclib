#include "af_list.h"

bool listIsInvalid(const List* list) {
    return list == NULL || 
           arrayIsInvalid(&list->array) || 
           list->count >= list->array.capacity;
}