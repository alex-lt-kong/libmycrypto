#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "mycrypto/misc.h"
#include "mycrypto/ripemd160.h"

#define TEST_COUNT 8
#define TEST_SIZE 128

unsigned char message[TEST_COUNT][TEST_SIZE] = {
    "",
    "a",
    "abc",
    "message digest",
    "abcdefghijklmnopqrstuvwxyz",
    "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
    "12345678901234567890123456789012345678901234567890123456789012345678901234"
    "567890"};

char expected_hash[TEST_COUNT][TEST_SIZE] = {
    "9c1185a5c5e9fc54612808977ee8f548b2258d31",
    "0bdc9d2d256b3ee9daae347be6f4dc835a467ffe",
    "8eb208f7e05d987a9b044a8e98c6b087f15a0bfc",
    "5d0689ef49d2fae572b881b123a85ffa21595f36",
    "f71c27109c692c1b56bbdceb5b9d2865b3708dbc",
    "12a053384a9c0c88e405a06c27dcf49ada62eb2b",
    "b0e20b6e3116640286ed3a87a5713079b21f5189",
    "9b752e45573d4b39f4dbd3323cab82bf63326bfb"};

int main() {
  int retval = 0;
  unsigned char output_bytes[RIPEMD160_HASH_SIZE];
  char *output;
  for (int i = 0; i < TEST_COUNT; ++i) {
    cal_ripemd160_hash(message[i], strlen((char *)message[i]), output_bytes);
    output = bytes_to_hex_string(output_bytes, RIPEMD160_HASH_SIZE, false);
    if (output == NULL) {
      return 1;
    }
    if (strcmp(output, expected_hash[i]) != 0) {
      fprintf(stderr, "FAILED\nExpect: %s\nActual: %s\n", expected_hash[i],
              output);
      ++retval;
    }
    free(output);
  }
  if (retval != 0) {
    fprintf(stderr, "===== %d test(s) FAILED!!! =====\n", retval);
  } else {
    printf("All tests passed\n");
  }
  return retval;
}
