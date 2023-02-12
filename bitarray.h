#ifndef BITARRAY_H_
#define BITARRAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define ARRAY_TYPE uint64_t
#define ARRAY_TYPE_MAX UINT64_MAX
#define TYPE_SIZE sizeof(ARRAY_TYPE)
#define BITS_PER_EL (TYPE_SIZE * 8)

typedef struct _bit_array {
  size_t size;
  size_t _array_size;
  uint64_t *array;
} bitarray;

// figure out how many array elements are needed to store n_bits bits
size_t __bitarray_size(size_t n_bits);

// get bit at idx
bool get_bit(bitarray *bit_array, size_t idx);

// set bit at idx
void set_bit(bitarray *bit_array, size_t idx);

// set bits in range [from, to) to "true"
void set_bit_range(bitarray *bit_array, size_t from, size_t to);

// set all bits to "true"
void set_all_bits(bitarray *bit_array);

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

// clear bit at idx
void clear_bit(bitarray *bit_array, size_t idx);

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

// perform NOT (~) on bitarray in-place
// (same functionality as clear_all_bits function)
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

// create bitarray that holds n_bits bits (all unset/false)
bitarray* create_bitarray(size_t n_bits);

// create bitarray that holds n_bits bits (all set/true)
bitarray* create_set_bitarray(size_t n_bits);

// create bitarray from string (must consist only of 0 and 1)
bitarray* create_bitarray_from_str(const char *str, size_t str_len);

// create bitarray from number
bitarray* create_bitarray_from_num(__uint128_t num);

// "destructor" function

// delete bitarray and free allocated memory
void delete_bitarray(bitarray *bit_array);

// print each bit of the bitarray
void print_bitarray(bitarray *bit_array);

// create a string from the bitarray (don't forget to free it at then end!)
char* make_string_from_bitarray(bitarray *bit_array);

#endif  // BITARRAY_H_
