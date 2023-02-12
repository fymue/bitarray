#include "bitarray.h"

inline size_t __bitarray_size(size_t n_bits) {
  /*
     calculate number of array elements needed to fit n_bits bits;
     if n_bits % (TYPE_SIZE * 8) == 0, the calculated size will 
     be too big by 1; but that's ok since only 8 additional bytes
     will be allocated
  */
  assert(n_bits >= 0);

  return n_bits / (TYPE_SIZE * 8) + 1;
}

// get bit at idx
bool get_bit(bitarray *bit_array, size_t idx) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);
  assert(idx >= 0 && idx < bit_array->size);

  // array index where bit at idx lives
  size_t array_idx = idx / BITS_PER_EL;

  // position of bit at array index/value
  uint8_t offset = idx % BITS_PER_EL;

  // left-shift 1 by offset and AND it with array value
  // if the bit is set, this will result in a positive number (==true)
  // if not, this will result i 0 (==false)
  return bit_array->array[array_idx] & (((ARRAY_TYPE)1) << offset);
}

// set functions

void set_bit(bitarray *bit_array, size_t idx) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);
  assert(idx >= 0 && idx < bit_array->size);

  // array index where bit at idx lives
  size_t array_idx = idx / BITS_PER_EL;

  // position of bit at array index/value
  uint8_t offset = idx % BITS_PER_EL;

  // OR array value in-place with 1 leftshifted by offset (will set the bit)
  bit_array->array[array_idx] |= (((ARRAY_TYPE)1) << offset);
  assert(get_bit(bit_array, idx));
}

void set_bit_range(bitarray *bit_array, size_t from, size_t to) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);

  // if the size of the bitarray is <= BITS_PER_EL,
  // there is no more efficient way than to iterate
  // over evey bit position and to set it
  if (bit_array->size <= BITS_PER_EL) {
    for (size_t i = from; i < to; i++) {
      set_bit(bit_array, i);
    }
    return;
  }

  // array index where bit at "from" lives
  size_t array_idx_from = from / BITS_PER_EL;

  // array index where bit at "to" lives
  size_t array_idx_to = to / BITS_PER_EL;

  // position of "from" bit at array index/value
  uint8_t offset_from = from % BITS_PER_EL;

  // position of "to" bit at array index/value
  uint8_t offset_to = to % BITS_PER_EL;

  size_t end_idx_from = from + (BITS_PER_EL - offset_from);
  for (size_t i = from; i < end_idx_from; i++) {
    set_bit(bit_array, i);
  }

  // set entire array value (BITS_PER_EL bits) to "true"
  // without iterating over every bit position
  for (size_t i = array_idx_from + 1; i < array_idx_to; i++) {
    bit_array->array[i] = ARRAY_TYPE_MAX;
  }

  for (size_t i = to - offset_to; i < to; i++) {
    set_bit(bit_array, i);
  }

  //assert(count_bit_range(bit_array, from, to) == (from - to));
}

void set_all_bits(bitarray *bit_array) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);

  for (size_t i = 0; i < bit_array->_array_size; i++) {
    bit_array->array[i] = ARRAY_TYPE_MAX;
  }

  assert(count_bits(bit_array) == bit_array->size);
}

// flip/invert functions

void flip_bit(bitarray *bit_array, size_t idx) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);
  assert(idx >= 0 && idx < bit_array->size);

  // array index where bit at idx lives
  size_t array_idx = idx / BITS_PER_EL;

  // position of bit at array index/value
  uint8_t offset = idx % BITS_PER_EL;

  // XOR array value in-place with 1 leftshifted by offset
  // (will flip the bit)
  bit_array->array[array_idx] ^= (((ARRAY_TYPE)1) << offset);
}

