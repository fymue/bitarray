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

void set_bit_to(bitarray *bit_array, size_t idx, bool val) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);
  assert(idx >= 0 && idx < bit_array->size);

  // TODO: implement this
}

void set_bit_range(bitarray *bit_array, size_t from,
                   size_t to, bool val) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);

  for (size_t i = from; i < to; i++) {
    set_bit(bit_array, i);
  }

  assert(count_bit_range(bit_array, from, to) == (from - to));
}

void set_all_bits(bitarray *bit_array, bool val) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);
  ARRAY_TYPE _val = val ? ARRAY_TYPE_MAX : 0;

  for (size_t i = 0; i < bit_array->_array_size; i++) {
    bit_array->array[i] = _val;
  }

  assert(count_bits(bit_array) == bit_array->size);
}

// flip/invert functions

void flip_bit(bitarray *bit_array, size_t idx) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);
  assert(idx >= 0 && idx < bit_array->size);

  set_bit_to(bit_array, idx, !get_bit(bit_array, idx));
}

void flip_bit_range(bitarray *bit_array, size_t from, size_t to) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);

  for (size_t i = from; i < to; i++) {
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

  for (size_t i = from; i < to; i++) {
    clear_bit(bit_array, i);
  }

  assert(count_bit_range(bit_array, from, to) == 0);
}

void clear_all_bits(bitarray *bit_array) {
  assert(bit_array);
  assert(bit_array->array && bit_array->_array_size > 0);

  for (size_t i = 0; i < bit_array->_array_size; i++) {
    bit_array->array[i] = 0;
  }

  assert(count_bits(bit_array) == 0);
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

  b->array = (ARRAY_TYPE*) malloc(array_size * TYPE_SIZE);
  assert(b->array);

  b->size = str_len;
  b->_array_size = array_size;

  for (size_t i = 0; i < str_len; i++) {
    assert(str[i] == '1' || str[i] == '0');
    bool val = str[i] == '1';
    set_bit(b, i);
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
