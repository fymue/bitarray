#include "bitarray.h"

static bool COUNT_EQUAL(bitarray *b, size_t from, size_t to, size_t ans) {
  if (from == 0 && to == b->size) {
    return count_bits(b) == ans;
  } else {
    return count_bit_range(b, from, to) == ans;
  }
}

static bool BITS_EQUAL(bitarray *b1, bitarray *b2, bool cmp_str) {
  bool ans;
  if (cmp_str) {
    char *b_1 = create_str_from_bitarray(b1);
    char *b_2 = create_str_from_bitarray(b2);
    ans = strncmp(b_1, b_2, b1->size) == 0;
    free(b_1);
    free(b_2);
  } else {
    ans =  equal_bits(b1, b2);
  }

  return ans;
}

int execute_tests() {
  int total_tests = 0;
  int fail_c = 0;
  bitarray *b, *b2, *res, *ref;
  bool ans;

  b = create_bitarray(10);
  ans = !COUNT_EQUAL(b, 0, 10, 0);
  total_tests++;
  if (ans) printf("Test %d (Count == 0) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(b);

  b = create_set_bitarray(200);
  ans = !COUNT_EQUAL(b, 0, 200, 200);
  total_tests++;
  if (ans) printf("Test %d (Count == 200) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(b);

  b = create_bitarray(256);
  set_bit_range(b, 100, 200);
  set_bit_range(b, 15, 35);
  ans = !COUNT_EQUAL(b, 0, 256, 120);
  total_tests++;
  if (ans) printf("Test %d (Count == 120) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(b);

  b = create_bitarray_from_str("101010101", 9);
  b2 = create_bitarray(9);
  set_bit(b2, 0);
  set_bit(b2, 2);
  set_bit(b2, 4);
  set_bit(b2, 6);
  set_bit(b2, 8);
  ans = !BITS_EQUAL(b, b2, false);
  total_tests++;
  if (ans) printf("Test %d (Equal bits) failed.\n", total_tests);
  fail_c += ans;
  ans = !BITS_EQUAL(b, b2, true);
  total_tests++;
  if (ans) printf("Test %d (Equal bits) failed.\n", total_tests);
  fail_c += ans;

  // check if equal_bits function can detect unequal bitarrays
  clear_bit(b2, 2);
  clear_bit(b2, 6);
  ans = BITS_EQUAL(b, b2, false);
  total_tests++;
  if (ans) printf("Test %d (Unequal bits) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(b2);

  b2 = copy_bitarray(b);
  ans = !BITS_EQUAL(b, b2, true);
  total_tests++;
  if (ans) printf("Test %d (copy bitarray) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(b2);
  delete_bitarray(b);

  // check AND
  b  = create_bitarray_from_str("100101101", 9);
  b2 = create_bitarray_from_str("101000100", 9);
  ref = create_bitarray_from_str("100000100", 9);
  res = and_bits(b, b2);
  ans = !BITS_EQUAL(res, ref, false);
  total_tests++;
  if (ans) printf("Test %d (AND) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(res);
  delete_bitarray(ref);

  // check OR
  res = or_bits(b, b2);
  ref = create_bitarray_from_str("101101101", 9);
  ans = !BITS_EQUAL(res, ref, false);
  total_tests++;
  if (ans) printf("Test %d (OR) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(res);
  delete_bitarray(ref);

  // check XOR
  ref = create_bitarray_from_str("001101001", 9);
  res = xor_bits(b, b2);
  ans = !BITS_EQUAL(res, ref, false);
  total_tests++;
  if (ans) printf("Test %d (XOR) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(res);
  delete_bitarray(ref);

  // check NOT
  res = not_bits(b);
  ref = create_bitarray_from_str("011010010", 9);
  ans = !BITS_EQUAL(res, ref, false);
  total_tests++;
  if (ans) printf("Test %d (NOT) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(res);
  delete_bitarray(ref);

  delete_bitarray(b);
  delete_bitarray(b2);

  b = create_bitarray_from_num(123456);
  ref = create_bitarray_from_str("11110001001000000", 17);

  // not very clean, but sizes need to match for equal comparison
  // (at least in debug mode),
  // so increase the size of the ref bitarray to match the size
  ref->size = BITS_PER_EL;
  ans = !BITS_EQUAL(b, ref, false);
  total_tests++;
  if (ans) printf("Test %d (bitarray from num) failed.\n", total_tests);
  fail_c += ans;

  ARRAY_TYPE num = convert_bitarray_to_num(ref);
  delete_bitarray(ref);
  delete_bitarray(b);
  ans = num == 123456;
  total_tests++;
  if (!ans) {
    printf("Test %d (bitarray to num) failed.\n", total_tests);
    fail_c += ans;
  }

  b = create_bitarray_from_num(9848293402894825);
  ref = create_bitarray_from_str(
    "100010111111001111100001111101111011000110000111101001", 54);

  ref->size = BITS_PER_EL;
  ans = !BITS_EQUAL(b, ref, false);
  total_tests++;
  if (ans) printf("Test %d (bitarray from num) failed.\n", total_tests);
  fail_c += ans;

  b =              create_bitarray_from_str("100101010101110101", 18);
  b2 = create_bitarray_from_str("011110101011100101010101110101", 30);
  append_bit_range(b2, b, 18, 30);
  ans = !BITS_EQUAL(b, b2, false);
  total_tests++;
  if (ans) printf("Test %d (append bit range) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(b);
  delete_bitarray(b2);

  printf("All tests have been executed. %d/%d tests failed, "
         "%d/%d tests passed.\n",
         fail_c, total_tests, total_tests - fail_c, total_tests);

  if (fail_c) {
    return 1;
  } else {
    return 0;
  }
}


int main(int argc, char **argv) {
  int res = execute_tests();
  return res;
}