void flip_bit_range(bitarray *bit_array, size_t from, size_t to) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);

  // if the size of the bitarray is <= BITS_PER_EL,
  // there is no more efficient way than to iterate
  // over evey bit position and to flip it
  if (bit_array->size <= BITS_PER_EL) {
    for (size_t i = from; i < to; i++) {
      flip_bit(bit_array, i);
    }
    return;
  }

  // array index where bit at "from" lives
  size_t array_idx_from = from / BITS_PER_EL;

  // array index where bit at "to" lives
  size_t array_idx_to = to / BITS_PER_EL;

  // position of "from" bit at array index/value
  uint8_t offset_from = from % BITS_PER_EL;

  // position of "to" bit at array index/value
  uint8_t offset_to = to % BITS_PER_EL;

  size_t end_idx_from = from + (BITS_PER_EL - offset_from);
  for (size_t i = from; i < end_idx_from; i++) {
    flip_bit(bit_array, i);
  }

  // flip entire array value (BITS_PER_EL bits)
  // without iterating over every bit position
  for (size_t i = array_idx_from + 1; i < array_idx_to; i++) {
    bit_array->array[i] = ~(bit_array->array[i]);
  }

  for (size_t i = to - offset_to; i < to; i++) {
    flip_bit(bit_array, i);
  }
}

void flip_all_bits(bitarray *bit_array) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);

  for (size_t i = 0; i < bit_array->_array_size; i++) {
    ~(bit_array->array[i]);
  }
}

// count functions

size_t count_bits(bitarray *bit_array) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);

  // TODO: implement this
}

size_t count_bit_range(bitarray *bit_array, size_t from, size_t to) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);

  // TODO: implement this
}

// clear functions

void clear_bit(bitarray *bit_array, size_t idx) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);
  assert(idx >= 0 && idx < bit_array->size);

  // array index where bit at idx lives
  size_t array_idx = idx / BITS_PER_EL;

  // position of bit at array index/value
  uint8_t offset = idx % BITS_PER_EL;

  // AND array value in-place with 1 leftshifted by offset FLIPPED
  // (the flip turns 100000 int 011111, which will clear the 0 bit after AND)
  bit_array->array[array_idx] &= ~(((ARRAY_TYPE)1) << offset);
  assert(!get_bit(bit_array, idx));
}

void clear_bit_range(bitarray *bit_array, size_t from, size_t to) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);

  // if the size of the bitarray is <= BITS_PER_EL,
  // there is no more efficient way than to iterate
  // over evey bit position and to clear it
  if (bit_array->size <= BITS_PER_EL) {
    for (size_t i = from; i < to; i++) {
      clear_bit(bit_array, i);
    }
    return;
  }

  // array index where bit at "from" lives
  size_t array_idx_from = from / BITS_PER_EL;

  // array index where bit at "to" lives
  size_t array_idx_to = to / BITS_PER_EL;

  // position of "from" bit at array index/value
  uint8_t offset_from = from % BITS_PER_EL;

  // position of "to" bit at array index/value
  uint8_t offset_to = to % BITS_PER_EL;

  size_t end_idx_from = from + (BITS_PER_EL - offset_from);
  for (size_t i = from; i < end_idx_from; i++) {
    clear_bit(bit_array, i);
  }

  // flip entire array value (BITS_PER_EL bits)
  // without iterating over every bit position
  for (size_t i = array_idx_from + 1; i < array_idx_to; i++) {
    bit_array->array[i] = 0;
  }

  for (size_t i = to - offset_to; i < to; i++) {
    clear_bit(bit_array, i);
  }

  //assert(count_bit_range(bit_array, from, to) == 0);
}

void clear_all_bits(bitarray *bit_array) {
  assert(bit_array);
  assert(bit_array->array && bit_array->_array_size > 0);

  for (size_t i = 0; i < bit_array->_array_size; i++) {
    bit_array->array[i] = 0;
  }

  //assert(count_bits(bit_array) == 0);
}

// bitwise operations

void and_bits_inplace(bitarray *left, bitarray *right) {
  assert(left && right);
  assert(left->size && right->size);

  // need to know which array is bigger to avoid out of bounds access
  size_t mx_i = left->_array_size < right->_array_size ?
                left->_array_size : right->_array_size;

  for (size_t i = 0; i < mx_i; i++) {
    left->array[i] &= right->array[i];
  }
}

void or_bits_inplace(bitarray *left, bitarray *right) {
  assert(left && right);
  assert(left->size && right->size);

  // need to know which array is bigger to avoid out of bounds access
  size_t mx_i = left->_array_size < right->_array_size ?
                left->_array_size : right->_array_size;

  for (size_t i = 0; i < mx_i; i++) {
    left->array[i] |= right->array[i];
  }
}

