# afclib

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
![CI](https://github.com/FourthRealm/afclib/actions/workflows/ci.yml/badge.svg)

afclib is a C library that contains modules for **string handling**,
**generic data structures**, and **design pattern frameworks**. It
serves as a lightweight, standard library for **general-purpose C 
programming**, making common tasks easier and more reliable.

## Installation

## Features
- **String Handling:** Buffered, heap allocated strings with all common operations included.
- **Lightweight:** No external dependencies, and small build size.

## Examples
```c
#include "af_string.h"

int main(void) {
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

    return 0;
}
```
```
HELLO WORLD
HELLO
WORLD
```

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
