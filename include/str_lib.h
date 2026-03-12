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
void stringPurge(String* string);

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
/// Repeat a string by a scaler.
void stringScale(String* string, int scaler);

//
// ---------------- Searching and Inspection ----------------
//

/// Create a new string from a slice of another string.
String stringSubstring(String* string, size_t start, size_t end);
/// Finds the first occurance of string other in the string.
size_t stringIndexOf(String* string, String* other);
/// Compares two strings lexicographically.
int stringCompare(String* a, String* b);
/// Checks if the end of a string is a particular suffix.
int stringEndsWith(String* string, String* suffix);

//
// ---------------- Transformation and Cleaning ----------------
//

/// Strips a string of all leading and trailing whitespace.
void stringStrip(String* string);

//
//
//

#endif