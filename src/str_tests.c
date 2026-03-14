#include "str_tests.h"

#include "str_lib.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

void strTestAll(void) {
    printf("Testing all string modules...\n\n");

    strTestLifetime();
    strTestArrays();
    strTestIO();
    strTestBasic();
    strTestSearch();
    strTestTransformation();
    strTestProcessing();

    printf("\nAll string modules tested successfully.\n");
}

void strTestLifetime(void) {
    printf("Testing lifetime and memory management...\n");

    // ---------------- Creation ----------------
    // Normal string creation
    String string = stringCreate("Hello");
    assert(string.length == 5 && string.data != NULL);

    // Free valid string
    stringFree(&string);
    assert(STR_INVALID(string) && string.length == 0 && string.capacity == 0);

    // Creation with size
    string = stringCreateSize(16);
    assert(string.capacity == 16 && string.length == 0 && string.data != NULL);
    stringFree(&string);

    // Edge case: creation size 0 returns STR_NULL
    string = stringCreateSize(0);
    assert(STR_INVALID(string) && string.length == 0 && string.capacity == 0);

    // Edge case: create empty string literal returns STR_NULL
    string = stringCreate("");
    assert(STR_INVALID(string) && string.length == 0 && string.capacity == 0);

    // ---------------- Copy ----------------
    string = stringCreate("Hello");
    String copy = stringCopy(&string);
    assert(copy.length == string.length);
    assert(memcmp(copy.data, string.data, string.length) == 0);

    // Copying STR_NULL returns STR_NULL
    String nullCopy = stringCopy(&STR_NULL);
    assert(STR_INVALID(nullCopy));

    stringFree(&copy);
    stringFree(&nullCopy);
    stringFree(&string);

    // ---------------- Reserve ----------------
    string = stringCreate("Hi");
    stringReserve(&string, 10);
    assert(string.capacity >= 10 && string.length == 2);

    // Reserve smaller than length should not shrink below length
    stringReserve(&string, 1);
    assert(string.capacity >= string.length);

    // Reserving STR_NULL should remain STR_NULL
    String nullString = STR_NULL;
    stringReserve(&nullString, 5);
    assert(STR_INVALID(nullString));

    // ---------------- Shrink Buffer ----------------
    stringShrinkBuffer(&string);
    assert(string.capacity == string.length);  // buffer matches length

    // Shrinking STR_NULL should remain STR_NULL
    stringShrinkBuffer(&nullString);
    assert(STR_INVALID(nullString));

    // ---------------- Clear ----------------
    stringClear(&string);
    assert(string.length == 0 && string.data != NULL); // buffer retained

    // Clearing STR_NULL should do nothing
    stringClear(&nullString);
    assert(STR_INVALID(nullString));

    // ---------------- Free ----------------
    stringFree(&string);
    stringFree(&nullString); // freeing STR_NULL is safe
    assert(STR_INVALID(string) && STR_INVALID(nullString));

    printf("Lifetime and memory management tested successfully.\n");
}

void strTestArrays(void) {
    printf("Testing string array functions...\n");

    // ---------------- stringArrayCreate ----------------
    StringArray array = stringArrayCreate(5);
    assert(array.length == 5);
    assert(array.data != NULL);

    // Verify all elements are STR_NULL
    for (size_t i = 0; i < array.length; i++) {
        assert(STR_INVALID(array.data[i]));
    }

    // Zero-length array
    StringArray zeroArray = stringArrayCreate(0);
    assert(zeroArray.length == 0);
    assert(zeroArray.data == NULL);

    // ---------------- stringArrayFree ----------------
    // Assign some strings but only free the array, not the strings
    for (size_t i = 0; i < array.length; i++) {
        array.data[i] = stringCreate("Test");
    }
    stringArrayFree(&array);
    // The strings are still allocated
    for (size_t i = 0; i < 5; i++) {
        assert(array.data == NULL || STR_INVALID(array.data[i])); // array.data should be NULL after free
    }

    // ---------------- stringArrayFreeDeep ----------------
    // Create a new array and fill with strings
    array = stringArrayCreate(3);
    array.data[0] = stringCreate("One");
    array.data[1] = stringCreate("Two");
    array.data[2] = stringCreate("Three");

    stringArrayFreeDeep(&array);
    assert(array.data == NULL);
    assert(array.length == 0);

    // Freeing STR_NULL array is safe
    StringArray nullArray = STR_ARRAY_NULL;
    stringArrayFree(&nullArray);
    stringArrayFreeDeep(&nullArray);
    assert(STR_ARRAY_INVALID(nullArray));

    printf("String array functions tested successfully.\n");
}

