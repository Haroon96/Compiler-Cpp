# Compiler-Cpp
A "built-from-scratch" code compiler in C++.

This project implements a "hybrid" compiler (similar to Java) for a custom language syntax. It follows a four step process: tokenization, parsing, translation, and interpretation. Two output binaries are generated from this source, the compiler and the interpreter. The `compile` binary compiles a given source code to a byte code format that can be interpreted by the `interpret` binary. In addition, the verbosity flag can be set during compilation which outputs the intermediate tokens generated, the parse tree for the code, and a human-friendly byte coded map of the program.

## Features
* Support for functions and recursive calls
* Strict checking parameter lists
* Logical branching (if / else / loops)
* Complex expression evaluation
* Basic User I/O

## Limitations
Of course, this isn't a fully featured compiler and lacks support for other, more common features. Some limitations are,
* No support for strings, only literal constants
* Arrays can not be passed to functions

## Language Syntax
The `samples` folder contains some sample programs that demonstrate the syntax for the language. The following snippet should, however, provide a base outline.

```
# Comments can be written by a leading #

def foo(int i) {          # functions are declared using the def keyword
    int r                 # variable declaration (or initialize it instead)
    if i == 1 {           # conditions aren't surrounded by parenthesis
      r = 1               # look ma, no semi-colons
    } else                # or go brace-less
      r = i + foo(i - 1)  # recursive call to self
    return r              # no early-returns allowed
}

def main() {              # main method must exist
    int i                 # you can initialize a variable too!
    gets i                # input from user
    print foo(i) * 10     # call foo with parameter i, multiply it by 10, and print the result
}

```

## Implementation Details
The compiler is implemented as a four step process. The individual components that comprise each step are loosely coupled for better understanding of the process and are, namely,
1. Tokenization (Lexical analysis)
2. Parsing (Syntax analysis)
3. Translation (to a byte code format)
4. Interpretation (using a virtual machine)

## Compiling the Compiler (Build Process)
A makefile is included in the repo that was tested for `make` on Windows (using cygwin). Should work for Linux machines as well.

## Usage
```
  compile [-v] filename
    Generates an object code file for the source in filename.
    Using -v sets the verbosity flag and causes intermediate outputs to be written to respective files.
```
```
  interpret filename
    Interprets the object code inside filename.
```
## Contributors
- Muhammad Haroon [(Haroon96)](https://github.com/Haroon96)
- Sara Iftikhar
