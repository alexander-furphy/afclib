#include "string_wrappers.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

void stringSetCStr(String* string, const char* value) {
    if(STR_IS_NULL(string)) {
        return;
    }

    size_t length = strlen(value);
    stringReserve(string, length);

    // Copy the data into the string.
    memcpy(string->data, value, length);
    string->length = length;
}

void stringAppendCStr(String* dest, const char* value) {
    if(STR_IS_NULL(dest) || value == NULL) {
        return;
    }

    size_t valueLength = strlen(value);
    size_t requiredLength = dest->length + valueLength;
    stringReserve(dest, requiredLength);

    // Copy the memory to the end of the destination
    memcpy(&(dest->data[dest->length]), value, valueLength);
    dest->length = requiredLength;
}

int stringContainsCStr(const String* string, const char* value) {
    if(value == NULL) {
        return 0;
    }

    String temp = stringCreate(value);
    int result = stringContains(string, &temp);
    stringFree(&temp);
    return result;
}

int stringStartsWithCStr(const String* string, const char* prefix) {
    if(prefix == NULL) {
        return 0;
    }

    String temp = stringCreate(prefix);
    int result = stringStartsWith(string, &temp);
    stringFree(&temp);
    return result;
}

int stringEndsWithCStr(const String* string, const char* suffix) {
    if(suffix == NULL) {
        return 0;
    }

    String temp = stringCreate(suffix);
    int result = stringEndsWith(string, &temp);
    stringFree(&temp);
    return result;
}

size_t stringIndexOfCStr(const String* string, const char* value, size_t startIndex) {
    if(value == NULL) {
        return STR_NOT_FOUND;
    }

    String temp = stringCreate(value);
    size_t result = stringIndexOf(string, &temp, startIndex);
    stringFree(&temp);
    return result;
}

size_t stringLastIndexOfCStr(const String* string, const char* value, size_t startIndex) {
    if(value == NULL) {
        return STR_NOT_FOUND;
    }

    String temp = stringCreate(value);
    size_t result = stringLastIndexOf(string, &temp, startIndex);
    stringFree(&temp);
    return result;
}

void stringReplaceCStr(String* string, const char* old, const char* newValue) {
    if(old == NULL || newValue == NULL) {
        return;
    }

    String oldStr = stringCreate(old);
    String newStr = stringCreate(newValue);

    stringReplace(string, &oldStr, &newStr);

    stringFree(&oldStr);
    stringFree(&newStr);
}

StringArray stringSplitCStr(const String* string, const char* delimiter) {
    if(delimiter == NULL) {
        return STR_ARRAY_NULL;
    }

    String temp = stringCreate(delimiter);
    StringArray result = stringSplit(string, &temp);
    stringFree(&temp);

    return result;
}

String stringJoinCStr(const char* separator, size_t count, ...) {
    if(separator == NULL) {
        return STR_NULL;
    }

    String sep = stringCreate(separator);

    va_list args;
    va_start(args, count);

    // Collect arguments
    String* strings = malloc(sizeof(String) * count);
    if(strings == NULL) {
        va_end(args);
        stringFree(&sep);
        return STR_NULL;
    }

    for(size_t i = 0; i < count; i++) {
        strings[i] = *va_arg(args, String*);
    }

    va_end(args);

    // Call join using array
    StringArray arr = { strings, count };
    String result = stringJoinArray(&sep, &arr);

    free(strings);
    stringFree(&sep);

    return result;
}

String stringJoinArrayCStr(const char* separator, const StringArray* array) {
    if(separator == NULL) {
        return STR_NULL;
    }

    String sep = stringCreate(separator);
    String result = stringJoinArray(&sep, array);
    stringFree(&sep);

    return result;
}

int stringEqualsCStr(const String* string, const char* value) {
    if(value == NULL) {
        return 0;
    }

    String temp = stringCreate(value);
    int result = stringEquals(string, &temp);
    stringFree(&temp);

    return result;
}

void stringPrintCStr(const char* value) {
    if(value == NULL) {
        return;
    }

    String temp = stringCreate(value);
    stringPrint(&temp);
    stringFree(&temp);
}
