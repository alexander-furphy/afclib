#include "str_lib.h"
#include <string.h>
#include <stdlib.h>

#define STR_ALLOC_FAIL_MSG "Failed to allocate string of length"
#define STR_NULL (String){0}
#define STR_PRINT_STREAM stdout

//
// ---------------- Static functions ----------------
//

static String stringResize(String string, size_t size) {
    size_t newLength = size;
    string.data = realloc(string.data, newLength);
    if(string.data == NULL) {
        fprintf(stderr, STR_ALLOC_FAIL_MSG " %zu.\n", newLength);
        return STR_NULL;
    }
    string.capacity = newLength;
    return string;
}

//
// ---------------- Memory Functions ----------------
//

String stringCreate(const char* value) {
    // strlen exlcudes null terminator, which is desired
    size_t length = strlen(value);

    // Allocate and validate memory
    char* data = malloc(length * sizeof(char));
    if(data == NULL) {
        fprintf(stderr, STR_ALLOC_FAIL_MSG " %zu.\n", length);
        return STR_NULL;
    }
    memcpy(data, value, length);

    return (String){data, length, length};
}

String stringFree(String string) {
    // Double free guard
    if(string.data == NULL) {
        fprintf(stderr, "Attempted free on null string.\n");
    }
    else {
        free(string.data);
    }
    
    return STR_NULL;
}

String stringPurge(String string) {
    string.data = realloc(string.data, string.length);
    if(string.data == NULL) {
        fprintf(stderr, STR_ALLOC_FAIL_MSG " %zu.\n", string.length);
        string = (String){0};
    }
    else {
        string.capacity = string.length;
    }

    return string;
}

String stringCopy(String other) {
    // Allocate and validate memory
    char* data = malloc(other.length * sizeof(char));
    if(data == NULL) {
        fprintf(stderr, STR_ALLOC_FAIL_MSG " %zu.\n", other.length);
        return (String){NULL, 0, 0};
    }
    memcpy(data, other.data, other.length);
    
    return (String){data, other.length, other.length};
}

//
// ---------------- IO Functions ----------------
//

void fStringLog(String string, FILE* stream) {
    // Print pointer, length and capacity
    fprintf(
        stream,
        "[mem = %p, length = %zu, capacity = %zu, value = '", 
        string.data, string.length, string.capacity
    );
    
    // Directly write the data
    fwrite(string.data, sizeof(char), string.length, stream);
    fprintf(stream, "']\n");
}

void fStringPrint(String string, FILE* stream) {
    fwrite(string.data, sizeof(char), string.length, stream);
}

void stringLog(String string) {
    fStringLog(string, STR_PRINT_STREAM);
}

void stringPrint(String string) {
    fStringPrint(string, STR_PRINT_STREAM);
}

//
// ---------------- Manipulation Functions ----------------
//

String stringAppend(String string, String other) {
    size_t requiredLength = string.length + other.length;
    if(string.capacity < requiredLength) {
        string = stringResize(string, requiredLength);
    }
    memcpy(&(string.data[string.length]), other.data, other.length);
    string.length = requiredLength;
    return string;
}

String stringMultiply(String string, int scaler) {
    size_t requiredLength = string.length * scaler;
    if(string.capacity < requiredLength) {
        string = stringResize(string, requiredLength);
    }
    for(int i = 1; i <= scaler; i++) {
        memcpy(&(string.data[string.length * i]), string.data, string.length);
    }
    string.length = requiredLength;
    return string;
}

String stringSubstring(String string, size_t a, size_t b) {
    size_t requiredSize = b - a;

    char* data = malloc(requiredSize * sizeof(char));
    if(data == NULL) {
        fprintf(stderr, STR_ALLOC_FAIL_MSG " %zu.\n", requiredSize);
        return STR_NULL;
    }

    memcpy(data, &(string.data[a]), requiredSize);

    return (String){data, requiredSize, requiredSize};
}

int stringCompare(String a, String b) {
    size_t smallest = a.length < b.length ? a.length : b.length;

    for(size_t i = 0; i < smallest; i++) {
        if(a.data[i] < b.data[i]) return -1;
        if(a.data[i] > b.data[i]) return 1;
    }

    return 0;
}

int stringEndsWith(String string, String suffix) {
    if(suffix.length > string.length) {
        return 0;
    }

    for(size_t i = 0; i < suffix.length; i++) {
        if(string.data[string.length - suffix.length + i] != suffix.data[i]) {
            return 0;
        }
    }

    return 1;
}
