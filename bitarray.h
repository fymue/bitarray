#ifndef _bit_array_h_
#define _bit_array_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct _bit_array {
  size_t size;
  uint64_t *array;
} bitarray;

// get bit at idx
bool get_bit(bitarray *bit_array, size_t idx);

// set bit at idx
void set_bit(bitarray *bit_array, size_t idx);

// set all bits to boolean value val
void set_all_bits(bitarray *bit_array, bool val);

// set bits in range [from, to) to boolean value val
void set_bit_range(bitarray *bit_array, size_t from, size_t to, bool val);

// flip/invert bit at idx
void flip_bit(bitarray *bit_array, size_t idx);

// flip/invert bits in range [from, to]
void flip_bit_range(bitarray *bit_array, size_t from, size_t to);

// flip/invert all bits (~)
void flip_all_bits(bitarray *bit_array);

// count all true bits
size_t count_bits(bitarray *bit_array);

// count true bits in range [from, to)
size_t count_bit_range(bitarray *bit_array, size_t from, size_t to);

// clear/reset bits to "false" in range [from, to)
void clear_bit_range(bitarray *bit_array, size_t from, size_t to);

// clear/reset all bits to "false"
void clear_all_bits(bitarray *bit_array);

// inplace bitwise operations

// perform bitwise AND (&=) on left bitarray in-place
void and_bits(bitarray *left, bitarray *right);

// perform bitwise OR (|=) on left bitarray in-place
void or_bits(bitarray *left, bitarray *right);

// perform bitwise XOR (^=) on left bitarray in-place
void xor_bits(bitarray *left, bitarray *right);

// perform NOT (~) on bitarray in-place (same functionality as clear_all_bits function)
void not_bits(bitarray *bit_array);

// perform rightshift (>>=) on bitarray in-place
void shift_bits_right(bitarray *bit_array, size_t n);

// perform left shift (<<=) in bitarray in-place
void shift_bits_left(bitarray *bit_array, size_t n);

// copy all bits from src to dest
void copy_all_bits(bitarray *src, bitarray *dest);

// copy bits in range [from, to] from src to dest
void copy_bit_range(bitarray *src, bitarray *dest, size_t from, size_t to);

// "constructor" functions

// create bitarray with initial size
bitarray* create_bitarray(size_t size);

// create bitarray with initial size and all bits set to val
bitarray* create_bitarray_with_val(size_t size, bool val);

// create bitarray from string (must consist only of 0 and 1)
bitarray* create_bitarray_from_str(const char *str);

// create bitarray from number
bitarray* create_bitarray_from_num(__uint128_t num);

// "destructor" function

// delete bitarray and free allocated memory
void delete_bitarray(bitarray *bit_array);

#endif  // _bit_array_h_
