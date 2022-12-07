#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "../../src/ripemd160.h"
#include "../../src/misc.h"


#define TEST_COUNT 8
#define TEST_SIZE 128

uint8_t message[TEST_COUNT][TEST_SIZE]={
  "",
  "a",
  "abc",
  "message digest",
  "abcdefghijklmnopqrstuvwxyz",
  "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
  "12345678901234567890123456789012345678901234567890123456789012345678901234567890"
};

char expected_hash[TEST_COUNT][TEST_SIZE] = {
  "9c1185a5c5e9fc54612808977ee8f548b2258d31",
  "0bdc9d2d256b3ee9daae347be6f4dc835a467ffe",
  "8eb208f7e05d987a9b044a8e98c6b087f15a0bfc",
  "5d0689ef49d2fae572b881b123a85ffa21595f36",
  "f71c27109c692c1b56bbdceb5b9d2865b3708dbc",
  "12a053384a9c0c88e405a06c27dcf49ada62eb2b",
  "b0e20b6e3116640286ed3a87a5713079b21f5189",
  "9b752e45573d4b39f4dbd3323cab82bf63326bfb"
};


Test(asserts, string) {
    uint8_t output_bytes[RIPEMD160_HASH_SIZE];
  char* output;
  for (int i = 0; i < TEST_COUNT; ++i) {
    cal_rpiemd160_hash(message[i], strlen((char*)message[i]), output_bytes);
    output = bytes_to_hex_string(output_bytes , RIPEMD160_HASH_SIZE, false);
    if (output == NULL) {
      cr_fail("bytes_to_hex_string() failed");
    }
    cr_expect(eq(str, output, expected_hash[i]));
    free(output);
  }
    
}