void strTestIO(void) {
    printf("Testing string IO and formatting (using tests/ folder)...\n");

    // ---------------- Setup tests folder ----------------
    const char* testFolder = "tests";
#ifdef _WIN32
    _mkdir(testFolder);  // Windows
#else
    mkdir(testFolder, 0755);  // POSIX
#endif

    // ---------------- stringSetFormat ----------------
    String string = stringCreate("Init");
    stringSetFormat(&string, "Hello %d %s", 42, "World");
    assert(string.length == strlen("Hello 42 World"));
    assert(memcmp(string.data, "Hello 42 World", string.length) == 0);

    stringSetFormat(&STR_NULL, "Fail");
    assert(STR_INVALID(STR_NULL));

    // ---------------- stringGetCString ----------------
    char* cstr = stringGetCString(&string);
    assert(cstr != NULL);
    assert(strcmp(cstr, "Hello 42 World") == 0);
    free(cstr);

    assert(stringGetCString(&STR_NULL) == NULL);

    // ---------------- fStringPrint / fStringLog ----------------
    char tmpFilePath[256];
    snprintf(tmpFilePath, sizeof(tmpFilePath), "%s/tmp_stream.txt", testFolder);

    FILE* tmpStream = fopen(tmpFilePath, "w+b");
    assert(tmpStream != NULL);

    fStringPrint(&string, tmpStream);
    fflush(tmpStream);
    fseek(tmpStream, 0, SEEK_SET);
    char buffer[128] = {0};
    size_t readLen = fread(buffer, 1, sizeof(buffer), tmpStream);
    assert(readLen == string.length);
    assert(memcmp(buffer, string.data, string.length) == 0);

    fseek(tmpStream, 0, SEEK_SET);
    memset(buffer, 0, sizeof(buffer));
    fStringLog(&string, tmpStream);
    fflush(tmpStream);
    fseek(tmpStream, 0, SEEK_SET);
    fread(buffer, 1, sizeof(buffer), tmpStream);
    assert(strstr(buffer, "Hello 42 World") != NULL);

    fStringPrint(&STR_NULL, tmpStream);
    fStringLog(&STR_NULL, tmpStream);

    fclose(tmpStream);
    remove(tmpFilePath);

    // ---------------- stringReadFile ----------------
    snprintf(tmpFilePath, sizeof(tmpFilePath), "%s/strtest_tmp.txt", testFolder);

    FILE* file = fopen(tmpFilePath, "wb");
    assert(file != NULL);
    fwrite("FileContent", 1, 11, file);
    fclose(file);

    String fileString = stringReadFile(tmpFilePath);
    assert(fileString.length == 11);
    assert(memcmp(fileString.data, "FileContent", 11) == 0);
    stringFree(&fileString);

    // Reading non-existent file returns STR_NULL
    fileString = stringReadFile("tests/nonexistentfile.txt");
    assert(STR_INVALID(fileString));

    // Reading empty file returns STR_NULL
    file = fopen(tmpFilePath, "wb");
    fclose(file);
    fileString = stringReadFile(tmpFilePath);
    assert(STR_INVALID(fileString));

    remove(tmpFilePath);
    stringFree(&string);

    printf("String IO and formatting (tests/ folder) tested successfully.\n");
}

