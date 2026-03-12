#include "str_lib.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>

#define STR_ALLOC_FAIL_MSG "Failed to allocate string of length"
#define STR_NULL (String){0}
#define STR_IS_NULL(s) (s == NULL || s->data == NULL)
#define STR_PRINT_STREAM stdout
#define NOT_FOUND ((size_t)-1)

//
// ---------------- Lifetime Functions ----------------
//

String stringCreate(const char* value) {
    // strlen exlcudes null terminator, which is desired
    size_t length = strlen(value);
    String string = stringCreateSize(length);
    memcpy(string.data, value, length);

    return string;
}

String stringCreateEmpty(void) {
    return stringCreateSize(STR_EMPTY_SIZE);
}

String stringCreateSize(size_t size) {
    char* data = malloc(size * sizeof(char));
    if(data == NULL) {
        fprintf(stderr, STR_ALLOC_FAIL_MSG " %zu.\n", size);
        return STR_NULL;
    }
    return (String){data, 0, size};
}

void stringFree(String* string) {
    if(string == NULL) return;

    // Double free guard
    if(string->data == NULL) {
        fprintf(stderr, "Attempted free on null string.\n");
        return;
    }

    free(string->data);
    
    *string = STR_NULL;
}

String stringCopy(String* other) {
    if(STR_IS_NULL(other)) return STR_NULL;

    // Allocate and validate memory
    char* data = malloc(other->length * sizeof(char));
    if(data == NULL) {
        fprintf(stderr, STR_ALLOC_FAIL_MSG " %zu.\n", other->length);
        return STR_NULL;
    }
    memcpy(data, other->data, other->length);
    
    return (String){data, other->length, other->length};
}

void stringReserve(String* string, size_t size) {
    // Don't allocate if the string buffer is already at the minimum size.
    if(string->capacity >= size) return;

    size_t newLength = size;
    void* temp = realloc(string->data, newLength);
    if(temp == NULL) {
        fprintf(stderr, STR_ALLOC_FAIL_MSG " %zu.\n", newLength);
        stringFree(string);
        *string = STR_NULL;
    }
    else {
        string->data = temp;
        string->capacity = newLength;
    }
}

void stringClear(String* string) {
    string->length = 0;
}

void stringPurge(String* string) {
    if(STR_IS_NULL(string)) return;

    string->data = realloc(string->data, string->length);
    if(string->data == NULL) {
        fprintf(stderr, STR_ALLOC_FAIL_MSG " %zu.\n", string->length);
        *string = STR_NULL;
    }
    else {
        string->capacity = string->length;
    }
}

//
// ---------------- Formatting and IO ----------------
//

void fStringLog(const String* string, FILE* stream) {
    if(STR_IS_NULL(string)) return;

    // Print pointer, length and capacity
    fprintf(
        stream,
        "[mem = %p, length = %zu, capacity = %zu, value = '", 
        string->data, string->length, string->capacity
    );
    
    // Directly write the data
    fwrite(string->data, sizeof(char), string->length, stream);
    fprintf(stream, "']\n");
}

void fStringPrint(const String* string, FILE* stream) {
    if(STR_IS_NULL(string)) return;
    fwrite(string->data, sizeof(char), string->length, stream);
}

void stringLog(const String* string) {
    fStringLog(string, STR_PRINT_STREAM);
}

void stringPrint(const String* string) {
    fStringPrint(string, STR_PRINT_STREAM);
}

void stringSetFormat(String* string, const char* format, ...) {
    va_list args, argsCopy;
    va_start(args, format);
    va_copy(argsCopy, args);

    int length = vsnprintf(NULL, 0, format, argsCopy);
    va_end(argsCopy);

    // Formatting error check
    if(length < 0) {
        va_end(args);
        return;
    }

    if(string->capacity < (size_t)(length + 1)) {
        stringReserve(string, length + 1);
    }

    vsnprintf(string->data, (size_t)length + 1, format, args);
    string->length = length;

    va_end(args);
}

char* stringGetCString(String* string) {
    // Add an extra byte for the null terminator
    char* data = malloc((string->length + 1) * sizeof(char));
    if(data == NULL) {
        fprintf(stderr, STR_ALLOC_FAIL_MSG " %zu.\n", string->length);
        return NULL;
    }
    
    // Copy the string into the array
    memcpy(data, string->data, string->length);

    // Add a null terminator
    data[string->length] = '\0';

    return data;
}

//
// ---------------- Manipulation Functions ----------------
//

void stringSet(String* string, const char* value) {
    size_t length = strlen(value);
    if(string->capacity < length) {
        stringReserve(string, length);
    }
    memcpy(string->data, value, length);
    string->length = length;
}

void stringAppend(String* string, String* other) {
    size_t requiredLength = string->length + other->length;
    if(string->capacity < requiredLength) {
        stringReserve(string, requiredLength);
    }
    memcpy(&(string->data[string->length]), other->data, other->length);
    string->length = requiredLength;
}

void stringScale(String* string, int scaler) {
    size_t requiredLength = string->length * scaler;
    if(string->capacity < requiredLength) {
        stringReserve(string, requiredLength);
    }
    for(int i = 1; i < scaler; i++) {
        memcpy(&(string->data[string->length * i]), string->data, string->length);
    }
    string->length = requiredLength;
}

//
// ---------------- Searching and Inspection ----------------
//

int stringCompare(String* a, String* b) {
    size_t smallest = a->length < b->length ? a->length : b->length;

    for(size_t i = 0; i < smallest; i++) {
        if(a->data[i] < b->data[i]) return -1;
        if(a->data[i] > b->data[i]) return 1;
    }

    return 0;
}

size_t stringIndexOf(String* string, String* other) {
    if(other->length > string->length || other->length == 0) return NOT_FOUND;

    for(size_t i = 0; i <= string->length - other->length; i++) {
        // Check if the segment of the string is equal
        if(!memcmp(&(string->data[i]), other->data, other->length)) {
            return i;
        }
    }

    return NOT_FOUND;
}

String stringSubstring(String* string, size_t start, size_t end) {
    if(start > end || start > string->length || end > string->length) {
        fprintf(stderr, "Invalid substr ptrs: start=%zu, end=%zu.\n", start, end);
        return STR_NULL;
    }

    size_t requiredSize = end - start;

    char* data = malloc(requiredSize * sizeof(char));
    if(data == NULL) {
        fprintf(stderr, STR_ALLOC_FAIL_MSG " %zu.\n", requiredSize);
        return STR_NULL;
    }

    memcpy(data, &(string->data[start]), requiredSize);

    return (String){data, requiredSize, requiredSize};
}

int stringEndsWith(String* string, String* suffix) {
    if(suffix->length > string->length) {
        return 0;
    }

    for(size_t i = 0; i < suffix->length; i++) {
        if(string->data[string->length - suffix->length + i] != suffix->data[i]) {
            return 0;
        }
    }

    return 1;
}

//
// ---------------- Transformation and Cleaning ----------------
//

void stringStrip(String* string) {
    if(string->length == 0) return;

    size_t start = 0;
    while(start < string->length && isspace((unsigned char)string->data[start])) {
        start++;
    }

    if(start == string->length) {
        String empty = stringCreate("");
        stringFree(string);
        *string = empty;
        return;
    }

    size_t end = string->length - 1;
    while(end > start && isspace((unsigned char)string->data[end])) {
        end--;
    }

    String newStr = stringSubstring(string, start, end + 1);
    stringFree(string);
    *string = newStr;
}
