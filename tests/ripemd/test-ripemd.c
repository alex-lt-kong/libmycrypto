#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>

#include "../../rmd160.h"

#define TEST_COUNT 8
#define TEST_SIZE 128

byte message[TEST_COUNT][TEST_SIZE]={
  "",
  "a",
  "abc",
  "message digest",
  "abcdefghijklmnopqrstuvwxyz",
  "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
  "12345678901234567890123456789012345678901234567890123456789012345678901234567890"
};

const char expected_hash[TEST_COUNT][TEST_SIZE] = {
  "9c1185a5c5e9fc54612808977ee8f548b2258d31",
  "0bdc9d2d256b3ee9daae347be6f4dc835a467ffe",
  "8eb208f7e05d987a9b044a8e98c6b087f15a0bfc",
  "5d0689ef49d2fae572b881b123a85ffa21595f36",
  "f71c27109c692c1b56bbdceb5b9d2865b3708dbc",
  "12a053384a9c0c88e405a06c27dcf49ada62eb2b",
  "b0e20b6e3116640286ed3a87a5713079b21f5189",
  "9b752e45573d4b39f4dbd3323cab82bf63326bfb"
};


bool test_ripemd160() {
  bool passed = false;
  bool all_passed = true;
  byte* output;
  for (int i = 0; i < TEST_COUNT; ++i) {

    output = cal_rpiemd160_hash(message[i], (dword)strlen((char*)message[i]));
    printf("Function result: [");
    for (int j=0; j<RMDsize/8; ++j) { printf("%02x", output[j]); }
    printf("]\n");

    passed = (strlen((char*)output) == strlen(expected_hash[i]));
    all_passed &= passed;
    printf("Expected result: [%s]\n", expected_hash[i]);
  }
  return all_passed;
}

int main() {
  //freopen("README.md", "w", stdout); // seems we don't need to close() an freopen()'ed file.
  printf("```\n");
  time_t now;
  time(&now);
  char utc_time_str[sizeof "1970-01-01T00:00:00Z"];
  strftime(utc_time_str, sizeof(utc_time_str), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
  printf("Tests start at %s\n\n", utc_time_str);
  bool all_passed;
  all_passed &= test_ripemd160();
  if (all_passed) {
    printf("\n\n========== ALL tests passed ==========\n"); 
  } else {
    printf("\n\n========== FAILED to pass some tests =========="); 
  }
  printf("```\n");

  return 0;
}