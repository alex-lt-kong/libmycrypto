#include <iostream>
#include <string.h>
#include "../../base64.h"

using namespace std;
#define TEST_COUNT 7

char test_vectors_decoded[TEST_COUNT][10]={
  "",
  "f",
  "fo",
  "foo",
  "foob",
  "fooba",
  "foobar"
};
char test_vectors_encoded[TEST_COUNT][10] = {
  "",
  "Zg==",
  "Zm8=",
  "Zm9v",
  "Zm9vYg==",
  "Zm9vYmE=",
  "Zm9vYmFy"
};

void test_base64_encode() {
  bool passed = false;
  char* output;
  for (int i = 0; i < TEST_COUNT; ++i) {
    output = encode_bytes_to_base64_string((unsigned char*)test_vectors_decoded[i], strlen(test_vectors_decoded[i]));
    cout << "Function result: [";
    for (int j = 0; j < strlen(output); ++j) {
      cout << output[j];
    }
    cout << "]\n";

    passed = (strlen(output) == strlen(test_vectors_encoded[i]));

    cout << "Expected result: [";
    for (int j = 0; j < strlen(test_vectors_encoded[i]); ++j) {
      cout << test_vectors_encoded[i][j];
      if (passed) {
        passed = passed && (output[j] == test_vectors_encoded[i][j]);
      }
    }
    cout << "]\n" << (passed ? "Passed" : "!!!FAILED!!!") << "\n" << endl;
  }
}

int main() {
  test_base64_encode();
  return 0;
}
