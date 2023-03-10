#include "libbitarray.h"

inline size_t __bitarray_size(size_t n_bits) {
  /*
     calculate number of array elements needed to fit n_bits bits;
     if n_bits % (TYPE_SIZE * 8) == 0, the calculated size will 
     be too big by 1; but that's ok since only 8 additional bytes
     will be allocated
  */
  assert(n_bits > 0);

  return n_bits / (TYPE_SIZE * 8) + 1;
}

// get bit at idx
bool get_bit(bitarray *bit_array, size_t idx) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);
  assert(idx < bit_array->size);

  // array index where bit at idx lives
  size_t array_idx = idx / BITS_PER_EL;

  // position of bit at array index/value
  uint8_t offset = idx % BITS_PER_EL;

  // left-shift 1 by offset and AND it with array value
  // if the bit is set, this will result in a positive number (==true)
  // if not, this will result i 0 (==false)
  return bit_array->array[array_idx] & (MASK_1 << offset);
}

// set functions

void set_bit(bitarray *bit_array, size_t idx) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);
  assert(idx < bit_array->size);

  // array index where bit at idx lives
  size_t array_idx = idx / BITS_PER_EL;

  // position of bit at array index/value
  uint8_t offset = idx % BITS_PER_EL;

  // OR array value in-place with 1 leftshifted by offset (will set the bit)
  bit_array->array[array_idx] |= (MASK_1 << offset);
  assert(get_bit(bit_array, idx));
}

void set_bit_range(bitarray *bit_array, size_t from, size_t to) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);
  assert(to <= bit_array->size);

  // if the size of the bitarray is <= BITS_PER_EL,
  // there is no more efficient way than to iterate
  // over evey bit position and to set it
  if (bit_array->size <= BITS_PER_EL || (to - from) <= BITS_PER_EL) {
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

  assert(count_bit_range(bit_array, from, to) == (to - from));
}

void set_all_bits(bitarray *bit_array) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);

  for (size_t i = 0; i < bit_array->_array_size; i++) {
    bit_array->array[i] = ARRAY_TYPE_MAX;
  }

  // clear the bits after n_bits so only bits < n_bits are set
  size_t capacity = bit_array->_array_size * BITS_PER_EL;
  size_t size = bit_array->size;

  // temporarily increase size to capacity (this is safe here)
  bit_array->size = capacity;
  clear_bit_range(bit_array, size, capacity);
  bit_array->size = size;  // change it back

  assert(count_bits(bit_array) == bit_array->size);
}

// flip/invert functions

void flip_bit(bitarray *bit_array, size_t idx) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);
  assert(idx < bit_array->size);

  // array index where bit at idx lives
  size_t array_idx = idx / BITS_PER_EL;

  // position of bit at array index/value
  uint8_t offset = idx % BITS_PER_EL;

  // XOR array value in-place with 1 leftshifted by offset
  // (will flip the bit)
  bit_array->array[array_idx] ^= (MASK_1 << offset);
}

void flip_bit_range(bitarray *bit_array, size_t from, size_t to) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);
  assert(to <= bit_array->size);

  // if the size of the bitarray is <= BITS_PER_EL,
  // there is no more efficient way than to iterate
  // over evey bit position and to flip it
  if (bit_array->size <= BITS_PER_EL || (to - from) <= BITS_PER_EL) {
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
    bit_array->array[i] = ~(bit_array->array[i]);
  }

  // keep free bits cleared
  size_t capacity = bit_array->_array_size * BITS_PER_EL;
  size_t size = bit_array->size;
  bit_array->size = capacity;
  clear_bit_range(bit_array, size, capacity);
  bit_array->size = size;
}

// count functions

size_t count_bits(bitarray *bit_array) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);

  size_t count = 0;
  for (size_t i = 0; i < bit_array->_array_size; i++) {
    // use machine-optimized popcount function for max speed
    count += pop_count(bit_array->array[i]);
  }

  return count;
}

size_t count_bit_range(bitarray *bit_array, size_t from, size_t to) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);
  assert(to <= bit_array->size);

  size_t count = 0;
  // if the size of the bitarray is <= BITS_PER_EL,
  // there is no more efficient way than to iterate
  // over evey bit position and to count it
  if (bit_array->size <= BITS_PER_EL || (to - from) <= BITS_PER_EL) {
    for (size_t i = from; i < to; i++) {
      count += get_bit(bit_array, i);
    }
    return count;
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
    count += get_bit(bit_array, i);
  }

  // get popcount of entire array value
  // without iterating over every bit position
  for (size_t i = array_idx_from + 1; i < array_idx_to; i++) {
    count += pop_count(bit_array->array[i]);
  }

  for (size_t i = to - offset_to; i < to; i++) {
    count += get_bit(bit_array, i);
  }

  return count;
}