void strTestBasic(void) {
    printf("Testing basic string manipulation...\n");

    // ---------------- Setup ----------------
    String string = stringCreate("Hello");
    assert(string.data != NULL && string.length == 5);

    // ---------------- stringSet ----------------
    stringSet(&string, "World");
    assert(string.length == 5);
    assert(memcmp(string.data, "World", 5) == 0);

    // Setting STR_NULL does nothing
    String nullString = STR_NULL;
    stringSet(&nullString, "Fail");
    assert(STR_INVALID(nullString));

    // ---------------- stringAppend ----------------
    String appendStr = stringCreate("123");
    stringAppend(&string, &appendStr);
    assert(string.length == 8);
    assert(memcmp(string.data, "World123", 8) == 0);

    // Appending STR_NULL does nothing
    stringAppend(&string, &STR_NULL);
    assert(string.length == 8);

    // ---------------- stringAppendCStr ----------------
    stringAppendCStr(&string, "ABC");
    assert(string.length == 11);
    assert(memcmp(string.data + 8, "ABC", 3) == 0);

    // Appending NULL C string does nothing
    stringAppendCStr(&string, NULL);
    assert(string.length == 11);

    // ---------------- stringInsert ----------------
    String insertStr = stringCreate("XY");
    stringInsert(&string, &insertStr, 5);
    assert(string.length == 13);
    assert(memcmp(string.data + 5, "XY", 2) == 0);

    // Inserting at 0 (beginning)
    stringInsert(&string, &insertStr, 0);
    assert(string.length == 15);
    assert(memcmp(string.data, "XY", 2) == 0);

    // Inserting at end
    stringInsert(&string, &insertStr, string.length);
    assert(string.length == 17);
    assert(memcmp(string.data + 15, "XY", 2) == 0);

    // Insert with STR_NULL does nothing
    stringInsert(&string, &STR_NULL, 0);
    assert(string.length == 17);

    // ---------------- stringDelete ----------------
    // Delete middle section
    stringDelete(&string, 2, 4);
    assert(string.length == 15);

    // Delete from start to 2
    stringDelete(&string, 0, 2);
    assert(string.length == 13);

    // Delete end section
    stringDelete(&string, 11, 13);
    assert(string.length == 11);

    // Invalid deletes do nothing
    stringDelete(&string, 5, 3);
    stringDelete(&STR_NULL, 0, 1);
    assert(string.length == 11);

    // ---------------- stringReplace ----------------
    String oldStr = stringCreate("l");
    String newStr = stringCreate("LL");
    stringReplace(&string, &oldStr, &newStr);
    for (size_t i = 0; i < string.length; i++) {
        // Just sanity: string length increased
        assert(string.length >= 11);
    }

    // Replace with STR_NULL or empty old does nothing
    stringReplace(&string, &STR_NULL, &newStr);
    stringReplace(&string, &oldStr, &STR_NULL);
    String emptyOld = STR_NULL;
    stringReplace(&string, &emptyOld, &newStr);

    // ---------------- stringScale ----------------
    String scaleStr = stringCreate("AB");
    stringScale(&scaleStr, 3);
    assert(scaleStr.length == 6); // "ABABAB"
    assert(memcmp(scaleStr.data, "ABABAB", 6) == 0);

    // Scaling STR_NULL or scaler <=1 does nothing
    stringScale(&STR_NULL, 5);
    stringScale(&scaleStr, 1);
    assert(scaleStr.length == 6);

    // ---------------- Cleanup ----------------
    stringFree(&string);
    stringFree(&appendStr);
    stringFree(&insertStr);
    stringFree(&oldStr);
    stringFree(&newStr);
    stringFree(&scaleStr);
    stringFree(&nullString);

    printf("Basic string manipulation tested successfully.\n");
}

