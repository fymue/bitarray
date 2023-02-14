# bitarray

## About

Dynamic bitarray implementation written in C.
This bitarray/bitset implementation supports plenty of bitarray-relevant features, such as 
- clearing/setting bit (ranges)
- fast counting of set bits (in range)
- `&` (AND), `|` (OR), `^` (XOR), `~` (NOT), `>>` (RIGHT SHIFT) and `<<` (LEFT SHIFT) on bitarrays
- converting a (unsigned) number to a bitarray for easy bit manipulation (and back to a number)

amongst others.

Unlike e.g. `std::bitset`, this bitarray implementation is dynamic, meaning the size of a bitarray doesn't need to be known at compile-time, which allows for some more flexibility.

## Installation/Usage

This repo provides three installation methods:

You can directly include `bitarray.h` into your `.c` file as a header-only "library". Simply add `#include "bitarray.h"` at the top of your `.c` file and compile it:

```
gcc -o mybinary -I/path/to/dir/of/bitarray.h main.c
```

It is recommended to also add the `-march=native` option to leverage optimized functions/assembly instructions for your specific machine.

Alternatively you can produce an `bitarray.o` file  by executing the command

```
make
```

which you can simply link/combine with your binary:

```
gcc -o mybinary main.c path/to/bitarray.o
```

The 3rd option is to create a shared library with the command

```
make shared
```

This will produce a `libbitarray.so` file, which you can link against:

```
gcc -L/path/to/dir/of/libbitarray.so -o mybinary -lbitarray main.c
```

To avoid linker errors, make sure to add the path where the `libbitarray.so` file is located to the `LD_LIBRARY_PATH` variable and to export it.

```
LD_LIBRARY_PATH=/path/to/dir/of/libbitarray.so:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH
```

## Contributing

I primarily wrote this implementation for practice reasons, so please feel free to open issues if you encounter any bugs or other problems. 

