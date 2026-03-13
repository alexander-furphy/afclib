/*

Written by Alexander Furphy, 12 March 2026.

Simple string library with memory management, 
logging, and string manipulation functions.

*/

#ifndef STR_LIB_H
#define STR_LIB_H

#include <stddef.h>
#include <stdio.h>

//
// ---------------- Constants ----------------
//

#define STR_NULL ((String){.data = NULL, .length = 0, .capacity = 0})
#define STR_INVALID(s) (s.data == NULL)
#define STR_ARRAY_NULL ((StringArray){.data = NULL, .length = 0})
#define STR_ARRAY_INVALID (s.data == NULL)
#define STR_STD_PRINT (stdout)

//
// ---------------- Data ----------------
//

/// Stores a string object (data, length, capacity).
typedef struct String {
    char* data;
    size_t length;
    size_t capacity;
} String;

/// Wrapper for an array of string objects.
typedef struct StringArray {
    String* data;
    size_t length;
} StringArray;

//
// ---------------- String Array Functions ----------------
//

/// Allocate a new string array with a specified number of elements.
/// All elements are initialised to 0.
StringArray stringArrayCreate(size_t length);

/// Free a string array.
/// Doesn't free the strings inside the array.
void stringArrayFree(StringArray* array);

/// Free a string array and every string within it.
void stringArrayFreeDeep(StringArray* array);

//
// ---------------- Lifetime and Memory Management ----------------
//

/// Allocate a string from a character array.
/// Will not allocate values 0 bytes in length.
String stringCreate(const char* value);

/// Create a string at a specified starting size.
/// Will not allocate a size of 0.
String stringCreateSize(size_t size);

/// Free a string, with null protection.
void stringFree(String* string);

/// Deep copy another string into a new string.
String stringCopy(String* other);

/// Reallocate the string to specified if it is below the specified size.
/// If the reallocation fails, the original string will be freed and set to null.
void stringReserve(String* string, size_t size);

/// Clear the string, without freeing the buffer.
/// Identical to writing string.length = 0.
void stringClear(String* string);

/// Reallocate a string to it's length, discarding the buffer.
/// If the reallocation fails, the original string will be freed and set to null.
void stringShrinkBuffer(String* string);

//
// ---------------- Formatting and IO ----------------
//

/// Log a string to the specified stream in the following format:
/// [mem = 0x9F28, length = 5, capacity = 17, value = 'Hello']"
void fStringLog(const String* string, FILE* stream);

/// Print a string to the specified stream.
void fStringPrint(const String* string, FILE* stream);

/// Log a string to the standard output stream in the following format:
/// [mem = 0x9F28, length = 5, capacity = 17, value = 'Hello']"
void stringLog(const String* string);

/// Print a string to the standard output stream.
void stringPrint(const String* string);

/// Pass formatted output to a string.
/// Direct binding to the standard C formatting.
void stringSetFormat(String* string, const char* format, ...);

/// Retrieve a heap allocated character array with a null terminator.
char* stringGetCString(String* string);

//
// ---------------- Basic Modification ----------------
//

/// Set an existing string to a character array.
/// Only resizes the buffer if the character array exceeds the size of the buffer.
void stringSet(String* string, const char* value);

/// Append another string to the destination.
void stringAppend(String* dest, String* other);

/// Append a C string to the destination.
void stringAppendCStr(String* dest, const char* value);

/// Insert another string into the string at the specified index.
/// Will not execute if index is greater than the length of the string.
void stringInsert(String* string, String* toInsert, size_t index);

/// Delete a slice of a string, from a specified index range.
/// Will not execute with out of bounds indices.
/// Start cannot be greater than end.
void stringDelete(String* string, size_t start, size_t end);

/// Replace each instance of old with new in the string.
void stringReplace(String* string, String* old, String* new);

/// Repeat a string by a scaler.
void stringScale(String* string, int scaler);

//
// ---------------- Searching and Inspection ----------------
//

/// Compares two strings lexicographically.
int stringCompare(String* a, String* b);
/// Checks if two strings are identical.
int stringEquals(String* a, String* b);
/// Finds the first occurance of string other in the string, starting at startIndex.
size_t stringIndexOf(String* string, String* other, size_t startIndex);
/// Finds the last occurance of string other in the string, starting length - startIndex.
size_t stringLastIndexOf(String* string, String* other, size_t startIndex);
/// Checks if the start of a string is a particular prefix.
int stringStartsWith(String* string, String* prefix);
/// Checks if the end of a string is a particular suffix.
int stringEndsWith(String* string, String* suffix);
/// Check if string other is found in string.
int stringContains(String* string, String* other);

//
// ---------------- Transformation and Cleaning ----------------
//

/// Strips a string of all leading and trailing whitespace.
void stringStrip(String* string);
/// Trims the leading whitespace of a string.
void stringTrimLeft(String* string);
/// Trims the trailing whitespace of a string.
void stringTrimRight(String* string);
/// Convert all lowercase characters in the string to uppercase.
void stringToUpper(String* string);
/// Convert all uppercase characters in the string to lowercase.
void stringToLower(String* string);
/// Reverses the characters in a string.
void stringReverse(String* string);

//
// ---------------- High Level Processing ----------------
//

/// Create a new string from a slice of another string. End is exclusive.
String stringSubstring(String* string, size_t start, size_t end);
/// Splits a string by a deliminator and returns it as a string array.
StringArray stringSplit(String* string, String* delimiter);
/// Join a number of strings as arguments. Pass arguments as string pointers.
String stringJoin(String* seperator, size_t count, ...);
/// Join an array of strings into one.
String stringJoinArray(String* seperator, StringArray* array);

#endif
