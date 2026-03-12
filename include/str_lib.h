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
// ---------------- Data ----------------
//

/// Stores a string object (data, length, capacity).
typedef struct String {
    char* data;
    size_t length;
    size_t capacity;
} String;

//
// ---------------- Memory Functions ----------------
//

/// Allocate a string from a character array.
String stringCreate(const char* value);
/// Free a string (double free protection).
void stringFree(String* string);
/// Reallocate a string to it's length, discarding the buffer.
void stringPurge(String* string);
/// Deeply copy another string.
String stringCopy(String* other);

//
// ---------------- IO Functions ----------------
//

/// Log a string to the specified stream.
void fStringLog(const String* string, FILE* stream);
/// Print a string to the specified stream.
void fStringPrint(const String* string, FILE* stream);
/// Log a string to the standard output stream.
void stringLog(const String* string);
/// Print a string to the standard output stream.
void stringPrint(const String* string);

//
// ---------------- Manipulation Functions ----------------
//

/// Append another string to the destination.
void stringAppend(String* dest, String* other);
/// Multiply a string by a scaler.
void stringMultiply(String* string, int scaler);
/// Create a new string from a slice of another string.
String stringSubstring(String* string, size_t start, size_t end);
/// Strips a string of all leading and trailing whitespace.
void stringStrip(String* string);

//
// ---------------- Searching/Analysing Functions ----------------
//

/// Finds the first occurance of string other in the string.
size_t stringIndexOf(String string, String other);
/// Compares two strings lexicographically.
int stringCompare(String a, String b);
/// Checks if the end of a string is a particular suffix.
int stringEndsWith(String string, String suffix);

#endif