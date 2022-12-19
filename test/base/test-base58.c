#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "../../src/base58.h"

#define TEST_COUNT 3
#define TEST_SIZE 128

const size_t test_vectors_decoded_len[TEST_COUNT]={12, 44, 6};

const uint8_t test_vectors_decoded[TEST_COUNT][TEST_SIZE]={
  "Hello World!",
  "The quick brown fox jumps over the lazy dog.",
  {0x00, 0x00, 0x28, 0x7f, 0xb4, 0xcd}
};

const char test_vectors_encoded[TEST_COUNT][TEST_SIZE] = {
  "2NEpo7TZRRrLZSi2U",
  "USm3fpXnKG5EUBx2ndxBDMPVciP5hGey2Jh4NDv6gmeo1LkMeiKrLJUUBk6Z",
  "11233QC4"
};

Test(test_base_suite, test_base58) {
  char output[1024] = {0};
  for (int i = 0; i < TEST_COUNT; ++i) {    
    size_t output_len = 512;
    bool res = b58enc(test_vectors_decoded[i], test_vectors_decoded_len[i], output, &output_len);   
    cr_assert(res);
    cr_expect(eq(str, output, (char*)test_vectors_encoded[i]));  
  }
}
