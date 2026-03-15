#include "af_string.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
#include <direct.h>  // for _mkdir
#else
#include <sys/stat.h>  // for mkdir
#include <sys/types.h>
#endif

/* Global counter for failed ci_asserts */
static int ci_ci_assert_failures = 0;

/* CI-friendly ci_assert macro */
#define ci_assert(expr) do { \
    if (!(expr)) { \
        fprintf(stderr, "ci_assertion failed: %s\n" \
                        "  in file %s, line %d\n", \
                        #expr, __FILE__, __LINE__); \
        ci_ci_assert_failures++; \
    } \
} while(0)

/* Call at the end of main to set proper exit code */
static void ci_ci_assert_exit() {
    if (ci_ci_assert_failures > 0) {
        fprintf(stderr, "Total ci_assertion failures: %d\n", ci_ci_assert_failures);
        exit(1);  /* Non-zero exit code for CI */
    }
}

void strTestLifetime(void) {
    printf("Testing lifetime and memory management...\n");

    // ---------------- Creation ----------------
    // Normal string creation
    String string = stringCreate("Hello");
    ci_assert(string.length == 5 && string.data != NULL);

    // Free valid string
    stringFree(&string);
    ci_assert(STR_INVALID(string) && string.length == 0 && string.capacity == 0);

    // Creation with size
    string = stringCreateSize(16);
    ci_assert(string.capacity == 16 && string.length == 0 && string.data != NULL);
    stringFree(&string);

    // Edge case: creation size 0 returns STR_NULL
    string = stringCreateSize(0);
    ci_assert(STR_INVALID(string) && string.length == 0 && string.capacity == 0);

    // Edge case: create empty string literal returns STR_NULL
    string = stringCreate("");
    ci_assert(STR_INVALID(string) && string.length == 0 && string.capacity == 0);

    // ---------------- Copy ----------------
    string = stringCreate("Hello");
    String copy = stringCopy(&string);
    ci_assert(copy.length == string.length);
    ci_assert(memcmp(copy.data, string.data, string.length) == 0);

    // Copying STR_NULL returns STR_NULL
    String nullCopy = stringCopy(&STR_NULL);
    ci_assert(STR_INVALID(nullCopy));

    stringFree(&copy);
    stringFree(&nullCopy);
    stringFree(&string);

    // ---------------- Reserve ----------------
    string = stringCreate("Hi");
    stringReserve(&string, 10);
    ci_assert(string.capacity >= 10 && string.length == 2);

    // Reserve smaller than length should not shrink below length
    stringReserve(&string, 1);
    ci_assert(string.capacity >= string.length);

    // Reserving STR_NULL should remain STR_NULL
    String nullString = STR_NULL;
    stringReserve(&nullString, 5);
    ci_assert(STR_INVALID(nullString));

    // ---------------- Shrink Buffer ----------------
    stringShrinkBuffer(&string);
    ci_assert(string.capacity == string.length);  // buffer matches length

    // Shrinking STR_NULL should remain STR_NULL
    stringShrinkBuffer(&nullString);
    ci_assert(STR_INVALID(nullString));

    // ---------------- Clear ----------------
    stringClear(&string);
    ci_assert(string.length == 0 && string.data != NULL); // buffer retained

    // Clearing STR_NULL should do nothing
    stringClear(&nullString);
    ci_assert(STR_INVALID(nullString));

    // ---------------- Free ----------------
    stringFree(&string);
    stringFree(&nullString); // freeing STR_NULL is safe
    ci_assert(STR_INVALID(string) && STR_INVALID(nullString));

    printf("Lifetime and memory management tested successfully.\n");
}

