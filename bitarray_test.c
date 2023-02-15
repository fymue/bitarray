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
  if (ans) printf("Test %d (count_bits == 0) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(b);

  b = create_set_bitarray(200);
  ans = !COUNT_EQUAL(b, 0, 200, 200);
  total_tests++;
  if (ans) printf("Test %d (count_bits == 200) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(b);

  b = create_bitarray(256);
  set_bit_range(b, 100, 200);
  set_bit_range(b, 15, 35);
  ans = !COUNT_EQUAL(b, 0, 256, 120);
  total_tests++;
  if (ans) printf("Test %d (count_bits == 120) failed.\n", total_tests);
  fail_c += ans;

  clear_bit_range(b, 100, 250);

  ans = !COUNT_EQUAL(b, 100, 250, 0);
  total_tests++;
  if (ans) printf("Test %d (count_bit_range == 0) failed.\n", total_tests);
  fail_c += ans;

  ans = !COUNT_EQUAL(b, 0, 256, 20);
  total_tests++;
  if (ans) printf("Test %d (count_bits == 20) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(b);

  b  =  create_bitarray_from_str("1001011010101101001010011010011"
                                 "0010100100010010100010101010010"
                                 "1110101110100101011010011010101"
                                 "1010101010110101010101101010101"
                                 "11101001101010101", 141);
  ans = !COUNT_EQUAL(b, 10, 130, 60);
  total_tests++;
  if (ans) printf("Test %d (count_bit_range == 60) failed.\n", total_tests);
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
  if (ans) printf("Test %d (set_bit) failed.\n", total_tests);
  fail_c += ans;
  ans = !BITS_EQUAL(b, b2, true);
  total_tests++;
  if (ans) printf("Test %d (set_bit (2)) failed.\n", total_tests);
  fail_c += ans;

  // check if equal_bits function can detect unequal bitarrays
  clear_bit(b2, 2);
  clear_bit(b2, 6);
  ans = BITS_EQUAL(b, b2, false);
  total_tests++;
  if (ans) printf("Test %d (clear_bit) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(b2);

  b2 = copy_bitarray(b);
  ans = !BITS_EQUAL(b, b2, true);
  total_tests++;
  if (ans) printf("Test %d (copy_bitarray) failed.\n", total_tests);
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
  if (ans) printf("Test %d (and_bits) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(res);
  delete_bitarray(ref);

  // check OR
  res = or_bits(b, b2);
  ref = create_bitarray_from_str("101101101", 9);
  ans = !BITS_EQUAL(res, ref, false);
  total_tests++;
  if (ans) printf("Test %d (or_bits) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(res);
  delete_bitarray(ref);

  // check XOR
  ref = create_bitarray_from_str("001101001", 9);
  res = xor_bits(b, b2);
  ans = !BITS_EQUAL(res, ref, false);
  total_tests++;
  if (ans) printf("Test %d (xor_bits) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(res);
  delete_bitarray(ref);

  // check NOT
  res = not_bits(b);
  ref = create_bitarray_from_str("011010010", 9);
  ans = !BITS_EQUAL(res, ref, false);
  total_tests++;
  if (ans) printf("Test %d (not_bits) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(res);
  delete_bitarray(ref);

  delete_bitarray(b);
  delete_bitarray(b2);

  // check AND inplace
  b  =  create_bitarray_from_str("1001011010101", 13);
  b2 =  create_bitarray_from_str("1010001001100", 13);
  ref = create_bitarray_from_str("1000001000100", 13);
  and_bits_inplace(b, b2);
  ans = !BITS_EQUAL(b, ref, false);
  total_tests++;
  if (ans) printf("Test %d (and_bits_inplace) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(ref);
  delete_bitarray(b);
  delete_bitarray(b2);

  // check OR inplace
  b  =  create_bitarray_from_str("1001011010101", 13);
  b2 =  create_bitarray_from_str("1010001001100", 13);
  ref = create_bitarray_from_str("1011011011101", 13);
  or_bits_inplace(b, b2);
  ans = !BITS_EQUAL(b, ref, false);
  total_tests++;
  if (ans) printf("Test %d (or_bits_inplace) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(ref);
  delete_bitarray(b);
  delete_bitarray(b2);

  // check XOR inplace
  b  =  create_bitarray_from_str("1001011010101", 13);
  b2 =  create_bitarray_from_str("1010001001100", 13);
  ref = create_bitarray_from_str("0011010011001", 13);
  xor_bits_inplace(b, b2);
  ans = !BITS_EQUAL(b, ref, false);
  total_tests++;
  if (ans) printf("Test %d (xor_bits_inplace) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(ref);
  delete_bitarray(b);
  delete_bitarray(b2);

  // check and inplace
  b  =  create_bitarray_from_str("1001011010101101001010011010011"
                                 "0010100100010010100010101010010"
                                 "1110101110100101011010011010101"
                                 "1010101010110101010101101010101"
                                 "11101001101010101", 141);
  b2 =  create_bitarray_from_str("1010001001100001010010100101010"
                                 "0001001100100100100101010010110"
                                 "1110011010010100101010100101110"
                                 "1010010101001011100101001101010"
                                 "01001011010101100", 141);
  ref = create_bitarray_from_str("1000001000100001000010000000010"
                                 "0000000100000000100000000010010"
                                 "1110001010000100001010000000100"
                                 "1010000000000001000101001000000"
                                 "01001001000000100", 141);
  and_bits_inplace(b, b2);
  ans = !BITS_EQUAL(b, ref, false);
  total_tests++;
  if (ans) printf("Test %d (and_bits_inplace 2) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(ref);
  delete_bitarray(b);
  delete_bitarray(b2);

  // check or inplace
  b  =  create_bitarray_from_str("1001011010101101001010011010011"
                                 "0010100100010010100010101010010"
                                 "1110101110100101011010011010101"
                                 "1010101010110101010101101010101"
                                 "11101001101010101", 141);
  b2 =  create_bitarray_from_str("1010001001100001010010100101010"
                                 "0001001100100100100101010010110"
                                 "1110011010010100101010100101110"
                                 "1010010101001011100101001101010"
                                 "01001011010101100", 141);
  ref = create_bitarray_from_str("1011011011101101011010111111011"
                                 "0011101100110110100111111010110"
                                 "1110111110110101111010111111111"
                                 "1010111111111111110101101111111"
                                 "11101011111111101", 141);
  or_bits_inplace(b, b2);
  ans = !BITS_EQUAL(b, ref, false);
  total_tests++;
  if (ans) printf("Test %d (or_bits_inplace 2) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(ref);
  delete_bitarray(b);
  delete_bitarray(b2);

  // check xor inplace
  b  =  create_bitarray_from_str("1001011010101101001010011010011"
                                 "0010100100010010100010101010010"
                                 "1110101110100101011010011010101"
                                 "1010101010110101010101101010101"
                                 "11101001101010101", 141);
  b2 =  create_bitarray_from_str("1010001001100001010010100101010"
                                 "0001001100100100100101010010110"
                                 "1110011010010100101010100101110"
                                 "1010010101001011100101001101010"
                                 "01001011010101100", 141);
  ref = create_bitarray_from_str("0011010011001100011000111111001"
                                 "0011101000110110000111111000100"
                                 "0000110100110001110000111111011"
                                 "0000111111111110110000100111111"
                                 "10100010111111001", 141);
  xor_bits_inplace(b, b2);
  ans = !BITS_EQUAL(b, ref, true);
  total_tests++;
  if (ans) printf("Test %d (xor_bits_inplace 2) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(ref);
  delete_bitarray(b);
  delete_bitarray(b2);

  b =   create_bitarray_from_str("1010100001011010110101", 22);
  ref = create_bitarray_from_str("1101011110011010110101", 22);
  flip_bit_range(b, 12, 21);
  ans = !BITS_EQUAL(b, ref, true);
  total_tests++;
  fail_c += ans;
  delete_bitarray(b);
  delete_bitarray(ref);

  b = create_bitarray_from_num(123456);
  ref = create_bitarray_from_str("11110001001000000", 17);

  // not very clean, but sizes need to match for equal comparison
  // (at least in debug mode),
  // so increase the size of the ref bitarray to match the size
  ref->size = BITS_PER_EL;
  ans = !BITS_EQUAL(b, ref, false);
  total_tests++;
  if (ans) printf("Test %d (create_bitarray_from_num) failed.\n", total_tests);
  fail_c += ans;

  ARRAY_TYPE num = convert_bitarray_to_num(ref);
  delete_bitarray(ref);
  delete_bitarray(b);
  ans = num == 123456;
  total_tests++;
  if (!ans) {
    printf("Test %d (convert_bitarray_to_num) failed.\n", total_tests);
    fail_c += ans;
  }

  b = create_bitarray_from_num(9848293402894825);
  ref = create_bitarray_from_str(
    "100010111111001111100001111101111011000110000111101001", 54);
  ref->size = BITS_PER_EL;
  ans = !BITS_EQUAL(b, ref, false);
  total_tests++;
  if (ans) printf("Test %d (create_bitarray_from_num) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(ref);
  delete_bitarray(b);

  b =              create_bitarray_from_str("100101010101110101", 18);
  b2 = create_bitarray_from_str("011110101011100101010101110101", 30);
  append_bit_range(b2, b, 18, 30);
  ans = !BITS_EQUAL(b, b2, false);
  total_tests++;
  if (ans) printf("Test %d (append_bit_range) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(b);
  delete_bitarray(b2);

  b =                    create_bitarray_from_str("100101010101110101", 18);
  b2 = create_bitarray_from_str("100101010101110101100101010101110101", 36);
  append_bit_range(b, b, 0, 18);
  ans = !BITS_EQUAL(b, b2, false);
  total_tests++;
  if (ans) printf("Test %d (append_bit_range) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(b);
  delete_bitarray(b2);

  b =  create_bitarray_from_str("100101010101110101", 18);
  b2 = create_bitarray(1);
  copy_all_bits(b, b2);
  ans = !BITS_EQUAL(b, b2, false);
  total_tests++;
  if (ans) printf("Test %d (copy_all_bits) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(b);
  delete_bitarray(b2);

  // left shift inplace
  b =   create_bitarray_from_str("0100101010010001010011101", 25);
  ref = create_bitarray_from_str("1001000101001110100000000", 25);
  left_shift_bits_inplace(b, 8);
  ans = !BITS_EQUAL(b, ref, false);
  total_tests++;
  if (ans) printf("Test %d (left_shift_bits_inplace) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(b);
  delete_bitarray(ref);

  // right shift inplace
  b =   create_bitarray_from_str("0100101010010001010011101", 25);
  ref = create_bitarray_from_str("0000000000010010101001000", 25);
  right_shift_bits_inplace(b, 10);
  ans = !BITS_EQUAL(b, ref, false);
  total_tests++;
  if (ans) printf("Test %d (right_shift_bits_inplace) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(b);
  delete_bitarray(ref);

  // left shift
  b =   create_bitarray_from_str("10101010010010100110", 20);
  ref = create_bitarray_from_str("10010010100110000000", 20);
  res = left_shift_bits(b, 6);
  ans = !BITS_EQUAL(res, ref, false);
  total_tests++;
  if (ans) printf("Test %d (left_shift_bits) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(b);
  delete_bitarray(ref);
  delete_bitarray(res);

  // right shift
  b =   create_bitarray_from_str("10101010010010100110", 20);
  ref = create_bitarray_from_str("00000101010100100101", 20);
  res = right_shift_bits(b, 5);
  ans = !BITS_EQUAL(res, ref, false);
  total_tests++;
  if (ans) printf("Test %d (right_shift_bits) failed.\n", total_tests);
  fail_c += ans;
  delete_bitarray(b);
  delete_bitarray(ref);
  delete_bitarray(res);

  printf("All tests have been executed. %d/%d tests failed, "
         "%d/%d tests passed.\n",
         fail_c, total_tests, total_tests - fail_c, total_tests);

  if (fail_c) {
    return 1;
  } else {
    return 0;
  }
}


int main() {
  int res = execute_tests();
  return res;
}
