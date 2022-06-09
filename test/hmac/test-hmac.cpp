#include <assert.h>
#include <fstream>
#include <limits>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../../hmac.h"
#include "../../sha1.h"
#include "../../sha256.h"
#include "../../misc.h"

using namespace std;

static_assert (CHAR_BIT == 8);

#define TEST_COUNT 7
#define TEST_SIZE 512
#define MAX_PRINT_LEN 64

const char tv_keys[TEST_COUNT][TEST_SIZE] = {
  "0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b",
  "4a656665",
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
  "0102030405060708090a0b0c0d0e0f10111213141516171819",
  "0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c",
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
};

const char tv_data[TEST_COUNT][TEST_SIZE] = {
  "4869205468657265",
  "7768617420646f2079612077616e7420666f72206e6f7468696e673f",
  "dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd",
  "cdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcd",
  "546573742057697468205472756e636174696f6e",
  "54657374205573696e67204c6172676572205468616e20426c6f636b2d53697a65204b6579202d2048617368204b6579204669727374",
  "5468697320697320612074657374207573696e672061206c6172676572207468616e20626c6f636b2d73697a65206b657920616e642061206c6172676572207468616e20626c6f636b2d73697a6520646174612e20546865206b6579206e6565647320746f20626520686173686564206265666f7265206265696e6720757365642062792074686520484d414320616c676f726974686d2e"
};

const char tv_sha1_hash[TEST_COUNT][TEST_SIZE] = {
  "b0344c61d8db38535ca8afceaf0bf12b881dc200c9833da726e9376c2e32cff7",
  "5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843",
  "773ea91e36800e46854db8ebd09181a72959098b3ef8c122d9635514ced565fe",
  "82558a389a443c0ea4cc819899f2083a85f0faa3e578f8077a2e3ff46729665b",
  "a3b6167473100ee06e0c796c2955552b",
  "60e431591ee0b67f0d8a26aacbf5b77f8e0bc6213728c5140546040f0ee37f54",
  "9b09ffa71b942fcb27635fbcd5b0e944bfdc63644f0713938a7f51535c3a35e2"
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

void test_hmac(
  void (*hash_function)(const unsigned char*, size_t, const unsigned char*, size_t, unsigned char*),
  const size_t hash_size,
  const char tv_keys[TEST_COUNT][TEST_SIZE],
  const char tv_data[TEST_COUNT][TEST_SIZE],
  const char tv_hash[TEST_COUNT][TEST_SIZE]
) {
  unsigned char* key_bytes = nullptr;
  unsigned char* data_bytes = nullptr;
  unsigned char hash[hash_size];
  char* hash_hex = nullptr;
  size_t key_len, data_len;
  for (int i = 0; i < TEST_COUNT; ++i) {    
    key_bytes = hex_string_to_bytes(tv_keys[i], &key_len);
    data_bytes = hex_string_to_bytes(tv_data[i], &data_len);

    printf("Key:    ");
    for (int j = 0; j < MAX_PRINT_LEN && j < strlen(tv_keys[i]); ++j) { printf("%c", tv_keys[i][j]); }    
    strlen(tv_keys[i]) > MAX_PRINT_LEN ? printf("...[Truncated]\n") : printf("\n");
    printf("Data:   ");
    for (int j = 0; j < MAX_PRINT_LEN && j < strlen(tv_data[i]); ++j) { printf("%c", tv_data[i][j]); }
    strlen(tv_data[i]) > MAX_PRINT_LEN ? printf("...[Truncated]\n") : printf("\n");

    hash_function(key_bytes, key_len, data_bytes, data_len, hash);
    hash_hex = bytes_to_hex_string(hash, hash_size, false);
    if (strlen(tv_hash[i]) < strlen(hash_hex)) { // i.e., test vector truncates the result
      hash_hex[strlen(tv_hash[i])] = '\0';
    } 
    cout << "Hash:   " << hash_hex << "\nResult: " << (strcmp(hash_hex, tv_hash[i]) == 0 ? "Passed" : "!!!FAILED!!!") << "\n"  << endl;
    free(hash_hex);
    free(key_bytes);
    free(data_bytes);
  }
}


int main() {
  cout << "========== Testing HMAC-SHA256 ==========" << endl;
  test_hmac(&hmac_sha256, SHA256_HASH_SIZE, tv_keys, tv_data, tv_sha256_hash);
  
  cout << "\n\n========== Testing HMAC-SHA1 ==========" << endl;
  /*
  unsigned char key[] = {
    0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
    0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
    0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
    0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef
  };
  unsigned char data[] = { 0x48, 0x65, 0x6c, 0x6c, 0x6f }; // Hello
  unsigned char sha1_out[SHA1_HASH_SIZE];
  hmac_sha1(key, sizeof(key), data, sizeof(data), sha1_out);
  cout << "Function result: "<< bytes_to_hex_string(sha1_out, SHA1_HASH_SIZE, false) << endl;
  cout << "Expected result: 24dfd13fb32a4a50a3962997787970c3e0d77fe4" << endl;
  unsigned char sha256_out[SHA256_HASH_SIZE];
  hmac_sha256(key, sizeof(key), data, sizeof(data), sha256_out);
  cout << "Function Result: "<< bytes_to_hex_string(sha256_out, SHA256_HASH_SIZE, false) << endl;
  cout << "Expected result: 183273bbe9800d87a9a600b8c5232661a9ddb312df6f431d634f1db2c296cd32" << endl;

  unsigned char new_key[] = {
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa
  };
  char new_data[] = "This is a test using a larger than block-size key and a larger than block-size data. The key needs to be hashed before being used by the HMAC algorithm.";
  hmac_sha256(new_key, sizeof(new_key), (unsigned char*)new_data, strlen(new_data), sha256_out);
  cout << "Function Result: "<< bytes_to_hex_string(sha256_out, SHA256_HASH_SIZE, false) << endl;
  cout << "Expected result: 9b09ffa71b942fcb27635fbcd5b0e944bfdc63644f0713938a7f51535c3a35e2" << endl;*/
  return 0;
}