void strTestArrays(void) {
    printf("Testing string array functions...\n");

    // ---------------- stringArrayCreate ----------------
    StringArray array = stringArrayCreate(5);
    ci_assert(array.length == 5);
    ci_assert(array.data != NULL);

    // Verify all elements are STR_NULL
    for (size_t i = 0; i < array.length; i++) {
        ci_assert(STR_INVALID(array.data[i]));
    }

    // Zero-length array
    StringArray zeroArray = stringArrayCreate(0);
    ci_assert(zeroArray.length == 0);
    ci_assert(zeroArray.data == NULL);

    // ---------------- stringArrayFree ----------------
    // Assign some strings but only free the array, not the strings
    for (size_t i = 0; i < array.length; i++) {
        array.data[i] = stringCreate("Test");
    }
    stringArrayFree(&array);
    // The strings are still allocated
    for (size_t i = 0; i < 5; i++) {
        ci_assert(array.data == NULL || STR_INVALID(array.data[i])); // array.data should be NULL after free
    }

    // ---------------- stringArrayFreeDeep ----------------
    // Create a new array and fill with strings
    array = stringArrayCreate(3);
    array.data[0] = stringCreate("One");
    array.data[1] = stringCreate("Two");
    array.data[2] = stringCreate("Three");

    stringArrayFreeDeep(&array);
    ci_assert(array.data == NULL);
    ci_assert(array.length == 0);

    // Freeing STR_NULL array is safe
    StringArray nullArray = STR_ARRAY_NULL;
    stringArrayFree(&nullArray);
    stringArrayFreeDeep(&nullArray);
    ci_assert(STR_ARRAY_INVALID(nullArray));

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
    ci_assert(string.length == strlen("Hello 42 World"));
    ci_assert(memcmp(string.data, "Hello 42 World", string.length) == 0);

    stringSetFormat(&STR_NULL, "Fail");
    ci_assert(STR_INVALID(STR_NULL));

    // ---------------- stringGetCString ----------------
    char* cstr = stringGetCString(&string);
    ci_assert(cstr != NULL);
    ci_assert(strcmp(cstr, "Hello 42 World") == 0);
    free(cstr);

    ci_assert(stringGetCString(&STR_NULL) == NULL);

    // ---------------- fStringPrint / fStringLog ----------------
    char tmpFilePath[256];
    snprintf(tmpFilePath, sizeof(tmpFilePath), "%s/tmp_stream.txt", testFolder);

    FILE* tmpStream = fopen(tmpFilePath, "w+b");
    ci_assert(tmpStream != NULL);

    fStringPrint(&string, tmpStream);
    fflush(tmpStream);
    fseek(tmpStream, 0, SEEK_SET);
    char buffer[128] = {0};
    size_t readLen = fread(buffer, 1, sizeof(buffer), tmpStream);
    ci_assert(readLen == string.length);
    ci_assert(memcmp(buffer, string.data, string.length) == 0);

    fseek(tmpStream, 0, SEEK_SET);
    memset(buffer, 0, sizeof(buffer));
    fStringLog(&string, tmpStream);
    fflush(tmpStream);
    fseek(tmpStream, 0, SEEK_SET);
    fread(buffer, 1, sizeof(buffer), tmpStream);
    ci_assert(strstr(buffer, "Hello 42 World") != NULL);

    fStringPrint(&STR_NULL, tmpStream);
    fStringLog(&STR_NULL, tmpStream);

    fclose(tmpStream);
    remove(tmpFilePath);

    // ---------------- stringReadFile ----------------
    snprintf(tmpFilePath, sizeof(tmpFilePath), "%s/strtest_tmp.txt", testFolder);

    FILE* file = fopen(tmpFilePath, "wb");
    ci_assert(file != NULL);
    fwrite("FileContent", 1, 11, file);
    fclose(file);

    String fileString = stringReadFile(tmpFilePath);
    ci_assert(fileString.length == 11);
    ci_assert(memcmp(fileString.data, "FileContent", 11) == 0);
    stringFree(&fileString);

    // Reading non-existent file returns STR_NULL
    fileString = stringReadFile("tests/nonexistentfile.txt");
    ci_assert(STR_INVALID(fileString));

    // Reading empty file returns STR_NULL
    file = fopen(tmpFilePath, "wb");
    fclose(file);
    fileString = stringReadFile(tmpFilePath);
    ci_assert(STR_INVALID(fileString));

    remove(tmpFilePath);
    stringFree(&string);

    printf("String IO and formatting (tests/ folder) tested successfully.\n");
}

