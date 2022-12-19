#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "../../src/base64.h"
#include "../../src/base32.h"

#define TEST_COUNT 7
#define TEST_SIZE 32

const char test_vectors_decoded[TEST_COUNT][TEST_SIZE]={
  "",
  "f",
  "fo",
  "foo",
  "foob",
  "fooba",
  "foobar"
};

const char test_vectors_base64_encoded[TEST_COUNT][TEST_SIZE] = {
  "",
  "Zg==",
  "Zm8=",
  "Zm9v",
  "Zm9vYg==",
  "Zm9vYmE=",
  "Zm9vYmFy"
};

const char test_vectors_base32_encoded[TEST_COUNT][TEST_SIZE] = {
  "",
  "MY======",
  "MZXQ====",
  "MZXW6===",
  "MZXW6YQ=",
  "MZXW6YTB",
  "MZXW6YTBOI======"
};


void test_base_encode(
  char* (*encode_function)(const uint8_t* , size_t), const char test_vectors_encoded[TEST_COUNT][TEST_SIZE]
) {
  char* output;
  for (int i = 0; i < TEST_COUNT; ++i) {
    output = (*encode_function)((uint8_t*)test_vectors_decoded[i], strlen(test_vectors_decoded[i]));
    cr_expect(eq(str, output, (char*)test_vectors_encoded[i]));
    free(output);
  }
}

void test_base_decode(
  uint8_t* (*decode_function)(const char*, size_t*), const char test_vectors_encoded[TEST_COUNT][TEST_SIZE]
) {
    uint8_t* output;
  size_t output_len = -1;
  for (int i = 0; i < TEST_COUNT; ++i) {
    output = (*decode_function)(test_vectors_encoded[i], &output_len);
    for (size_t j = 0; j < output_len; ++j) {
      cr_expect(eq(u8, test_vectors_decoded[i][j], output[j]));
    }
    free(output);
  }
}


Test(test_base_suite, test_base32) {
  test_base_encode(&encode_bytes_to_base32_string, test_vectors_base32_encoded);
  test_base_decode(&decode_base32_string_to_bytes, test_vectors_base32_encoded);
}


Test(test_base_suite, test_base64) {
  test_base_encode(&encode_bytes_to_base64_string, test_vectors_base64_encoded);
  test_base_decode(&decode_base64_string_to_bytes, test_vectors_base64_encoded);
}
