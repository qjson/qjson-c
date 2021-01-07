# qjson to json converter

[json](https://www.json.org) is a very popular data encoding with a good support in many 
programming languages. It may thus seam a good idea to use it for manually managed 
data like configuration files. Unfortunately, json is not very convenient for such 
application because every string must be quoted and elements must be separated by commas. 
It is easy to parse by a program and to verify that it is correct, but it’s not connvenient
to write. 

For this reason, different alternatives to json have been proposed which are more human 
friendly like [yaml] or [hjson](https://hjson.github.io/) for instance. 

qjson is inspired by hjson by extending its functionality and relaxing some rules.

Here is a list of qjson text properties:

- comments of the form //...  #... or /*...*/
- commas between array values and object members are optional 
- double quote, single quote and quoteless strings
- non-breaking space is considered as a white space character
- newline is \n or \r\n, report \r alone or \n\r as an error
- numbers are integer, floating point, hexadecimal, octal or binary
- numbers may contain underscore '_' as separator
- numbers may be simple mathematical expression with parenthesis
- member identifiers may be quoteless strings including spaces
- the newline type in multiline string is explicitely specified
- backspace and form feed controls are invalid characters except
  in /*...*/ comments or multiline strings
- time durations expressed with w, d, h, m, s suffix are converted to seconds
- time specified in ISO format is converted to UTC time is seconds


## Usage 

Simply copy the qjson.h and qjson.c files in your project.

The API is a single function that receive the qjson text as input and
returns the corresponding json string or an error message.

`int qjson_decode(const char* qjsonText, char** output)`

The function returns 0 if the conversion to json succeeded,
or -1 in case of error. 

According to the return value, the address of a heap allocated json text 
or error message is stored in the output varible that is simply overwritten
and doesn’t need to be initialized prior the call. The user must 
free this string when done with it to avoid memory leaks.

Here is an example of usage:

```
#include <stdio.h>
#include "qjson.h"

int main(int argc, char *argv[]) {

    ... read qjsonText from file or other source ...

    // convert qjson to json
    char *output;
    if ((res = qjson_decode(qjsonText, &output)) < 0 {
        // an error was detected in the qjsonText 
        fprintf(stderr, "error: qjson: %s\n", output);
        free(output);
        return -1;
    }

    // output contains the json text
    printf("qjson: %s\n", output);

    ... process json text as usual ...

    free(output);
    return 0;
}
```

qjson-c has no dependency with other libraries. 

It is also possible to build a library and `qjson` program binary with `make`. Call `make test` 
to test the program, and `make clean` to remove all built files, including the libraries and 
the program. 

### qjson command line application

A qjson command line may be build with the provided main.c file.
To compile the qjson program use the following command:

`gcc --static main.c qjson.c -o qjson`

## Syntax 

**This package supports qjson syntax v0.0.0"**

THe qjson syntax is described in the 
[qjson-syntax project](http://github.com/qjson/qjson-syntax).

Check the syntax version supported by the package since
the syntax may evolve. 

## Reliability

qjson-c is a direct translation of qjson-go that has been extensively tested 
with manualy defined tests (100% coverage). AFL fuzzing was run on qjson-c
for many days without finding a simple hang or crash. For this fuzzing test,
the json text produced, when the input was valid, was checked with json-c to
detect invalid json. All json output in these tests is valid json.

This code should thus not crash or hang you program and json output is valid.
What could not be extensively and automatically tested is if the output is 
the expected output. It was only verified in the manual tests of qjson-go. 

For bug reports in this qjson-c package, fill an issue in this project. 

## Contributing

qjson is a recently developped package. It is thus a good time to 
suggest changes or extensions to the syntax since the user base is very
small. 

For suggestions or problems relative to syntax, fill an issue in the 
[qjson-syntax project](http://github.com/qjson/qjson-syntax).

Any contribution is welcome. 

## License

The licences is the 3-BSD clause. 
