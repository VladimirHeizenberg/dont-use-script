# DontUseScript Interpreter

DontUseScript This is a custom scripting language designed with Python-like semantics (dynamic typing, garbage collection, first-class functions). This project implements the full execution pipeline: **Lexical Analysis**, **Parsing**, **Abstract Syntax Tree (AST)**, **Interpretation**.

## Key features

* Memory Model: Uses a reference-counting mechanism (implemented by using `std::shared_ptr`) where primitives are copied by value and complex types (strings, lists, functions) by reference.
* First-class functions: Functions can be assigned to variables, passed as arguments, and returned from other functions.* Dynamic arrays and strings
* Control flow: `if-else-then`, `for`, `while` with support of `break` and `continue`.
* Standart library: Built-in standart library for math and work with arrays and strings. Is a part of a language (interpreter, i.o.w. STD is implemented inside C++ code).

## Example code

This is how the code for evaluating Fibonacci numbers in DontUseScript can look like:
```
fib = function(n)
    if n == 0 then
        return 0
    end if

    a = 0
    b = 1

    for i in range(0, n - 1, 1)
        c = a + b
        a = b
        b = c
    end for

    return b
end function


print(fib(10))
```

## Building and testing

```
mkdir build && cd build
cmake ..
cmake --build . --target 
ctest -V
```