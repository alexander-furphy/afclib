#include <stdio.h>
#include <string.h>
#include "str_lib.h"
#include <stdarg.h>

void lifetimeTest(void) {
    String string = stringCreate("Alexander Furphy.");
    stringLog(&string);
    stringPurge(&string);
    stringLog(&string);
    String copy = stringCopy(&string);
    stringLog(&copy);
    stringFree(&string);
    stringLog(&string);
    stringFree(&copy);
    stringLog(&copy);

    String null = stringCreate("");
    stringLog(&null);
}

void ioTest(void) {
    String test = stringCreate("Test String!");
    stringPrint(&test);
    stringLog(&test);
    fStringPrint(&test, stderr);

    stringFree(&test);
    stringLog(&test);

    test = stringCreate("Hello!\n");
    FILE* file = fopen("iotest.txt", "w");
    fStringPrint(&test, file);
    fclose(file);
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
}

void searchingTest(void) {
    String a = stringCreate("Hello, World!");
    String b = stringCreate("World!");
    printf("'World!' first occurance: index %zu.\n", stringIndexOf(&a, &b));
    printf("Comparision of strings: %d.\n", stringCompare(&a, &b));
    printf("String ends with 'World'? %d.\n", stringEndsWith(&a, &b));
}

void stringBufferTest(void) {
    String string = stringCreateEmpty();
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
}

int main(void) {
    String string = stringCreateEmpty();
    stringSetFormat(&string, "Number: %d, Decimal: %f, String: %s\n", 5, 2.324, "Hello");
    stringPrint(&string);

    return 0;
}