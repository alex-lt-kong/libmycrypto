#include <iostream>
#include <string.h>
#include "../../base64.h"
#include "../../base32.h"

using namespace std;
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
  char* (*encode_function)(const unsigned char* , size_t), const char test_vectors_encoded[TEST_COUNT][TEST_SIZE]
) {
  bool passed = false;
  char* output;
  for (int i = 0; i < TEST_COUNT; ++i) {

    output = (*encode_function)((unsigned char*)test_vectors_decoded[i], strlen(test_vectors_decoded[i]));
    cout << "Function result: [";
    for (int j = 0; j < strlen(output); ++j) { cout << output[j]; }
    cout << "]\n";

    passed = (strlen(output) == strlen(test_vectors_encoded[i]));

    cout << "Expected result: [";
    for (int j = 0; j < strlen(test_vectors_encoded[i]); ++j) {
      cout << test_vectors_encoded[i][j];
      if (passed) { passed = passed && (output[j] == test_vectors_encoded[i][j]); }
    }
    cout << "]\n" << (passed ? "Passed" : "!!!FAILED!!!") << "\n" << endl;
  }
}

void test_base_decode(
  unsigned char* (*decode_function)(const char*, size_t*), const char test_vectors_encoded[TEST_COUNT][TEST_SIZE]
) {
  bool passed = false;
  unsigned char* output;
  size_t output_len = -1;
  for (int i = 0; i < TEST_COUNT; ++i) {

    output = (*decode_function)(test_vectors_encoded[i], &output_len);
    cout << "Function result: [";
    for (int j = 0; j < output_len; ++j) { cout << output[j]; }
    cout << "]\n";

    passed = (output_len == strlen(test_vectors_decoded[i]));

    cout << "Expected result: [";
    for (int j = 0; j < strlen(test_vectors_decoded[i]); ++j) {
      cout << test_vectors_decoded[i][j];
      if (passed) { passed = passed && (output[j] == test_vectors_decoded[i][j]); }
    }
    cout << "]\n" << (passed ? "Passed" : "!!!FAILED!!!") << "\n" << endl;
  }
}

int main() {
  cout << "    ===== Testing Base32 ENcoding =====\n";
  test_base_encode(&encode_bytes_to_base32_string, test_vectors_base32_encoded);
  cout << "\n\n===== Testing Base32 DEcoding =====\n";
  test_base_decode(&decode_base32_string_to_bytes, test_vectors_base32_encoded);
  cout << "\n\n===== Testing Base64 Encoding =====\n";
  test_base_encode(&encode_bytes_to_base64_string, test_vectors_base64_encoded);
  cout << "\n\n===== Testing Base64 DEcoding =====\n";
  test_base_decode(&decode_base64_string_to_bytes, test_vectors_base64_encoded);
  return 0;
}