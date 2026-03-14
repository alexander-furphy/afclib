/**

-------------------------------- Memory Model --------------------------------

Strings have an internal buffer that expands as the string requires more
memory. The buffer will not shrink to fit the string's length automatically.
If that is required, use the stringShrinkBuffer function.

-------------------------------- Important --------------------------------

Strings are not null terminated. Do not try to manually print the data
member of string objects. If a C string is required, use the stringGetCString
function, which returns a heap allocated character array that has a null
terminator. The caller is responsible for freeing this character array.

-------------------------------- Author --------------------------------

Written by Alexander Furphy.

14 March 2026.

*/

#ifndef STR_LIB_H
#define STR_LIB_H

//
// ---------------- Includes ----------------
//

#include <stddef.h>
#include <stdio.h>

//
// ---------------- Macros ----------------
//

/// Represents a null string.
#define STR_NULL ((String){.data = NULL, .length = 0, .capacity = 0})

/// Boolean check to see if a string is valid.
/// Does not work for string pointers.
#define STR_INVALID(s) ((s).data == NULL)

/// Represents a null string array.
#define STR_ARRAY_NULL ((StringArray){.data = NULL, .length = 0})

/// Boolean check to see if a string array is valid.
/// Does not work for string array pointers.
#define STR_ARRAY_INVALID(s) ((s).data == NULL)

/// Output stream used by all IO functions that don't specify a stream.
#define STR_STD_PRINT (stdout)

/// Code for not found. Used for seaching functions such as indexOf.
#define STR_NOT_FOUND ((size_t)-1)

//
// ---------------- Data ----------------
//

/// Stores a string object (data, length, capacity).
/// Do not assign data, length, or capacity manually.
typedef struct String {
    char* data;
    size_t length;
    size_t capacity;
} String;

/// Wrapper for an array of string objects.
/// Do not assign length or data manually.
typedef struct StringArray {
    String* data;
    size_t length;
} StringArray;

//
// ---------------- String Array Functions ----------------
//

/// Allocate a new string array with a specified number of elements.
/// All elements are initialised to 0.
StringArray stringArrayCreate(const size_t length);

/// Free a string array.
/// Doesn't free the strings inside the array.
void stringArrayFree(StringArray* array);

/// Free a string array and every string within it.
void stringArrayFreeDeep(StringArray* array);

//
// ---------------- Lifetime and Memory Management ----------------
//

/// Allocate a new string and copies the character array into it.
/// Will not allocate values 0 bytes in length.
String stringCreate(const char* value);

/// Create a string at a specified starting size.
/// Will not allocate a size of 0.
String stringCreateSize(const size_t size);

/// Free a string, with null protection.
void stringFree(String* string);

/// Deep copy another string into a new string.
String stringCopy(const String* other);

/// Reallocate the string to specified if it is below the specified size.
/// If the reallocation fails, the original string will be freed and set to null.
void stringReserve(String* string, const size_t size);

/// Clear the string, without freeing the buffer.
/// Identical to writing string.length = 0.
void stringClear(String* string);

/// Reallocate a string to it's length, discarding the buffer.
/// If the reallocation fails, the original string will be freed and set to null.
void stringShrinkBuffer(String* string);

//
// ---------------- Formatting and IO ----------------
//

/// Read the whole contents of a file into a string.
String stringReadFile(const char* file);

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

/// Get a copy of the string returned as a heap allocated C string.
/// Caller is responsible for freeing the character array.
char* stringGetCString(const String* string);

//
// ---------------- Basic Modification ----------------
//

/// Copy a character array into an existing string.
/// Only resizes the buffer if the character array exceeds the size of the buffer.
void stringSet(String* string, const char* value);

/// Append another string to the destination.
void stringAppend(String* dest, const String* other);

/// Append a C string to the destination.
void stringAppendCStr(String* dest, const char* value);

/// Insert another string into the string at the specified index.
/// Will not execute if index is greater than the length of the string.
void stringInsert(String* string, const String* toInsert, const size_t index);

/// Delete a slice of a string, from a specified index range.
/// Will not execute with out of bounds indices.
/// Start cannot be greater than end.
void stringDelete(String* string, const size_t start, const size_t end);

/// Replace each instance of old with new in the string.
void stringReplace(String* string, const String* old, const String* new);

/// Repeat a string by a scaler.
void stringScale(String* string, const int scaler);

//
// ---------------- Searching and Inspection ----------------
//

/// Compares two strings lexicographically.
/// If either string is null, 0 is returned.
int stringCompare(const String* a, const String* b);

/// Checks if two strings are identical.
/// If either string is null, 0 is returned.
int stringEquals(const String* a, const String* b);

/// Finds the first occurance of string other in the string, starting at startIndex.
/// If the string isn't found or either string is null STR_NOT_FOUND is returned.
size_t stringIndexOf(const String* string, const String* other, const size_t startIndex);

/// Finds the last occurance of string other in the string, starting length - startIndex.
/// If the string isn't found or either string is null STR_NOT_FOUND is returned.
size_t stringLastIndexOf(const String* string, const String* other, const size_t startIndex);

/// Checks if the start of a string is a particular prefix.
/// Returns 0 if either string is null.
int stringStartsWith(const String* string, const String* prefix);

/// Checks if the end of a string is a particular suffix.
/// Returns 0 if either string is null.
int stringEndsWith(const String* string, const String* suffix);

/// Check if string other is found in string.
/// Returns 0 if either string is null.
int stringContains(const String* string, const String* other);

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

/// Create a new string from a slice of another string.
/// Start is inclusive and end is exclusive.
String stringSubstring(const String* string, const size_t start, const size_t end);

/// Splits a string by a deliminator and returns it as a string array
/// containing newly allocated strings. Will return null on failure.
StringArray stringSplit(const String* string, const String* delimiter);

/// Join a number of strings as variadic arguments, with the number specified by count.
/// Strings will be joined by the seperator and returned as a newly allocated string.
String stringJoin(const String* seperator, const size_t count, ...);

/// Join an array of strings into one.
/// Strings will be joined by the seperator and returned as a newly allocated string.
String stringJoinArray(const String* seperator, const StringArray* array);

#endif // STR_LIB_H
