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

const char official_tvs_decoded[TEST_COUNT][TEST_SIZE]={
  "",
  "f",
  "fo",
  "foo",
  "foob",
  "fooba",
  "foobar"
};

const char official_tvs_b64_encoded[TEST_COUNT][TEST_SIZE] = {
  "",
  "Zg==",
  "Zm8=",
  "Zm9v",
  "Zm9vYg==",
  "Zm9vYmE=",
  "Zm9vYmFy"
};

const char official_tvs_b32_encoded[TEST_COUNT][TEST_SIZE] = {
  "",
  "MY======",
  "MZXQ====",
  "MZXW6===",
  "MZXW6YQ=",
  "MZXW6YTB",
  "MZXW6YTBOI======"
};


Test(test_base_suite, test_base32_encode) {
  char* output;
  for (int i = 0; i < TEST_COUNT; ++i) {
    output = encode_bytes_to_base32_string(
      (uint8_t*)official_tvs_decoded[i], strlen(official_tvs_decoded[i])
    );
    cr_expect(eq(str, output, (char*)official_tvs_b32_encoded[i]));
    free(output);
  }
}

Test(test_base_suite, test_base32_decode_valid) {
  uint8_t* output;
  int64_t output_len = -1;
  for (int i = 0; i < TEST_COUNT; ++i) {
    output = decode_base32_string_to_bytes(official_tvs_b32_encoded[i], &output_len);
    for (int64_t j = 0; j < output_len; ++j) {
      cr_expect(eq(u8, official_tvs_decoded[i][j], output[j]));
    }
    free(output);
  }
}

Test(test_base_suite, test_base64_encode_official) {
  char* output;
  for (int i = 0; i < TEST_COUNT; ++i) {
    output = encode_bytes_to_base64_string(
      (uint8_t*)official_tvs_decoded[i], strlen(official_tvs_decoded[i]), 0
    );
    cr_expect(eq(str, output, (char*)official_tvs_b64_encoded[i]));
    free(output);
  }
}

Test(test_base_suite, test_base64_encode_official_wrapped) {
  const char official_tvs_b64_encoded_wrapped[][TEST_SIZE] = {
    "",
    "Zg==",
    "Zm8=",
    "Zm9v\n",
    "Zm9v\nYg==",
    "Zm9v\nYmE=",
    "Zm9v\nYmFy\n"
  };
  char* output;
  for (size_t i = 0; i < sizeof(official_tvs_b64_encoded_wrapped)/sizeof(official_tvs_b64_encoded_wrapped[0]); ++i) {
    output = encode_bytes_to_base64_string((uint8_t*)official_tvs_decoded[i], strlen(official_tvs_decoded[i]), 4);
    cr_expect(eq(str, output, (char*)official_tvs_b64_encoded_wrapped[i]));
    free(output);
  }
}


Test(test_base_suite, test_base64_decode_official) {
  uint8_t* output;
  int64_t output_len = -1;
  for (int i = 0; i < TEST_COUNT; ++i) {
    output = decode_base64_string_to_bytes(official_tvs_b64_encoded[i], &output_len);
    cr_expect(output != NULL && output_len >= 0);
    cr_expect(eq(u32, output_len, strlen(official_tvs_decoded[i])));
    cr_expect(strncmp((char*)output, official_tvs_decoded[i], output_len) == 0);
    free(output);
  }
}

Test(test_base_suite, test_base64_google) {
  uint8_t* output;
  int64_t output_len = -1;
  // From Google: https://boringssl.googlesource.com/boringssl/+/master/crypto/base64/base64_test.cc
  char goog_tv_encoded[][128] = {
    "Zm9vYmFy\n\n",
    " Zm9vYmFy\n\n",
    " Z m 9 v Y m F y\n\n",
    "eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eA==\n",
    "eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eA\n==\n",
    "eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eA=\n=\n",
    "eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4\neHh4eHh4eHh4eHh4\n",
    "eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4\neHh4eHh4eHh4eHh4eHh4eA==\n",
    "eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh\n4eHh4eHh4eHh4eHh4eHh4eA==\n",
    "eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eA==\n"
  };
  char goog_decoded[][128] = {
    "foobar",
    "foobar",
    "foobar",
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
  };
  for (size_t i = 0; i < sizeof(goog_tv_encoded)/sizeof(goog_tv_encoded[0]); ++i) {
    output = decode_base64_string_to_bytes(goog_tv_encoded[i], &output_len);
    cr_expect(output != NULL && output_len > 0);
    cr_expect(eq(u32, output_len, strlen(goog_decoded[i])));
    cr_expect(strncmp((char*)output, goog_decoded[i], output_len) == 0);
    free(output);
  }

  char goog_tv_encoded_invalid[][128] = {
    "Zm9vYmFy=\n",
    "Zm9vYmFy==\n",
    "Z",
    "Z\n",
    "ab!c",
    "ab=c",
    "abc",
    "eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eA==\neHh4eHh4eHh4eHh4eHh4eHh4\n",
    "ZW5jb2RlIG1lCg==================================================================\n", // CVE-2015-0292
    "ZW5jb2RlIG1lCg===================================================================\n",
    "ZW5jb2RlIG1lCgCC====\n"
  };
  printf("===== Testing invalid input, some error messages are expected =====\n");
  for (size_t i = 0; i < sizeof(goog_tv_encoded_invalid)/sizeof(goog_tv_encoded_invalid[0]); ++i) {
    output = decode_base64_string_to_bytes(goog_tv_encoded_invalid[i], &output_len);
    cr_expect(output == NULL && output_len == -1);
  }
  printf("===== Invalid input tests end =====\n");
      
}
