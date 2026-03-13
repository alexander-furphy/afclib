#include "str_lib.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>

#define STR_ALLOC_FAIL_MSG "Failed to allocate string of length"
#define STR_IS_NULL(s) (s == NULL || s->data == NULL)
#define STR_ARRAY_IS_NULL(a) (a == NULL || a->data == NULL)
#define STR_NOT_FOUND ((size_t)-1)

//
// ---------------- String Array Functions ----------------
//

StringArray stringArrayCreate(size_t length) {
    // Initialise to zero to avoid user trying to use garbage values
    String* data = calloc(length, sizeof(String));

    if(data == NULL) {
        return STR_ARRAY_NULL;
    }

    return (StringArray){data, length};
}

void stringArrayFree(StringArray* array) {
    if(STR_ARRAY_IS_NULL(array)) {
        return;
    }

    // Free and set to null
    free(array->data);
    *array = STR_ARRAY_NULL;
}

void stringArrayFreeDeep(StringArray* array) {
    if(STR_ARRAY_IS_NULL(array)) {
        return;
    }

    // Free every string within the array
    for(size_t i = 0; i < array->length; i++) {
        stringFree(&(array->data[i]));
    }

    stringArrayFree(array);
}

//
// ---------------- Lifetime Functions ----------------
//

String stringCreate(const char* value) {
    if(value == NULL) {
        return STR_NULL;
    }

    // strlen exlcudes null terminator, which is desired
    size_t length = strlen(value);

    if(length == 0) {
        return STR_NULL;
    }

    String string = stringCreateSize(length);

    if(STR_INVALID(string)) {
        return STR_NULL;
    }

    // Only copy if string was created properly
    memcpy(string.data, value, length);
    string.length = length;

    return string;
}

String stringCreateSize(size_t size) {
    if(size == 0) {
        return STR_NULL;
    }

    char* data = malloc(size * sizeof(char));

    if(data == NULL) {
        return STR_NULL;
    }

    return (String){data, 0, size};
}

void stringFree(String* string) {
    if(STR_IS_NULL(string)) {
        return;
    }

    free(string->data);
    *string = STR_NULL;
}

String stringCopy(String* other) {
    if(STR_IS_NULL(other)) {
        return STR_NULL;
    }

    // Allocate and validate memory
    char* data = malloc(other->length * sizeof(char));
    if(data == NULL) {
        return STR_NULL;
    }

    memcpy(data, other->data, other->length);
    
    return (String){data, other->length, other->length};
}

void stringReserve(String* string, size_t size) {
    if(STR_IS_NULL(string)) {
        return;
    }

    // Don't allocate if the string buffer is already at the minimum size.
    if(string->capacity >= size) {
        return;
    }

    // Reallocate into a new pointer
    void* temp = realloc(string->data, size);

    // If the reallocation failed, free the original
    if(temp == NULL) {
        stringFree(string);
        *string = STR_NULL;
    }
    else {
        string->data = temp;
        string->capacity = size;
    }
}

void stringClear(String* string) {
    string->length = 0;
}

void stringShrinkBuffer(String* string) {
    if(STR_IS_NULL(string)) {
        return;
    }

    // Reallocate into a new pointer
    void* temp = realloc(string->data, string->length);

    // If the reallocation failed, free the original
    if(temp == NULL) {
        stringFree(string);
        *string = STR_NULL;
    }
    else {
        string->data = temp;
        string->capacity = string->length;
    }
}

//
// ---------------- Formatting and IO ----------------
//

void fStringLog(const String* string, FILE* stream) {
    if(STR_IS_NULL(string)) {
        return;
    }

    // Print pointer, length and capacity
    fprintf(
        stream,
        "[mem = %p, length = %zu, capacity = %zu, value = '", 
        string->data, string->length, string->capacity
    );
    
    // Directly write the data from the string
    fwrite(string->data, sizeof(char), string->length, stream);

    fprintf(stream, "']\n");
}

void fStringPrint(const String* string, FILE* stream) {
    if(STR_IS_NULL(string)) {
        return;
    }

    fwrite(string->data, sizeof(char), string->length, stream);
}

void stringLog(const String* string) {
    fStringLog(string, STR_STD_PRINT);
}

void stringPrint(const String* string) {
    fStringPrint(string, STR_STD_PRINT);
}

void stringSetFormat(String* string, const char* format, ...) {
    if(STR_IS_NULL(string)) {
        return;
    }

    va_list args, argsCopy;
    va_start(args, format);

    // Take a copy for the length calculation.
    va_copy(argsCopy, args);

    // Calculate the length required for the string.
    int length = vsnprintf(NULL, 0, format, argsCopy);
    va_end(argsCopy);

    // Formatting error check.
    if(length < 0) {
        va_end(args);
        return;
    }

    stringReserve(string, length + 1);

    // Write the data to the string.
    vsnprintf(string->data, (size_t)length + 1, format, args);
    string->length = length;

    va_end(args);
}

