#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "../../src/hmac.h"
#include "../../src/sha1.h"
#include "../../src/sha256.h"
#include "../../src/misc.h"

#define TEST_COUNT 7
#define TEST_SIZE 512
#define MAX_PRINT_LEN 64

const char tv_sha1_keys[TEST_COUNT][TEST_SIZE] = {
  "0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b",
  "4a656665", // Jefe
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
  "0102030405060708090a0b0c0d0e0f10111213141516171819",
  "0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c",
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
};

const char tv_sha1_data[TEST_COUNT][TEST_SIZE] = {
  "4869205468657265",  // Hi There
  "7768617420646f2079612077616e7420666f72206e6f7468696e673f", // what do ya want for nothing?
  "dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd",
  "cdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcd",
  "546573742057697468205472756e636174696f6e",//Test With Truncation
  "54657374205573696e67204c6172676572205468616e20426c6f636b2d53697a65204b6579202d2048617368204b6579204669727374", // Test Using Larger Than Block-Size Key - Hash Key First
  "54657374205573696E67204C6172676572205468616E20426C6F636B2D53697A65204B657920616E64204C6172676572205468616E204F6E6520426C6F636B2D53697A652044617461"
  // Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data
};

const char tv_sha1_hash[TEST_COUNT][TEST_SIZE] = {
  "b617318655057264e28bc0b6fb378c8ef146be00",
  "effcdf6ae5eb2fa2d27416d5f184df9c259a7c79",
  "125d7342b9ac11cd91a39af48aa17b4f63f175d3",
  "4c9007f4026250c6bc8414f9bf50c86c2d7235da",
  "4c1a03424b55e07fe7f27be1d58bb9324a9a5a04",
  "aa4ae5e15272d00e95705637ce8a3b55ed402112",
  "e8e99d0f45237d786d6bbaa7965c7808bbff1a91"
};


const char tv_sha256_keys[TEST_COUNT][TEST_SIZE] = {
  "0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b",
  "4a656665",
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
  "0102030405060708090a0b0c0d0e0f10111213141516171819",
  "0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c",
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
};

const char tv_sha256_data[TEST_COUNT][TEST_SIZE] = {
  "4869205468657265",
  "7768617420646f2079612077616e7420666f72206e6f7468696e673f",
  "dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd",
  "cdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcd",
  "546573742057697468205472756e636174696f6e",//Test With Truncation
  "54657374205573696e67204c6172676572205468616e20426c6f636b2d53697a65204b6579202d2048617368204b6579204669727374",
  "5468697320697320612074657374207573696e672061206c6172676572207468616e20626c6f636b2d73697a65206b657920616e642061206c6172676572207468616e20626c6f636b2d73697a6520646174612e20546865206b6579206e6565647320746f20626520686173686564206265666f7265206265696e6720757365642062792074686520484d414320616c676f726974686d2e"
};

const char tv_sha256_hash[TEST_COUNT][TEST_SIZE] = {
  "b0344c61d8db38535ca8afceaf0bf12b881dc200c9833da726e9376c2e32cff7",
  "5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843",
  "773ea91e36800e46854db8ebd09181a72959098b3ef8c122d9635514ced565fe",
  "82558a389a443c0ea4cc819899f2083a85f0faa3e578f8077a2e3ff46729665b",
  "a3b6167473100ee06e0c796c2955552b",
  "60e431591ee0b67f0d8a26aacbf5b77f8e0bc6213728c5140546040f0ee37f54",
  "9b09ffa71b942fcb27635fbcd5b0e944bfdc63644f0713938a7f51535c3a35e2"
};

bool test_hmac(
  void (*hash_function)(const uint8_t*, size_t, const uint8_t*, size_t, uint8_t*),
  const size_t hash_size,
  const char tv_keys[TEST_COUNT][TEST_SIZE],
  const char tv_data[TEST_COUNT][TEST_SIZE],
  const char tv_hash[TEST_COUNT][TEST_SIZE]
) {
  uint8_t* key_bytes = NULL;
  uint8_t* data_bytes = NULL;
  uint8_t hash[hash_size];
  char* hash_hex = NULL;
  bool all_passed = true;
  size_t key_len, data_len;
  for (int i = 0; i < TEST_COUNT; ++i) {    
    key_bytes = hex_string_to_bytes(tv_keys[i], &key_len);
    data_bytes = hex_string_to_bytes(tv_data[i], &data_len);
    hash_function(key_bytes, key_len, data_bytes, data_len, hash);
    hash_hex = bytes_to_hex_string(hash, hash_size, false);
    if (strlen(tv_hash[i]) < strlen(hash_hex)) { // i.e., test vector truncates the result
      hash_hex[strlen(tv_hash[i])] = '\0';
    }
    cr_expect(eq(str, hash_hex, (char*)tv_hash[i]));

    free(hash_hex);
    free(key_bytes);
    free(data_bytes);
  }
  return all_passed;
  //
}

Test(hmac_test_suite, test_hmac_sha256) {
  test_hmac(&hmac_sha256, SHA256_HASH_SIZE, tv_sha256_keys, tv_sha256_data, tv_sha256_hash);
}

Test(hmac_test_suite, test_hmac_sha1) {
  test_hmac(&hmac_sha1, SHA1_HASH_SIZE, tv_sha1_keys, tv_sha1_data, tv_sha1_hash);
}
