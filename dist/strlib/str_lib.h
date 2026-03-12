/*

Written by Alexander Furphy, 12 March 2026.

Simple string library with memory management, 
logging, and string manipulation functions.

*/

#ifndef STR_LIB_H
#define STR_LIB_H

#include <stddef.h>
#include <stdio.h>

/// Stores a string object (data, length, capacity).
typedef struct String {
    char* data;
    size_t length;
    size_t capacity;
} String;

/// Allocate a string from a character array.
String stringCreate(const char* value);
/// Free a string (double free protection).
String stringFree(String string);
/// Reallocate a string to it's length, discarding the buffer.
String stringPurge(String string);
/// Deeply copy another string.
String stringCopy(String other);

/// Log a string to the specified stream.
void fStringLog(String string, FILE* stream);
/// Print a string to the specified stream.
void fStringPrint(String string, FILE* stream);
/// Log a string to the standard output stream.
void stringLog(String string);
/// Print a string to the standard output stream.
void stringPrint(String string);

/// Append another string to the destination.
String stringAppend(String dest, String other);
/// Multiply a string by a scaler.
String stringMultiply(String string, int scaler);
/// Create a new string from a slice of another string.
String stringSubstring(String string, size_t a, size_t b);

#endif