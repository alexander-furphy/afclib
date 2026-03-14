#pragma region // Old Tests

#include <stdio.h>
#include <stdlib.h>
#include "str_lib.h"

void lifetimeTest(void) {
    String string = stringCreate("Alexander Furphy.");
    stringLog(&string);
    stringShrinkBuffer(&string);
    stringLog(&string);
    String copy = stringCopy(&string);
    stringLog(&copy);
    stringFree(&string);
    stringLog(&string);
    stringFree(&copy);
    stringLog(&copy);

    String null = stringCreate("");
    stringLog(&null);
    stringFree(&null);
}

void ioTest(void) {
    printf("\n\n");

    String test = stringCreate("Test String!");
    stringPrint(&test);
    stringLog(&test);
    fStringPrint(&test, stderr);

    stringFree(&test);
    stringLog(&test);

    stringFree(&test);

    test = stringCreate("Hello!\n");
    FILE* file = fopen("iotest.txt", "a");
    fStringPrint(&test, file);
    fclose(file);
    stringFree(&test);

    String fileData = stringReadFile("iotest.txt");
    stringPrint(&fileData);
    stringFree(&fileData);

    printf("\n\n");
}

void manipTest(void) {
    String a = stringCreate(" \t\nHello  ");
    String b = stringCreate(", World! ");
    
    stringLog(&a);
    stringLog(&b);

    stringStrip(&a);
    stringLog(&a);

    stringAppend(&a, &b);
    stringFree(&b);
    stringLog(&b);
    stringLog(&a);

    stringScale(&a, 3);
    stringLog(&a);

    String sub = stringSubstring(&a, 10, 14);
    stringLog(&sub);

    stringFree(&a);
    stringFree(&sub);
}

void searchingTest(void) {
    String a = stringCreate("Hello, World!");
    String b = stringCreate("World!");
    printf("'World!' first occurance: index %zu.\n", stringIndexOf(&a, &b, 5));
    printf("Comparision of strings: %d.\n", stringCompare(&a, &b));
    printf("String ends with 'World'? %d.\n", stringEndsWith(&a, &b));

    stringFree(&a);
    stringFree(&b);
}

void stringBufferTest(void) {
    String string = STR_NULL;
    stringLog(&string);
    stringSet(&string, "Hello, World!");
    stringLog(&string);
    stringSet(&string, "This is a very long string that will require resizing.");
    stringLog(&string);
    stringReserve(&string, 100);
    stringClear(&string);
    stringLog(&string);
    stringSet(&string, "Small string ");
    stringLog(&string);
    stringScale(&string, 7);
    stringLog(&string);
    char* data = stringGetCString(&string);
    printf("%s", data);
    free(data);
    stringFree(&string);
}

void extraProcessingTest(void) {
    String string = stringCreate("Hello\n");
    stringLog(&string);
    String toInsert = stringCreate("arp");
    stringLog(&toInsert);
    stringInsert(&string, &toInsert, 1);
    stringInsert(&string, &toInsert, 7);
    stringLog(&string);
    stringPrint(&string);
    stringDelete(&string, 1, 4);
    stringLog(&string);
    stringFree(&string);
    stringFree(&toInsert);

    String sentence = stringCreate("This is a simple sentence.");
    String space = stringCreate(" ");
    String empty = stringCreate("");
    stringReplace(&sentence, &space, &empty);
    stringLog(&sentence);

    stringToUpper(&sentence);
    stringLog(&sentence);
    stringToLower(&sentence);
    stringLog(&sentence);

    stringReverse(&sentence);
    stringLog(&sentence);

    stringFree(&sentence);
    stringFree(&space);
    stringFree(&empty);
}

void comparisionTest(void) {
    String string = stringCreate("Hello");
    String copy = stringCopy(&string);
    printf("Equal: %d\n", stringEquals(&string, &copy));
    stringFree(&copy);
    copy = stringCreate("Not equal");
    printf("Equal: %d\n", stringEquals(&string, &copy));
    stringFree(&string);
    stringFree(&copy);

    String a = stringCreate("Hello, World");
    String b = stringCreate("Hello");
    printf("Starts with: %d\n", stringStartsWith(&a, &b));
    stringFree(&b);
    b = STR_NULL;
    printf("Starts with: %d\n", stringStartsWith(&a, &b));
    stringFree(&b);
    b = stringCreate("ello");
    printf("Starts with: %d\n", stringStartsWith(&a, &b));

    printf("Contains: %d\n", stringContains(&a, &b));

    stringFree(&b);
    b = stringCreate("o");
    printf("Last 'o': %zu\n", stringLastIndexOf(&a, &b, 0));

    stringFree(&a);
    stringFree(&b);
}

void stringArrayTest(void) {
    StringArray array = stringArrayCreate(3);
    array.data[0] = stringCreate("One");
    array.data[1] = stringCreate("two");
    array.data[2] = stringCreate("three.\n");

    for(size_t i = 0; i < array.length; i++) {
        stringPrint(&array.data[i]);
    }

    String seperator = stringCreate(", ");
    String joined = stringJoinArray(&seperator, &array);
    stringPrint(&joined);

    stringSet(&seperator, "\n");
    String otherJoined = stringJoin(
        &seperator, 3, 
        &array.data[0], 
        &array.data[1], 
        &array.data[2]
    );

    stringPrint(&otherJoined);

    stringFree(&seperator);
    stringFree(&joined);
    stringFree(&otherJoined);

    stringArrayFreeDeep(&array);

    String sentence = stringCreate("This is a simple sentence");
    String delimiter = stringCreate(" ");
    StringArray splitted = stringSplit(&sentence, &delimiter);

    for(size_t i = 0; i < splitted.length; i++) {
        stringPrint(&splitted.data[i]);
        printf("\n");
    }

    stringFree(&sentence);
    stringFree(&delimiter);
    stringArrayFreeDeep(&splitted);
}

#pragma endregion

#include "str_tests.h"

int main(void) {
    strTestAll();
    return 0;
}