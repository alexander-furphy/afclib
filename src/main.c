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

    stringMultiply(&a, 3);
    stringLog(&a);

    String sub = stringSubstring(&a, 10, 14);
    stringLog(&sub);
}

int main(void) {
    manipTest();
    return 0;
}