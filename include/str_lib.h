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

#define STR_EMPTY_SIZE 16

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
StringArray stringArrayCreate(size_t length);
/// Free a string array.
void stringArrayFree(StringArray* array);
/// Free a string array and every string within it.
void stringArrayFreeDeep(StringArray* array);

//
// ---------------- Lifetime and Memory Management ----------------
//

/// Allocate a string from a character array.
String stringCreate(const char* value);
/// Create an empty string with a buffer size equal to STR_EMPTY_SIZE.
String stringCreateEmpty(void);
/// Create a string at a specified starting size.
String stringCreateSize(size_t size);
/// Free a string (double free protection).
void stringFree(String* string);
/// Deeply copy another string.
String stringCopy(String* other);
/// Reallocate the buffer of the string to size bytes if it is below size bytes.
void stringReserve(String* string, size_t size);
/// Clear the string, but retain buffer.
void stringClear(String* string);
/// Reallocate a string to it's length, discarding the buffer.
void stringShrinkBuffer(String* string);

//
// ---------------- Formatting and IO ----------------
//

/// Log a string to the specified stream.
void fStringLog(const String* string, FILE* stream);
/// Print a string to the specified stream.
void fStringPrint(const String* string, FILE* stream);
/// Log a string to the standard output stream.
void stringLog(const String* string);
/// Print a string to the standard output stream.
void stringPrint(const String* string);
/// Pass formatted output to a string.
void stringSetFormat(String* string, const char* format, ...);
/// Retrieve a heap allocated character array with a null terminator.
char* stringGetCString(String* string);

//
// ---------------- Basic Modification ----------------
//

/// Set an existing string to a character array, reusing the buffer.
void stringSet(String* string, const char* value);
/// Append another string to the destination.
void stringAppend(String* dest, String* other);
/// Append a C string to the destination.
void stringAppendCStr(String* dest, const char* value);
/// Insert toInsert into the string, at the specified index.
void stringInsert(String* string, String* toInsert, size_t index);
/// Delete a slice of a string, from a specified index range.
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