// clear functions

void clear_bit(bitarray *bit_array, size_t idx) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);
  assert(idx < bit_array->size);

  // array index where bit at idx lives
  size_t array_idx = idx / BITS_PER_EL;

  // position of bit at array index/value
  uint8_t offset = idx % BITS_PER_EL;

  // AND array value in-place with 1 leftshifted by offset FLIPPED
  // (the flip turns 100000 int 011111, which will clear the 0 bit after AND)
  bit_array->array[array_idx] &= ~(MASK_1 << offset);
  assert(!get_bit(bit_array, idx));
}

void clear_bit_range(bitarray *bit_array, size_t from, size_t to) {
  assert(bit_array);
  assert(bit_array->array && bit_array->size > 0);
  assert(to <= bit_array->size);

  // if the size of the bitarray is <= BITS_PER_EL,
  // there is no more efficient way than to iterate
  // over evey bit position and to clear it
  if (bit_array->size <= BITS_PER_EL || (to - from) <= BITS_PER_EL) {
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

// bitwise operations (in-place)

void and_bits_inplace(bitarray *left, bitarray *right) {
  assert(left && right);
  assert(left->size && right->size);

  if (left->size != right->size) {
    printf("Cannot AND bitarrays that aren't the same size (%ld != %ld)\n",
           left->size, right->size);
    exit(1);
  }

  for (size_t i = 0; i < left->_array_size; i++) {
    left->array[i] &= right->array[i];
  }
}

void or_bits_inplace(bitarray *left, bitarray *right) {
  assert(left && right);
  assert(left->size == right->size);

  if (left->size != right->size) {
    printf("Cannot OR bitarrays that aren't the same size (%ld != %ld)\n",
           left->size, right->size);
    exit(1);
  }

  for (size_t i = 0; i < left->_array_size; i++) {
    left->array[i] |= right->array[i];
  }
}

void xor_bits_inplace(bitarray *left, bitarray *right) {
  assert(left && right);
  assert(left->size && right->size);

  if (left->size != right->size) {
    printf("Cannot XOR bitarrays that aren't the same size (%ld != %ld)\n",
           left->size, right->size);
    exit(1);
  }

  for (size_t i = 0; i < left->_array_size; i++) {
    left->array[i] ^= right->array[i];
  }
}

void not_bits_inplace(bitarray *bit_array) {
  flip_all_bits(bit_array);
}

void right_shift_bits_inplace(bitarray *bit_array, size_t n) {
  assert(bit_array && bit_array->size);
  assert(n <= bit_array->size);
  if (!n) return;

  // probably not the most efficient way of doing it,
  // but simply append the bits after the shift to a new (cleared) bitarray
  // and swap the array ptr of the input bitarray
  bitarray *tmp = create_bitarray(bit_array->size);
  tmp->size = 0;
  append_bit_range(bit_array, tmp, n, bit_array->size);

  ARRAY_TYPE *old = bit_array->array;
  bit_array->array = tmp->array;

  free(old);
  free(tmp);
}

void left_shift_bits_inplace(bitarray *bit_array, size_t n) {
  assert(bit_array && bit_array->size);
  assert(n <= bit_array->size);
  if (!n) return;

  // probably not the most efficient way of doing it,
  // but simply append the bits after the shift to a new (cleared) bitarray
  // and swap the array ptr of the input bitarray
  bitarray *tmp = create_bitarray(bit_array->size);
  tmp->size = n;
  append_bit_range(bit_array, tmp, 0, bit_array->size - n);

  ARRAY_TYPE *old = bit_array->array;
  bit_array->array = tmp->array;

  free(old);
  free(tmp);
}

// bitwise operations
bitarray* and_bits(bitarray *left, bitarray *right) {
  assert(left && right);
  assert(left->size && right->size);

  if (left->size != right->size) {
    printf("Cannot AND bitarrays that aren't the same size (%ld != %ld)\n",
           left->size, right->size);
    exit(1);
  }

  bitarray *b = copy_bitarray(left);

  for (size_t i = 0; i < left->_array_size; i++) {
    b->array[i] &= right->array[i];
  }

  return b;
}

bitarray* or_bits(bitarray *left, bitarray *right) {
  assert(left && right);
  assert(left->size && right->size);

  if (left->size != right->size) {
    printf("Cannot OR bitarrays that aren't the same size (%ld != %ld)\n",
           left->size, right->size);
    exit(1);
  }

  bitarray *b = copy_bitarray(left);

  for (size_t i = 0; i < left->_array_size; i++) {
    b->array[i] |= right->array[i];
  }

  return b;
}

bitarray* xor_bits(bitarray *left, bitarray *right) {
  assert(left && right);
  assert(left->size && right->size);

  if (left->size != right->size) {
    printf("Cannot XOR bitarrays that aren't the same size (%ld != %ld)\n",
           left->size, right->size);
    exit(1);
  }

  bitarray *b = copy_bitarray(left);

  for (size_t i = 0; i < left->_array_size; i++) {
    b->array[i] ^= right->array[i];
  }

  return b;
}

bitarray* not_bits(bitarray *bit_array) {
  assert(bit_array && bit_array->size);

  bitarray *b = copy_bitarray(bit_array);
  flip_all_bits(b);

  return b;
}

bitarray* right_shift_bits(bitarray *bit_array, size_t n) {
  assert(bit_array && bit_array->size);
  assert(n <= bit_array->size);
  if (!n) return bit_array;

  bitarray *b = create_bitarray(bit_array->size);
  b->size = 0;
  append_bit_range(bit_array, b, n, bit_array->size);
  b->size += n;

  return b;
}

bitarray* left_shift_bits(bitarray *bit_array, size_t n) {
  assert(bit_array && bit_array->size);
  assert(n <= bit_array->size);
  if (!n) return bit_array;

  bitarray *b = create_bitarray(bit_array->size);
  b->size = n;
  append_bit_range(bit_array, b, 0, bit_array->size - n);

  return b;
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
  assert(to <= src->size);

  if (!(dest->array) || dest->size < (to - from)) {
    if (dest->array) free(dest->array);
    size_t array_size = __bitarray_size(to - from);
    dest->array = (ARRAY_TYPE*) calloc(array_size, TYPE_SIZE);
    dest->_array_size = array_size;
  }

  dest->size = to - from;

  if (src->size <= BITS_PER_EL || (to - from) <= BITS_PER_EL) {
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

void append_all_bits(bitarray *src, bitarray *dest) {
  assert(src && dest);

  if (!(src->size)) return;

  append_bit_range(src, dest, 0, src->size);
}

void append_bit_range(bitarray *src, bitarray *dest,
                      size_t from, size_t to) {
  assert(src && dest);
  assert(to <= src->size);

  if (!(src->size)) return;

  size_t capacity = dest->_array_size * BITS_PER_EL;
  size_t free_bits = capacity - dest->size;
  size_t old_size = dest->size;
  dest->size += (to - from);

  if (src->size > free_bits) {
    size_t new_array_size = dest->_array_size +
                            (src->size / BITS_PER_EL + 1);

    dest->array = (ARRAY_TYPE*) realloc(dest->array, new_array_size);
    dest->_array_size = new_array_size;
    clear_bit_range(dest, old_size, dest->size);
  }

  for (size_t i = from, j = old_size; i < to; i++, j++) {
    if (get_bit(src, i)) set_bit(dest, j);
  }
}

bitarray* copy_bitarray(bitarray *bit_array) {
  assert(bit_array && bit_array->size > 0);

  bitarray *b = create_bitarray(bit_array->size);
  copy_all_bits(bit_array, b);

  return b;
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

  assert(count_bits(b) == 0);

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

  // clear the bits after n_bits so only bits < n_bits are set
  size_t capacity = b->_array_size * BITS_PER_EL;

  // temporarily increase size to capacity (this is safe here)
  b->size = capacity;
  clear_bit_range(b, n_bits, capacity);
  b->size = n_bits;  // change it back

  assert(count_bits(b) == b->size);

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

  // add 1s and 0s in reverse order so the rightmost bit
  // lives at idx 0 and so on...
  for (size_t i = 0, j = str_len; j-- > 0; i++) {
    assert(str[i] == '1' || str[i] == '0');
    if (str[j] == '1') {
      set_bit(b, i);
    }
  }

  return b;
}

bitarray* create_bitarray_from_num(ARRAY_TYPE num) {
  bitarray *b = (bitarray*) malloc(sizeof(bitarray));
  assert(b);

  b->array = (ARRAY_TYPE*) malloc(TYPE_SIZE);
  assert(b->array);

  b->array[0] = num;

  b->size = BITS_PER_EL;
  b->_array_size = 1;

  return b;
}

ARRAY_TYPE convert_bitarray_to_num(bitarray* bit_array) {
  assert(bit_array);
  assert(bit_array->size > 0 && bit_array->size <= BITS_PER_EL);
  return bit_array->array[0];
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

  for (size_t i = bit_array->size; i-- > 0;) {
    printf("%d", get_bit(bit_array, i));
  }
  printf("\n");
}

char* create_str_from_bitarray(bitarray *bit_array) {
  assert(bit_array);
  assert(bit_array->array);

  size_t size = bit_array->size + 1;
  char* str = (char*) malloc(size * sizeof(char));

  size_t i, j;
  for (j = 0, i = bit_array->size; i-- > 0; j++) {
    str[j] = get_bit(bit_array, i) ? '1' : '0';
  }

  str[j] = '\0';
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