void xor_bits_inplace(bitarray *left, bitarray *right) {
  assert(left && right);
  assert(left->size && right->size);

  // need to know which array is bigger to avoid out of bounds access
  size_t mx_i = left->_array_size < right->_array_size ?
                left->_array_size : right->_array_size;

  for (size_t i = 0; i < mx_i; i++) {
    left->array[i] ^= right->array[i];
  }
}

bitarray* and_bits(bitarray *left, bitarray *right) {
  assert(left && right);
  assert(left->size && right->size);

  // need to know which array is bigger to avoid out of bounds access
  size_t n_bits, mx_i;
  if (left->size < right->size) {
    mx_i = left->_array_size;
    n_bits = left->size;
  } else {
    mx_i = right->_array_size;
    n_bits = right->size;
  }

  bitarray *b = create_bitarray(n_bits);

  for (size_t i = 0; i < mx_i; i++) {
    b->array[i] = (left->array[i]) & (right->array[i]);
  }

  return b;
}

bitarray* or_bits(bitarray *left, bitarray *right) {
  assert(left && right);
  assert(left->size && right->size);

  // need to know which array is bigger to avoid out of bounds access
  size_t n_bits, mx_i;
  if (left->size < right->size) {
    mx_i = left->_array_size;
    n_bits = left->size;
  } else {
    mx_i = right->_array_size;
    n_bits = right->size;
  }

  bitarray *b = create_bitarray(n_bits);

  for (size_t i = 0; i < mx_i; i++) {
    b->array[i] = (left->array[i]) | (right->array[i]);
  }

  return b;
}

bitarray* xor_bits(bitarray *left, bitarray *right) {
  assert(left && right);
  assert(left->size && right->size);

  // need to know which array is bigger to avoid out of bounds access
  size_t n_bits, mx_i;
  if (left->size < right->size) {
    mx_i = left->_array_size;
    n_bits = left->size;
  } else {
    mx_i = right->_array_size;
    n_bits = right->size;
  }

  bitarray *b = create_bitarray(n_bits);

  for (size_t i = 0; i < mx_i; i++) {
    b->array[i] = (left->array[i]) ^ (right->array[i]);
  }

  return b;
}

void not_bits(bitarray *bit_array) {
  flip_all_bits(bit_array);
}

// copy functions

void copy_all_bits(bitarray *src, bitarray *dest) {
  assert(src && dest);
  if (!(dest->array)) {
    dest->array = (ARRAY_TYPE*) malloc(src->_array_size * TYPE_SIZE);
  }

  if (dest->_array_size < src->_array_size) {
    free(dest->array);
    dest->array = (ARRAY_TYPE*) malloc(src->_array_size * TYPE_SIZE);
    dest->_array_size = src->_array_size;
  }

  assert(src->_array_size == dest->_array_size);

  for (size_t i = 0; i < src->_array_size; i++) {
    dest->array[i] = src->array[i];
  }

  dest->size = src->size;
}

void copy_bit_range(bitarray *src, bitarray *dest,
                    size_t from, size_t to) {
  assert(src && dest);
  if (!(dest->array) || dest->size < (to - from)) {
    if (dest->array) free(dest->array);
    size_t array_size = __bitarray_size(to - from);
    dest->array = (ARRAY_TYPE*) malloc(array_size * TYPE_SIZE);
    dest->_array_size = array_size;
  }

  dest->size = to - from;

  if (src->size <= BITS_PER_EL) {
    for (size_t i = from; i < to; i++) {
      if (get_bit(src, i)) {
        set_bit(dest, i - from);
      } else {
        clear_bit(dest, i - from);
      }
    }
    return;
  }

  // array index where bit at "from" lives
  size_t array_idx_from = from / BITS_PER_EL;

  // array index where bit at "to" lives
  size_t array_idx_to = to / BITS_PER_EL;

  // position of "from" bit at array index/value
  uint8_t offset_from = from % BITS_PER_EL;

  // position of "to" bit at array index/value
  uint8_t offset_to = to % BITS_PER_EL;

  size_t end_idx_from = from + (BITS_PER_EL - offset_from);

  for (size_t i = from; i < end_idx_from; i++) {
    if (get_bit(src, i)) {
      set_bit(dest, i);
    } else {
      clear_bit(dest, i);
    }
  }

  // copy entire array value (BITS_PER_EL bits)
  // without iterating over every bit position
  for (size_t i = array_idx_from + 1; i < array_idx_to; i++) {
    dest->array[i] = src->array[i];
  }

  for (size_t i = to - offset_to; i < to; i++) {
    if (get_bit(src, i)) {
      set_bit(dest, i);
    } else {
      clear_bit(dest, i);
    }
  }
}

