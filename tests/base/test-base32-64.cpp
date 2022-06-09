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


bool test_base_encode(
  char* (*encode_function)(const unsigned char* , size_t), const char test_vectors_encoded[TEST_COUNT][TEST_SIZE]
) {
  bool passed = false;
  bool all_passed = true;
  char* output;
  for (int i = 0; i < TEST_COUNT; ++i) {

    output = (*encode_function)((unsigned char*)test_vectors_decoded[i], strlen(test_vectors_decoded[i]));
    cout << "Function result: [";
    for (int j = 0; j < strlen(output); ++j) { cout << output[j]; }
    cout << "]\n";

    passed = (strlen(output) == strlen(test_vectors_encoded[i]));
    all_passed &= passed;
    cout << "Expected result: [";
    for (int j = 0; j < strlen(test_vectors_encoded[i]); ++j) {
      cout << test_vectors_encoded[i][j];
      if (passed) { passed = passed && (output[j] == test_vectors_encoded[i][j]); }
    }
    cout << "]\n" << (passed ? "Passed" : "!!!FAILED!!!") << "\n" << endl;
  }
  return all_passed;
}

bool test_base_decode(
  unsigned char* (*decode_function)(const char*, size_t*), const char test_vectors_encoded[TEST_COUNT][TEST_SIZE]
) {
  bool passed = false;
  bool all_passed = true;
  unsigned char* output;
  size_t output_len = -1;
  for (int i = 0; i < TEST_COUNT; ++i) {

    output = (*decode_function)(test_vectors_encoded[i], &output_len);
    cout << "Function result: [";
    for (int j = 0; j < output_len; ++j) { cout << output[j]; }
    cout << "]\n";

    passed = (output_len == strlen(test_vectors_decoded[i]));
    all_passed &= passed;
    cout << "Expected result: [";
    for (int j = 0; j < strlen(test_vectors_decoded[i]); ++j) {
      cout << test_vectors_decoded[i][j];
      if (passed) { passed = passed && (output[j] == test_vectors_decoded[i][j]); }
    }
    cout << "]\n" << (passed ? "Passed" : "!!!FAILED!!!") << "\n" << endl;
  }
  return all_passed;
}

int main() {
  freopen("README.md", "w", stdout); // seems we don't need to close() an freopen()'ed file.
  cout << "```\n";
  time_t now;
  time(&now);
  char utc_time_str[sizeof "2011-10-08T07:07:09Z"];
  strftime(utc_time_str, sizeof(utc_time_str), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
  cout << "Tests start at " << utc_time_str << "\n\n" << endl;

  bool all_passed = true;
  cout << "===== Testing Base32 ENcoding =====\n";
  all_passed &= test_base_encode(&encode_bytes_to_base32_string, test_vectors_base32_encoded);
  cout << "\n\n===== Testing Base32 DEcoding =====\n";
  all_passed &= test_base_decode(&decode_base32_string_to_bytes, test_vectors_base32_encoded);
  cout << "\n\n===== Testing Base64 Encoding =====\n";
  all_passed &= test_base_encode(&encode_bytes_to_base64_string, test_vectors_base64_encoded);
  cout << "\n\n===== Testing Base64 DEcoding =====\n";
  all_passed &= test_base_decode(&decode_base64_string_to_bytes, test_vectors_base64_encoded);

  if (all_passed) {
    cout << "\n\n========== ALL tests passed ==========" << endl; 
  } else {
    cout << "\n\n========== FAILED to pass some tests ==========" << endl; 
  }
  cout << "```\n";

  return 0;
}