char* stringGetCString(String* string) {
    if(STR_IS_NULL(string)) {
        return NULL;
    }

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
    if(STR_IS_NULL(string)) {
        return;
    }

    size_t length = strlen(value);
    stringReserve(string, length);

    // Copy the data into the string.
    memcpy(string->data, value, length);
    string->length = length;
}

void stringAppend(String* dest, String* other) {
    if(STR_IS_NULL(dest) || STR_IS_NULL(other)) {
        return;
    }

    size_t requiredLength = dest->length + other->length;

    // Resize if needed
    stringReserve(dest, requiredLength);

    // Copy the data to the end of the destination
    memcpy(&(dest->data[dest->length]), other->data, other->length);
    dest->length = requiredLength;
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

void stringInsert(String* string, String* toInsert, size_t index) {
    if(STR_IS_NULL(string) || STR_IS_NULL(toInsert) || index > string->length) {
        return;
    }

    // Calculate the required length
    size_t requiredLength = string->length + toInsert->length;
    stringReserve(string, requiredLength);

    // Calculate the destionation and copy size
    size_t dest = index + toInsert->length;
    size_t copySize = string->length - index;

    // Move the second part of the string to make room for the insertion
    memmove(&(string->data[dest]), &(string->data[index]), copySize);

    // Copy the insertion into the string.
    memcpy(&(string->data[index]), toInsert->data, toInsert->length);

    string->length = requiredLength;
}

void stringDelete(String* string, size_t start, size_t end) {
    if(STR_IS_NULL(string) || start > string->length || end > string->length || start > end) {
        return;
    }

    // Calculate the amount to shift the second half to the left
    size_t shiftAmount = string->length - end;

    // Move the second part of the string to the left.
    memmove(&(string->data[start]), &(string->data[end]), shiftAmount);
    string->length -= end - start;
}

void stringReplace(String* string, String* old, String* new) {
    // Return if any string is null or the old is 0
    if(STR_IS_NULL(string) || STR_IS_NULL(old) || STR_IS_NULL(new) || old->length == 0) {
        return;
    }

    // Difference 
    signed long lengthDiff = new->length - old->length;
    
    // Search until the end of the string
    size_t currentPos = 0;
    while(currentPos <= (string->length - old->length)) {
        // Find a match starting from the current position
        size_t matchIndex = stringIndexOf(string, old, currentPos);

        // If not found, no strings are left to substitutes
        if(matchIndex == STR_NOT_FOUND) {
            break;
        }

        // Calculate the length of the string suffix
        size_t suffixStart = matchIndex + old->length;
        size_t suffixLength = string->length - suffixStart;

        // Resize if required
        stringReserve(string, string->length + lengthDiff);

        // Don't move if suffix is of length 0
        if(suffixLength > 0) {
            // Move the suffix to make room for the substitution of new
            memmove(
                &(string->data[matchIndex + new->length]),
                &(string->data[suffixStart]), 
                suffixLength
            );
        }

        // Copy the new string into the empty space between the prefix and suffix string
        memcpy(&(string->data[matchIndex]), new->data, new->length);

        // Update string data
        string->length += lengthDiff;

        // Advance current position
        currentPos = matchIndex + new->length;
    }
}

void stringScale(String* string, int scaler) {
    if(STR_IS_NULL(string) || scaler <= 1) {
        return;
    }

    size_t originalLength = string->length;
    size_t requiredLength = string->length * scaler;

    stringReserve(string, requiredLength);

    // Repetively double the string until it no longer fits into a power of two.
    size_t currentLength = originalLength;
    while(currentLength * 2 <= requiredLength) {
        memcpy(string->data + currentLength, string->data, currentLength);
        currentLength *= 2;
    }

    // Fill in the remaining slots
    if(currentLength < requiredLength) {
        memcpy(string->data + currentLength, string->data, requiredLength - currentLength);
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

int stringEquals(String* a, String* b) {
    // If the strings are null or of different lengths, they are not equal.
    if(STR_IS_NULL(a) || STR_IS_NULL(b) || a->length != b->length) {
        return 0;
    }

    // Loop through each character
    for(size_t i = 0; i < a->length; i++) {
        // Return false if the current characters are unequal
        if(a->data[i] != b->data[i]) {
            return 0;
        }
    }

    // Strings are identical, return true
    return 1;
}

size_t stringIndexOf(String* string, String* other, size_t startIndex) {
    if(other->length > string->length || other->length == 0) {
        return STR_NOT_FOUND;
    }

    for(size_t i = startIndex; i <= string->length - other->length; i++) {
        // Check if the segment of the string is equal
        if(!memcmp(&(string->data[i]), other->data, other->length)) {
            return i;
        }
    }

    return STR_NOT_FOUND;
}

size_t stringLastIndexOf(String* string, String* other, size_t startIndex) {
    if(other->length > string->length || other->length == 0) {
        return STR_NOT_FOUND;
    }

    for(size_t i = string->length - other->length - 1 - startIndex; i >= 0; i--) {
        if(!memcmp(&(string->data[i]), other->data, other->length)) {
            return i;
        }
    }
    
    return STR_NOT_FOUND;
}

int stringStartsWith(String* string, String* prefix) {
    if(prefix->length > string->length) {
        return 0;
    }

    for(size_t i = 0; i < prefix->length; i++) {
        if(string->data[i] != prefix->data[i]) {
            return 0;
        }
    }

    return 1;
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

int stringContains(String* string, String* other) {
    return stringIndexOf(string, other, 0) != STR_NOT_FOUND;
}

//
// ---------------- Transformation and Cleaning ----------------
//

void stringStrip(String* string) {
    stringTrimLeft(string);
    stringTrimRight(string);
}

void stringTrimLeft(String* string) {
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

    String newStr = stringSubstring(string, start, string->length);
    stringFree(string);
    *string = newStr;
}

void stringTrimRight(String* string) {
    if(string->length == 0) return;

    // Keep decrementing until the current character is not a space
    size_t end = string->length - 1;
    while(end > 0 && isspace((unsigned char)string->data[end])) {
        end--;
    }

    if(end == 0) {
        String empty = stringCreate("");
        stringFree(string);
        *string = empty;
        return;
    }

    // Adding one because substring exclusive
    String newStr = stringSubstring(string, 0, end + 1);
    stringFree(string);
    *string = newStr;
}

void stringToUpper(String* string) {
    if(STR_IS_NULL(string) || string->length == 0) {
        return;
    }

    for(size_t i = 0; i < string->length; i++) {
        string->data[i] = (char)toupper((uint8_t)string->data[i]);
    }
}

void stringToLower(String* string) {
    if(STR_IS_NULL(string) || string->length == 0) {
        return;
    }

    for(size_t i = 0; i < string->length; i++) {
        string->data[i] = (char)tolower((uint8_t)string->data[i]);
    }
}

void stringReverse(String* string) {
    String copy = stringCopy(string);

    // Copy each character in reverse
    for(size_t i = 0; i < string->length; i++) {
        // Subtract one, because when i = 0 the last element of copy should be read
        string->data[i] = copy.data[string->length - i - 1];
    }

    stringFree(&copy);
}

//
// ---------------- High Level Processing ----------------
//

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

StringArray stringSplit(String* string, String* delimiter) {
    // A split string will have at least one element
    size_t requiredElements = 1;
    size_t currentIndex = 0;
    while(1) {
        currentIndex = stringIndexOf(string, delimiter, currentIndex);
        
        if(currentIndex == STR_NOT_FOUND) {
            break;
        }

        currentIndex += delimiter->length;
        requiredElements++;
    }

    StringArray array = stringArrayCreate(requiredElements);

    currentIndex = 0;
    size_t lastIndex = currentIndex;
    for(size_t i = 0; i < requiredElements - 1; i++) {
        currentIndex = stringIndexOf(string, delimiter, currentIndex);
        currentIndex += delimiter->length;

        // Add a new string
        array.data[i] = stringSubstring(string, lastIndex, currentIndex);

        lastIndex = currentIndex;
    }

    currentIndex = string->length;
    array.data[requiredElements - 1] = stringSubstring(string, lastIndex, currentIndex);

    return array;
}

String stringJoin(String* seperator, size_t count, ...) {
    va_list args, argsCopy;
    va_start(args, count);

    // Calculate length
    va_copy(argsCopy, args);

    size_t requiredLength = 0;
    for(size_t i = 0; i < count; i++) {
        requiredLength += va_arg(argsCopy, String*)->length;

        // Add a seperator for all but the last element
        if(i < count - 1) {
            requiredLength += seperator->length;
        }
    }

    va_end(argsCopy);

    // Build a new string
    String string = stringCreateSize(requiredLength);
    for(size_t i = 0; i < count; i++) {
        stringAppend(&string, va_arg(args, String*));

        // Add a seperator for all but the last element
        if(i < count - 1) {
            stringAppend(&string, seperator);
        }
    }

    va_end(args);

    return string;
}

String stringJoinArray(String* seperator, StringArray* array) {
    // Calculate the required length
    size_t requiredLength = 0;
    for(size_t i = 0; i < array->length; i++) {
        requiredLength += array->data[i].length;

        // Add a seperator for all but the last element
        if(i < (array->length - 1)) {
            requiredLength += seperator->length;
        }
    }

    // Build a new string
    String string = stringCreateSize(requiredLength);
    for(size_t i = 0; i < array->length; i++) {
        stringAppend(&string, &array->data[i]);

        // Add a seperator for all but the last element
        if(i < (array->length - 1)) {
            stringAppend(&string, seperator);
        }
    }

    return string;
}
