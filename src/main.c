#include <stdio.h>
#include <string.h>
#include "str_lib.h"

int main(void) {
    String a = stringCreate("Hello, World!");
    String b = stringCreate("!");
    printf("Result: %d\n", stringEndsWith(a, b));

    return 0;
}