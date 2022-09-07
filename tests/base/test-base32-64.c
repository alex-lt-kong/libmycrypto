#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

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


bool test_base_encode(
  char* (*encode_function)(const uint8_t* , size_t), const char test_vectors_encoded[TEST_COUNT][TEST_SIZE]
) {
  bool passed = false;
  bool all_passed = true;
  char* output;
  for (int i = 0; i < TEST_COUNT; ++i) {

    output = (*encode_function)((uint8_t*)test_vectors_decoded[i], strlen(test_vectors_decoded[i]));
    printf("Function result: [%s]\n", output);

    passed = (strlen(output) == strlen(test_vectors_encoded[i]));
    passed = (strcmp(output, test_vectors_encoded[i]) == 0);
    all_passed &= passed;
    printf("Expected result: [%s",  test_vectors_encoded[i]);    
    printf("]\n%s\n\n", (passed ? "Passed" : "!!!FAILED!!!"));    
    free(output);
  }
  return all_passed;
}

bool test_base_decode(
  uint8_t* (*decode_function)(const char*, size_t*), const char test_vectors_encoded[TEST_COUNT][TEST_SIZE]
) {
  bool passed = false;
  bool all_passed = true;
  uint8_t* output;
  size_t output_len = -1;
  for (int i = 0; i < TEST_COUNT; ++i) {

    output = (*decode_function)(test_vectors_encoded[i], &output_len);
    printf("Function result: [");
    for (int j = 0; j < output_len; ++j) { printf("%c", output[j]); }
    printf("]\n");

    passed = (output_len == strlen(test_vectors_decoded[i]));
    all_passed &= passed;
    printf("Expected result: [%s", test_vectors_decoded[i]);
    for (int j = 0; j < strlen(test_vectors_decoded[i]); ++j) {
      if (passed) { passed = passed && (output[j] == test_vectors_decoded[i][j]); }
    }
    printf("]\n%s\n\n", passed ? "Passed" : "!!!FAILED!!!");
    free(output);
  }
  return all_passed;
}

int main() {
  freopen("README.md", "w", stdout); // seems we don't need to close() an freopen()'ed file.
  printf("```\n");
  time_t now;
  time(&now);
  char utc_time_str[sizeof "1970-01-01T00:00:00Z"];
  strftime(utc_time_str, sizeof(utc_time_str), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
  printf("Tests start at %s\n\n\n", utc_time_str);

  bool all_passed = true;
  printf("===== Testing Base32 ENcoding =====\n");
  all_passed &= test_base_encode(&encode_bytes_to_base32_string, test_vectors_base32_encoded);
  printf("\n\n===== Testing Base32 DEcoding =====\n");
  all_passed &= test_base_decode(&decode_base32_string_to_bytes, test_vectors_base32_encoded);
  printf("\n\n===== Testing Base64 Encoding =====\n");
  all_passed &= test_base_encode(&encode_bytes_to_base64_string, test_vectors_base64_encoded);
  printf("\n\n===== Testing Base64 DEcoding =====\n");
  all_passed &= test_base_decode(&decode_base64_string_to_bytes, test_vectors_base64_encoded);

  if (all_passed) {
    printf("\n\n========== ALL tests passed ==========\n"); 
  } else {
    printf("\n\n========== FAILED to pass some tests ==========\n"); 
  }
  printf("```\n");

  return 0;
}