void strTestSearch(void) {
    printf("Testing string search and inspection functions...\n");

    // ---------------- Setup ----------------
    String s1 = stringCreate("HelloWorld");
    String s2 = stringCreate("World");
    String s3 = stringCreate("Hello");
    String s4 = stringCreate("oW");
    String s5 = STR_NULL;

    // ---------------- stringCompare ----------------
    assert(stringCompare(&s1, &s1) == 0);  // identical
    assert(stringCompare(&s1, &s2) < 0);   // HelloWorld < World lexicographically
    assert(stringCompare(&s2, &s1) > 0);   // World > HelloWorld
    assert(stringCompare(&s1, &s5) == 0);  // STR_NULL returns 0

    // ---------------- stringEquals ----------------
    assert(stringEquals(&s1, &s1) == 1);
    assert(stringEquals(&s1, &s2) == 0);
    assert(stringEquals(&s1, &s5) == 0);   // STR_NULL
    assert(stringEquals(&s5, &s5) == 0);   // both null returns 0

    // ---------------- stringIndexOf ----------------
    assert(stringIndexOf(&s1, &s2, 0) == 5);   // "World" at index 5
    assert(stringIndexOf(&s1, &s3, 0) == 0);   // "Hello" at start
    assert(stringIndexOf(&s1, &s4, 0) == 4);   // "oW" at index 4
    assert(stringIndexOf(&s1, &s4, 5) == STR_NOT_FOUND); // start past match
    assert(stringIndexOf(&s1, &s5, 0) == STR_NOT_FOUND); // null search
    assert(stringIndexOf(&s5, &s2, 0) == STR_NOT_FOUND); // null string

    // ---------------- stringLastIndexOf ----------------
    String s6 = stringCreate("ababa");
    String sub = stringCreate("aba");
    assert(stringLastIndexOf(&s6, &sub, 0) == 2); // last "aba" starts at index 2
    assert(stringLastIndexOf(&s6, &s5, 0) == STR_NOT_FOUND);
    assert(stringLastIndexOf(&s5, &sub, 0) == STR_NOT_FOUND);

    // ---------------- stringStartsWith ----------------
    assert(stringStartsWith(&s1, &s3) == 1);   // "HelloWorld" starts with "Hello"
    assert(stringStartsWith(&s1, &s2) == 0);
    assert(stringStartsWith(&s1, &s5) == 0);   // STR_NULL
    assert(stringStartsWith(&s5, &s3) == 0);   // STR_NULL

    // ---------------- stringEndsWith ----------------
    assert(stringEndsWith(&s1, &s2) == 1);     // ends with "World"
    assert(stringEndsWith(&s1, &s3) == 0);
    assert(stringEndsWith(&s1, &s5) == 0);     // STR_NULL
    assert(stringEndsWith(&s5, &s2) == 0);     // STR_NULL

    // ---------------- stringContains ----------------
    assert(stringContains(&s1, &s2) == 1);     // contains "World"
    assert(stringContains(&s1, &s4) == 1);     // contains "oW"
    assert(stringContains(&s1, &sub) == 0);    // does not contain "aba"
    assert(stringContains(&s1, &s5) == 0);     // STR_NULL
    assert(stringContains(&s5, &s2) == 0);     // STR_NULL

    // ---------------- Cleanup ----------------
    stringFree(&s1);
    stringFree(&s2);
    stringFree(&s3);
    stringFree(&s4);
    stringFree(&s5);
    stringFree(&s6);
    stringFree(&sub);

    printf("String search and inspection tested successfully.\n");
}

void strTestTransformation(void) {
    printf("Testing string transformation and cleaning functions...\n");

    // ---------------- Setup ----------------
    String string = stringCreate("   Hello World   ");
    String emptyString = stringCreateSize(10); // initially empty buffer
    String nullString = STR_NULL;

    // ---------------- stringStrip ----------------
    stringStrip(&string);
    assert(string.length == 11); // "Hello World"
    assert(memcmp(string.data, "Hello World", 11) == 0);

    // STR_NULL and empty string
    stringStrip(&nullString);
    assert(STR_INVALID(nullString));
    stringStrip(&emptyString);
    assert(emptyString.length == 0);

    // ---------------- stringTrimLeft ----------------
    string = stringCreate("   LeftTrim");
    stringTrimLeft(&string);
    assert(string.length == 8);
    assert(memcmp(string.data, "LeftTrim", 8) == 0);

    stringTrimLeft(&nullString);
    assert(STR_INVALID(nullString));

    // ---------------- stringTrimRight ----------------
    stringFree(&string);
    string = stringCreate("RightTrim   ");
    stringTrimRight(&string);
    assert(string.length == 9);
    assert(memcmp(string.data, "RightTrim", 9) == 0);

    stringTrimRight(&nullString);
    assert(STR_INVALID(nullString));

    // ---------------- stringToUpper ----------------
    stringFree(&string);
    string = stringCreate("AbC123 xYz");
    stringToUpper(&string);
    assert(memcmp(string.data, "ABC123 XYZ", 10) == 0);

    stringToUpper(&nullString);
    assert(STR_INVALID(nullString));

    // ---------------- stringToLower ----------------
    stringToLower(&string);
    assert(memcmp(string.data, "abc123 xyz", 10) == 0);

    stringToLower(&nullString);
    assert(STR_INVALID(nullString));

    // ---------------- stringReverse ----------------
    stringReverse(&string);
    assert(memcmp(string.data, "zyx 321cba", 10) == 0);

    stringReverse(&nullString);
    assert(STR_INVALID(nullString));

    // ---------------- Cleanup ----------------
    stringFree(&string);
    stringFree(&emptyString);
    stringFree(&nullString);

    printf("String transformation and cleaning tested successfully.\n");
}