// "constructor" functions

bitarray* create_bitarray(size_t n_bits) {
  size_t array_size = __bitarray_size(n_bits);
  assert(array_size > 0);
  bitarray *b = (bitarray*) malloc(sizeof(bitarray));
  assert(b);

  // zero-initialize the memory so every bit is set to 0
  b->array = (ARRAY_TYPE*) calloc(array_size, TYPE_SIZE);
  assert(b->array);

  b->size = n_bits;
  b->_array_size = array_size;

  return b;
}

bitarray* create_set_bitarray(size_t n_bits) {
  size_t array_size = __bitarray_size(n_bits);
  assert(array_size > 0);
  bitarray *b = (bitarray*) malloc(sizeof(bitarray));
  assert(b);

  b->array = (ARRAY_TYPE*) malloc(array_size * TYPE_SIZE);
  assert(b->array);

  // set all bits
  for (size_t i = 0; i < array_size; i++) {
    b->array[i] = ARRAY_TYPE_MAX;
  }

  b->size = n_bits;
  b->_array_size = array_size;

  return b;
}

bitarray* create_bitarray_from_str(const char *str, size_t str_len) {
  size_t array_size = __bitarray_size(str_len);
  assert(array_size > 0);
  bitarray *b = (bitarray*) malloc(sizeof(bitarray));
  assert(b);

  b->array = (ARRAY_TYPE*) calloc(array_size, TYPE_SIZE);
  assert(b->array);

  b->size = str_len;
  b->_array_size = array_size;

  for (size_t i = 0; i < str_len; i++) {
    assert(str[i] == '1' || str[i] == '0');
    if (str[i] == '1') {
      set_bit(b, i);
    }
  }

  return b;
}

bitarray* create_bitarray_from_num(__uint128_t num) {
  size_t n_bits = sizeof(num);
  size_t array_size = __bitarray_size(n_bits);
  assert(array_size > 0);
  bitarray *b = (bitarray*) malloc(sizeof(bitarray));
  assert(b);

  b->array = (ARRAY_TYPE*) malloc(array_size * TYPE_SIZE);
  assert(b->array);

  b->size = n_bits;
  b->_array_size = array_size;

  // TODO: figure out how to access each bit of num and add it to b

  return b;
}

// "destructor" functions
void delete_bitarray(bitarray *bit_array) {
  assert(bit_array);
  assert(bit_array->array);
  free(bit_array->array);
  free(bit_array);
}

void print_bitarray(bitarray *bit_array) {
  assert(bit_array);
  assert(bit_array->array);

  for (size_t i = 0; i < bit_array->size; i++) {
    printf("%d", get_bit(bit_array, i));
  }
  printf("\n");
}

char* make_string_from_bitarray(bitarray *bit_array) {
  assert(bit_array);
  assert(bit_array->array);

  size_t size = bit_array->size + 1;
  char* str = (char*) malloc(size * sizeof(char));

  size_t i;
  for (i = 0; i < bit_array->size; i++) {
    str[i] = get_bit(bit_array, i) ? '1' : '0';
  }

  str[i] = '\0';
  assert(strlen(str) == bit_array->size);

  return str;
}

bool equal_bits(bitarray *left, bitarray *right) {
  assert(left && right);
  assert(left->size == right->size);

  for (size_t i = 0; i < left->_array_size; i++) {
    if (left->array[i] != right->array[i]) {
      return false;
    }
  }

  return true;
}
