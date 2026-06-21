# afclib

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
![CI](https://github.com/alexander-furphy/afclib/actions/workflows/ci.yml/badge.svg)

afclib is a C library that contains modules for **string handling** and
**generic data structures**. It serves as a lightweight utility library for 
**general-purpose C programming**, making common tasks easier and less prone to 
memory bugs.

## Installation

**macOS/Linux**
Type the following with the CWD set to the root dir.
```
make install
```
All files are created in /usr/local/include/.
Headers should be included with **angle brackets <>.**

Link the library to a program once installed with:
gcc main.c -lafclib -o program

**Windows**
Build using CMake to generate a Visual Studio solution:
cmake -B build

## Features
- **String Handling:** Buffered, heap allocated strings with all main operations included.
- **Generic Collections** List, stack, and queue, implemented with void pointers.
- **Lightweight:** No external dependencies, and small build size.

## Examples
```c
#include <af_string.h>

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