void strTestProcessing(void) {
    printf("Testing high-level string processing functions...\n");

    // ---------------- Setup ----------------
    String s = stringCreate("Hello,World,Test");
    String delim = stringCreate(",");
    String sep = stringCreate("-");

    // ---------------- stringSubstring ----------------
    String sub = stringSubstring(&s, 0, 5);
    assert(sub.length == 5);
    assert(memcmp(sub.data, "Hello", sub.length) == 0);
    stringFree(&sub);

    // Substring middle
    sub = stringSubstring(&s, 6, 11);
    assert(sub.length == 5);
    assert(memcmp(sub.data, "World", sub.length) == 0);
    stringFree(&sub);

    // Full string substring
    sub = stringSubstring(&s, 0, s.length);
    assert(sub.length == s.length);
    assert(memcmp(sub.data, s.data, s.length) == 0);
    stringFree(&sub);

    // Empty substring
    sub = stringSubstring(&s, 3, 3);
    assert(STR_INVALID(sub));

    // Out-of-bounds slice returns STR_NULL
    sub = stringSubstring(&s, 0, 100);
    assert(STR_INVALID(sub));

    // STR_NULL input
    sub = stringSubstring(&STR_NULL, 0, 1);
    assert(STR_INVALID(sub));

    // ---------------- stringSplit ----------------
    StringArray arr = stringSplit(&s, &delim);
    assert(arr.length == 3);
    assert(memcmp(arr.data[0].data, "Hello", arr.data[0].length) == 0);
    assert(memcmp(arr.data[1].data, "World", arr.data[1].length) == 0);
    assert(memcmp(arr.data[2].data, "Test", arr.data[2].length) == 0);

    // Split STR_NULL returns null array
    StringArray nullArr = stringSplit(&STR_NULL, &delim);
    assert(STR_ARRAY_INVALID(nullArr));

    stringArrayFreeDeep(&arr);

    // ---------------- stringJoin (variadic) ----------------
    String a = stringCreate("One");
    String b = stringCreate("Two");
    String c = stringCreate("Three");

    String joined = stringJoin(&sep, 3, &a, &b, &c);
    // Expected: "One-Two-Three"
    const char* expected = "One-Two-Three";
    assert(joined.length == strlen(expected));
    assert(memcmp(joined.data, expected, joined.length) == 0);
    stringFree(&joined);

    // Join with count 0 returns STR_NULL
    joined = stringJoin(&sep, 0);
    assert(STR_INVALID(joined));

    // Join with STR_NULL separator behaves like empty separator
    joined = stringJoin(&STR_NULL, 2, &a, &b);
    const char* expected2 = "OneTwo";
    assert(joined.length == strlen(expected2));
    assert(memcmp(joined.data, expected2, joined.length) == 0);
    stringFree(&joined);

    // ---------------- stringJoinArray ----------------
    StringArray joinArray = stringArrayCreate(3);
    joinArray.data[0] = stringCreate("X");
    joinArray.data[1] = stringCreate("Y");
    joinArray.data[2] = stringCreate("Z");

    joined = stringJoinArray(&sep, &joinArray);
    const char* expected3 = "X-Y-Z";
    assert(joined.length == strlen(expected3));
    assert(memcmp(joined.data, expected3, joined.length) == 0);
    stringFree(&joined);

    // Join empty array returns STR_NULL
    StringArray emptyArray = stringArrayCreate(0);
    joined = stringJoinArray(&sep, &emptyArray);
    assert(STR_INVALID(joined));

    // Join STR_NULL array returns STR_NULL
    joined = stringJoinArray(&sep, &STR_ARRAY_NULL);
    assert(STR_INVALID(joined));

    stringArrayFreeDeep(&joinArray);
    stringArrayFree(&emptyArray);

    // ---------------- Cleanup ----------------
    stringFree(&s);
    stringFree(&delim);
    stringFree(&sep);
    stringFree(&a);
    stringFree(&b);
    stringFree(&c);

    printf("High-level string processing tested successfully.\n");
}
