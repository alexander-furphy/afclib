afclib
------

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

afclib is a C library that contains modules for **string handling**,
**generic data structures**, and **design pattern frameworks**. Ideal
as a standard library for **general purpose C programming**, making
common tasks easier and more reliable.

## Installation

## Features
- **String Handling:** Buffered, heap allocated strings with all common operations included.
- **Lightweight:** No external dependencies, and small build size.

## Examples
```
#include "af_string.h"

int main(void) {
    String string = stringCreate("Hello World\n");
    stringToUpper(&string);
    stringPrint(&string);

    String delimiter = stringCreate(" ");
    StringArray array = stringSplit(&string, &delimiter);
    for(size_t i = 0; i < array.length; i++) {
        stringPrint(&array.data[i]);
        printf("\n");
    }
}
```

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
