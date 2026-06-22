#include <af_string.h>

void stringExample(void) {
    printf("Testing string library:\n");

    String string = stringCreate("Hello World\n");
    stringToUpper(&string);
    stringPrint(&string);

    String delimiter = stringCreate(" ");
    StringArray array = stringSplit(&string, &delimiter);
    String newline = stringCreate("\n");
    String joined = stringJoinArray(&newline, &array);
    stringPrint(&joined);

    stringFree(&string);
    stringFree(&delimiter);
    stringFree(&newline);
    stringFree(&joined);
    stringArrayFreeDeep(&array);

    printf("\n");
}