# C bitarray implementation

## About

Dynamic bitarray implementation written in C.

This bitarray/bitset implementation supports plenty of bitarray-relevant features, such as 
- clearing/setting bit (ranges)
- fast counting of set bits in a range or the entire bitarray
- `&` (AND), `|` (OR), `^` (XOR), `~` (NOT), `>>` (RIGHT SHIFT) and `<<` (LEFT SHIFT) on bitarrays
- converting an (unsigned) number/string to a bitarray for easy bit manipulation
- converting a bitarray into a number or string

amongst others.

Unlike e.g. C++'s `std::bitset`, this bitarray implementation is dynamic, meaning the size of a bitarray doesn't need to be known at compile-time, which allows for some more flexibility.

## Installation/Usage

This repo provides **three** installation methods:

### Method 1

You can directly include `bitarray.h` into your `.c` file as a header-only "library". Simply add `#include "bitarray.h"` at the top of your `.c` file and compile it:

```
BIT_ARRAY_PATH=/path/to/this/repo
gcc -o mybinary -I$BIT_ARRAY_PATH main.c
```

It is recommended to also add the `-march=native` option to leverage optimized functions/assembly instructions for your specific machine.

### Method 2

Alternatively you can produce an `bitarray.o` file  by executing the command

```
make default
```

which you can simply link/combine with your binary:

```
BIT_ARRAY_PATH=/path/to/this/repo
gcc -I$(BIT_ARRAY_PATH) -o mybinary main.c path/to/bitarray.o
```

If you choose this method, make sure to include the `libbitarray.h` header file at the top of your main `.c` file instead of the `bitarray.h` header file to avoid linker errors.

### Method 3

The 3rd option is to create a shared library with the command

```
make shared
```

This will produce a `libbitarray.so` file, which you can link against.
To avoid linker errors, make sure to add the path where the `libbitarray.so` file is located to the `LD_LIBRARY_PATH` variable.

```
export LD_LIBRARY_PATH=/path/to/dir/of/libbitarray.so:$LD_LIBRARY_PATH
BIT_ARRAY_PATH=/path/to/this/repo
gcc -L/path/to/dir/of/libbitarray.so -I$BIT_ARRAY_PATH -o mybinary -lbitarray main.c
```

Again, make sure to include the `libbitarray.h` header file at the top of your main `.c` file instead of the `bitarray.h` header file to avoid linker errors.

By default, no bounds checking is performed in any of the functions to ensure maximum speed/performance. However, if you compile/link using the `Makefile` targets `debug` instead of `default` or `debug_shared` instead of `shared`, `assert` statements will be triggered if you attempt to access an out-of-bounds bit.

You can also run a small but thorough test program to check if everything works by executing the command
 
```
make test && ./test
```

## Contributing

I primarily wrote this implementation for practice reasons, so please feel free to open issues if you encounter any bugs or other problems. 

