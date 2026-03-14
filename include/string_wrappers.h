#ifndef STRING_WRAPPERS_H
#define STRING_WRAPPERS_H

#include "string_lib.h"

//
// ---------------- C String Convenience Wrappers ----------------
//

/**
 * Convenience wrappers that allow standard C strings to be used with the
 * String API. These functions internally create temporary String objects
 * and call the corresponding core String functions.
 *
 * Behaviour is otherwise identical to the underlying String operations.
 * These wrappers are provided for convenience when working with string
 * literals or null-terminated character arrays.
 */

/// Set the value of a string from a C string.
/// Behaviour is identical to stringSet.
void stringSetCStr(String* string, const char* value);

/// Append a C string to the end of a string.
/// Behaviour is identical to stringAppend.
void stringAppendCStr(String* dest, const char* value);

/// Check if a string contains a C string.
/// Behaviour is identical to stringContains.
int stringContainsCStr(const String* string, const char* value);

/// Check if a string begins with a C string prefix.
/// Behaviour is identical to stringStartsWith.
int stringStartsWithCStr(const String* string, const char* prefix);

/// Check if a string ends with a C string suffix.
/// Behaviour is identical to stringEndsWith.
int stringEndsWithCStr(const String* string, const char* suffix);

/// Find the first occurrence of a C string within a string starting at startIndex.
/// Behaviour is identical to stringIndexOf.
size_t stringIndexOfCStr(const String* string, const char* value, size_t startIndex);

/// Find the last occurrence of a C string within a string starting from length - startIndex.
/// Behaviour is identical to stringLastIndexOf.
size_t stringLastIndexOfCStr(const String* string, const char* value, size_t startIndex);

/// Replace all occurrences of a C string with another C string.
/// Behaviour is identical to stringReplace.
void stringReplaceCStr(String* string, const char* old, const char* newValue);

/// Split a string using a C string delimiter.
/// Behaviour is identical to stringSplit.
StringArray stringSplitCStr(const String* string, const char* delimiter);

/// Join multiple strings using a C string separator.
/// Behaviour is identical to stringJoin.
String stringJoinCStr(const char* separator, size_t count, ...);

/// Join an array of strings using a C string separator.
/// Behaviour is identical to stringJoinArray.
String stringJoinArrayCStr(const char* separator, const StringArray* array);

/// Compare a string with a C string for equality.
/// Behaviour is identical to stringEquals.
int stringEqualsCStr(const String* string, const char* value);

/// Print a C string using the library's printing system.
void stringPrintCStr(const char* value);

#endif // STRING_WRAPPERS_H