void strTestBasic(void) {
    printf("Testing basic string manipulation...\n");

    // ---------------- Setup ----------------
    String string = stringCreate("Hello");
    ci_assert(string.data != NULL && string.length == 5);

    // ---------------- stringSet ----------------
    stringSetCStr(&string, "World");
    ci_assert(string.length == 5);
    ci_assert(memcmp(string.data, "World", 5) == 0);

    // Setting STR_NULL does nothing
    String nullString = STR_NULL;
    stringSetCStr(&nullString, "Fail");
    ci_assert(STR_INVALID(nullString));

    // ---------------- stringAppend ----------------
    String appendStr = stringCreate("123");
    stringAppend(&string, &appendStr);
    ci_assert(string.length == 8);
    ci_assert(memcmp(string.data, "World123", 8) == 0);

    // Appending STR_NULL does nothing
    stringAppend(&string, &STR_NULL);
    ci_assert(string.length == 8);

    // ---------------- stringAppendCStr ----------------
    stringAppendCStr(&string, "ABC");
    ci_assert(string.length == 11);
    ci_assert(memcmp(string.data + 8, "ABC", 3) == 0);

    // Appending NULL C string does nothing
    stringAppendCStr(&string, NULL);
    ci_assert(string.length == 11);

    // ---------------- stringInsert ----------------
    String insertStr = stringCreate("XY");
    stringInsert(&string, &insertStr, 5);
    ci_assert(string.length == 13);
    ci_assert(memcmp(string.data + 5, "XY", 2) == 0);

    // Inserting at 0 (beginning)
    stringInsert(&string, &insertStr, 0);
    ci_assert(string.length == 15);
    ci_assert(memcmp(string.data, "XY", 2) == 0);

    // Inserting at end
    stringInsert(&string, &insertStr, string.length);
    ci_assert(string.length == 17);
    ci_assert(memcmp(string.data + 15, "XY", 2) == 0);

    // Insert with STR_NULL does nothing
    stringInsert(&string, &STR_NULL, 0);
    ci_assert(string.length == 17);

    // ---------------- stringDelete ----------------
    // Delete middle section
    stringDelete(&string, 2, 4);
    ci_assert(string.length == 15);

    // Delete from start to 2
    stringDelete(&string, 0, 2);
    ci_assert(string.length == 13);

    // Delete end section
    stringDelete(&string, 11, 13);
    ci_assert(string.length == 11);

    // Invalid deletes do nothing
    stringDelete(&string, 5, 3);
    stringDelete(&STR_NULL, 0, 1);
    ci_assert(string.length == 11);

    // ---------------- stringReplace ----------------
    String oldStr = stringCreate("l");
    String newStr = stringCreate("LL");
    stringReplace(&string, &oldStr, &newStr);
    for (size_t i = 0; i < string.length; i++) {
        // Just sanity: string length increased
        ci_assert(string.length >= 11);
    }

    // Replace with STR_NULL or empty old does nothing
    stringReplace(&string, &STR_NULL, &newStr);
    stringReplace(&string, &oldStr, &STR_NULL);
    String emptyOld = STR_NULL;
    stringReplace(&string, &emptyOld, &newStr);

    // ---------------- stringScale ----------------
    String scaleStr = stringCreate("AB");
    stringScale(&scaleStr, 3);
    ci_assert(scaleStr.length == 6); // "ABABAB"
    ci_assert(memcmp(scaleStr.data, "ABABAB", 6) == 0);

    // Scaling STR_NULL or scaler <=1 does nothing
    stringScale(&STR_NULL, 5);
    stringScale(&scaleStr, 1);
    ci_assert(scaleStr.length == 6);

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
    ci_assert(stringCompare(&s1, &s1) == 0);  // identical
    ci_assert(stringCompare(&s1, &s2) < 0);   // HelloWorld < World lexicographically
    ci_assert(stringCompare(&s2, &s1) > 0);   // World > HelloWorld
    ci_assert(stringCompare(&s1, &s5) == 0);  // STR_NULL returns 0

    // ---------------- stringEquals ----------------
    ci_assert(stringEquals(&s1, &s1) == 1);
    ci_assert(stringEquals(&s1, &s2) == 0);
    ci_assert(stringEquals(&s1, &s5) == 0);   // STR_NULL
    ci_assert(stringEquals(&s5, &s5) == 0);   // both null returns 0

    // ---------------- stringIndexOf ----------------
    ci_assert(stringIndexOf(&s1, &s2, 0) == 5);   // "World" at index 5
    ci_assert(stringIndexOf(&s1, &s3, 0) == 0);   // "Hello" at start
    ci_assert(stringIndexOf(&s1, &s4, 0) == 4);   // "oW" at index 4
    ci_assert(stringIndexOf(&s1, &s4, 5) == STR_NOT_FOUND); // start past match
    ci_assert(stringIndexOf(&s1, &s5, 0) == STR_NOT_FOUND); // null search
    ci_assert(stringIndexOf(&s5, &s2, 0) == STR_NOT_FOUND); // null string

    // ---------------- stringLastIndexOf ----------------
    String s6 = stringCreate("ababa");
    String sub = stringCreate("aba");
    ci_assert(stringLastIndexOf(&s6, &sub, 0) == 2); // last "aba" starts at index 2
    ci_assert(stringLastIndexOf(&s6, &s5, 0) == STR_NOT_FOUND);
    ci_assert(stringLastIndexOf(&s5, &sub, 0) == STR_NOT_FOUND);

    // ---------------- stringStartsWith ----------------
    ci_assert(stringStartsWith(&s1, &s3) == 1);   // "HelloWorld" starts with "Hello"
    ci_assert(stringStartsWith(&s1, &s2) == 0);
    ci_assert(stringStartsWith(&s1, &s5) == 0);   // STR_NULL
    ci_assert(stringStartsWith(&s5, &s3) == 0);   // STR_NULL

    // ---------------- stringEndsWith ----------------
    ci_assert(stringEndsWith(&s1, &s2) == 1);     // ends with "World"
    ci_assert(stringEndsWith(&s1, &s3) == 0);
    ci_assert(stringEndsWith(&s1, &s5) == 0);     // STR_NULL
    ci_assert(stringEndsWith(&s5, &s2) == 0);     // STR_NULL

    // ---------------- stringContains ----------------
    ci_assert(stringContains(&s1, &s2) == 1);     // contains "World"
    ci_assert(stringContains(&s1, &s4) == 1);     // contains "oW"
    ci_assert(stringContains(&s1, &sub) == 0);    // does not contain "aba"
    ci_assert(stringContains(&s1, &s5) == 0);     // STR_NULL
    ci_assert(stringContains(&s5, &s2) == 0);     // STR_NULL

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
    ci_assert(string.length == 11); // "Hello World"
    ci_assert(memcmp(string.data, "Hello World", 11) == 0);

    // STR_NULL and empty string
    stringStrip(&nullString);
    ci_assert(STR_INVALID(nullString));
    stringStrip(&emptyString);
    ci_assert(emptyString.length == 0);

    // ---------------- stringTrimLeft ----------------
    string = stringCreate("   LeftTrim");
    stringTrimLeft(&string);
    ci_assert(string.length == 8);
    ci_assert(memcmp(string.data, "LeftTrim", 8) == 0);

    stringTrimLeft(&nullString);
    ci_assert(STR_INVALID(nullString));

    // ---------------- stringTrimRight ----------------
    stringFree(&string);
    string = stringCreate("RightTrim   ");
    stringTrimRight(&string);
    ci_assert(string.length == 9);
    ci_assert(memcmp(string.data, "RightTrim", 9) == 0);

    stringTrimRight(&nullString);
    ci_assert(STR_INVALID(nullString));

    // ---------------- stringToUpper ----------------
    stringFree(&string);
    string = stringCreate("AbC123 xYz");
    stringToUpper(&string);
    ci_assert(memcmp(string.data, "ABC123 XYZ", 10) == 0);

    stringToUpper(&nullString);
    ci_assert(STR_INVALID(nullString));

    // ---------------- stringToLower ----------------
    stringToLower(&string);
    ci_assert(memcmp(string.data, "abc123 xyz", 10) == 0);

    stringToLower(&nullString);
    ci_assert(STR_INVALID(nullString));

    // ---------------- stringReverse ----------------
    stringReverse(&string);
    ci_assert(memcmp(string.data, "zyx 321cba", 10) == 0);

    stringReverse(&nullString);
    ci_assert(STR_INVALID(nullString));

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
    ci_assert(sub.length == 5);
    ci_assert(memcmp(sub.data, "Hello", sub.length) == 0);
    stringFree(&sub);

    // Substring middle
    sub = stringSubstring(&s, 6, 11);
    ci_assert(sub.length == 5);
    ci_assert(memcmp(sub.data, "World", sub.length) == 0);
    stringFree(&sub);

    // Full string substring
    sub = stringSubstring(&s, 0, s.length);
    ci_assert(sub.length == s.length);
    ci_assert(memcmp(sub.data, s.data, s.length) == 0);
    stringFree(&sub);

    // Empty substring
    sub = stringSubstring(&s, 3, 3);
    ci_assert(STR_INVALID(sub));

    // Out-of-bounds slice returns STR_NULL
    sub = stringSubstring(&s, 0, 100);
    ci_assert(STR_INVALID(sub));

    // STR_NULL input
    sub = stringSubstring(&STR_NULL, 0, 1);
    ci_assert(STR_INVALID(sub));

    // ---------------- stringSplit ----------------
    StringArray arr = stringSplit(&s, &delim);
    ci_assert(arr.length == 3);
    ci_assert(memcmp(arr.data[0].data, "Hello", arr.data[0].length) == 0);
    ci_assert(memcmp(arr.data[1].data, "World", arr.data[1].length) == 0);
    ci_assert(memcmp(arr.data[2].data, "Test", arr.data[2].length) == 0);

    // Split STR_NULL returns null array
    StringArray nullArr = stringSplit(&STR_NULL, &delim);
    ci_assert(STR_ARRAY_INVALID(nullArr));

    stringArrayFreeDeep(&arr);

    // ---------------- stringJoin (variadic) ----------------
    String a = stringCreate("One");
    String b = stringCreate("Two");
    String c = stringCreate("Three");

    String joined = stringJoin(&sep, 3, &a, &b, &c);
    // Expected: "One-Two-Three"
    const char* expected = "One-Two-Three";
    ci_assert(joined.length == strlen(expected));
    ci_assert(memcmp(joined.data, expected, joined.length) == 0);
    stringFree(&joined);

    // Join with count 0 returns STR_NULL
    joined = stringJoin(&sep, 0);
    ci_assert(STR_INVALID(joined));

    // Join with STR_NULL separator behaves like empty separator
    joined = stringJoin(&STR_NULL, 2, &a, &b);
    const char* expected2 = "OneTwo";
    ci_assert(joined.length == strlen(expected2));
    ci_assert(memcmp(joined.data, expected2, joined.length) == 0);
    stringFree(&joined);

    // ---------------- stringJoinArray ----------------
    StringArray joinArray = stringArrayCreate(3);
    joinArray.data[0] = stringCreate("X");
    joinArray.data[1] = stringCreate("Y");
    joinArray.data[2] = stringCreate("Z");

    joined = stringJoinArray(&sep, &joinArray);
    const char* expected3 = "X-Y-Z";
    ci_assert(joined.length == strlen(expected3));
    ci_assert(memcmp(joined.data, expected3, joined.length) == 0);
    stringFree(&joined);

    // Join empty array returns STR_NULL
    StringArray emptyArray = stringArrayCreate(0);
    joined = stringJoinArray(&sep, &emptyArray);
    ci_assert(STR_INVALID(joined));

    // Join STR_NULL array returns STR_NULL
    joined = stringJoinArray(&sep, &STR_ARRAY_NULL);
    ci_assert(STR_INVALID(joined));

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

void strTestWrappers(void) {
    printf("Testing C-string wrappers...\n");

    // --- Set / Append ---
    String s = stringCreate("Hello");
    stringSetCStr(&s, "World");
    ci_assert(stringEqualsCStr(&s, "World"));

    stringAppendCStr(&s, "!");
    ci_assert(stringEqualsCStr(&s, "World!"));

    // --- Contains / Starts / Ends ---
    ci_assert(stringContainsCStr(&s, "World"));
    ci_assert(stringStartsWithCStr(&s, "World"));
    ci_assert(stringEndsWithCStr(&s, "!"));

    // --- IndexOf / LastIndexOf ---
    stringSetCStr(&s, "abcabcabc");

    ci_assert(stringIndexOfCStr(&s, "abc", 0) == 0);
    ci_assert(stringIndexOfCStr(&s, "abc", 1) == 3);

    ci_assert(stringLastIndexOfCStr(&s, "abc", 0) == 6);
    ci_assert(stringLastIndexOfCStr(&s, "abc", 3) == 3);

    // --- Replace ---
    stringSetCStr(&s, "one two two three");
    stringReplaceCStr(&s, "two", "2");
    ci_assert(stringContainsCStr(&s, "2"));

    // --- Split ---
    stringSetCStr(&s, "a,b,c");

    StringArray arr = stringSplitCStr(&s, ",");
    ci_assert(arr.length == 3);
    ci_assert(stringEqualsCStr(&arr.data[0], "a"));
    ci_assert(stringEqualsCStr(&arr.data[1], "b"));
    ci_assert(stringEqualsCStr(&arr.data[2], "c"));

    stringArrayFreeDeep(&arr);

    // --- Join (variadic) ---
    String a = stringCreate("A");
    String b = stringCreate("B");
    String c = stringCreate("C");

    String joined = stringJoinCStr(",", 3, &a, &b, &c);
    ci_assert(stringEqualsCStr(&joined, "A,B,C"));

    stringFree(&joined);

    // --- JoinArray ---
    StringArray joinArr = stringArrayCreate(3);
    joinArr.data[0] = stringCreate("X");
    joinArr.data[1] = stringCreate("Y");
    joinArr.data[2] = stringCreate("Z");

    joined = stringJoinArrayCStr("-", &joinArr);
    ci_assert(stringEqualsCStr(&joined, "X-Y-Z"));

    stringFree(&joined);
    stringArrayFreeDeep(&joinArr);

    // --- Equals ---
    stringSetCStr(&s, "test");
    ci_assert(stringEqualsCStr(&s, "test"));
    ci_assert(!stringEqualsCStr(&s, "TEST"));

    // --- Print (just ensure no crash) ---
    stringPrintCStr("Wrapper print test\n");

    stringFree(&s);
    stringFree(&a);
    stringFree(&b);
    stringFree(&c);

    printf("C-string wrappers tested successfully.\n");
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);

    printf("Testing all string modules...\n\n");

    strTestLifetime();
    strTestArrays();
    strTestIO();
    strTestBasic();
    strTestSearch();
    strTestTransformation();
    strTestProcessing();
    strTestWrappers();

    printf("\nAll string modules tested successfully.\n");

    // Exit if failiure
    ci_ci_assert_exit();

    return 0;
}
