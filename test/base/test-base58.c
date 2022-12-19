#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "../../src/base58.h"
#include "../../src/misc.h"

#define OFFICIAL_TEST_SIZE 1024
#define BTC_TEST_SIZE 1024

const size_t official_test_vectors_decoded_len[]={12, 44, 6};
const uint8_t official_test_vectors_decoded[][OFFICIAL_TEST_SIZE]={
  "Hello World!",
  "The quick brown fox jumps over the lazy dog.",
  {0x00, 0x00, 0x28, 0x7f, 0xb4, 0xcd},
};
const char test_vectors_encoded[][OFFICIAL_TEST_SIZE] = {  
  "2NEpo7TZRRrLZSi2U",
  "USm3fpXnKG5EUBx2ndxBDMPVciP5hGey2Jh4NDv6gmeo1LkMeiKrLJUUBk6Z",
  "11233QC4"
};


const uint8_t btc_test_vectors_decoded_hex_str[][BTC_TEST_SIZE]={
  "",
  "61",
  "626262",
  "636363",
  "73696d706c792061206c6f6e6720737472696e67",
  "00eb15231dfceb60925886b67d065299925915aeb172c06647",
  "516b6fcd0f", 
  "bf4f89001e670274dd",
  "572e4794",
  "ecac89cad93923c02321",
  "10c8511e",
  "00000000000000000000",
  "000111d38e5fc9071ffcd20b4a763cc9ae4f252bb4e48fd66a835e252ada93ff480d6dd43dc62a641155a5",
  "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9fa0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebfc0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedfe0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff"
};

const uint8_t btc_test_vectors_encoded[][BTC_TEST_SIZE]={
  "",
  "2g",
  "a3gV",
  "aPEr",
  "2cFupjhnEsSn59qHXstmK2ffpLv2",
  "1NS17iag9jJgTHD1VXjvLCEnZuQ3rJDE9L",
  "ABnLTmg",
  "3SEo3LWLoPntC",
  "3EFU7m",
  "EJDM8drfXA6uyA",
  "Rt5zm",
  "1111111111",
  "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz",
  "1cWB5HCBdLjAuqGGReWE3R3CguuwSjw6RHn39s2yuDRTS5NsBgNiFpWgAnEx6VQi8csexkgYw3mdYrMHr8x9i7aEwP8kZ7vccXWqKDvGv3u1GxFKPuAkn8JCPPGDMf3vMMnbzm6Nh9zh1gcNsMvH3ZNLmP5fSG6DGbbi2tuwMWPthr4boWwCxf7ewSgNQeacyozhKDDQQ1qL5fQFUW52QKUZDZ5fw3KXNQJMcNTcaB723LchjeKun7MuGW5qyCBZYzA1KjofN1gYBV3NqyhQJ3Ns746GNuf9N2pQPmHz4xpnSrrfCvy6TVVz5d4PdrjeshsWQwpZsZGzvbdAdN8MKV5QsBDY"
};

Test(test_base_suite, test_base58_official) {
 char output[1024] = {0};
  for (int i = 0; i < sizeof(test_vectors_encoded)/sizeof(test_vectors_encoded[0]); ++i) {    
    size_t output_len = 512;
    bool res = b58enc(official_test_vectors_decoded[i], official_test_vectors_decoded_len[i], output, &output_len);   
    cr_assert(res);
    cr_expect(eq(str, output, (char*)test_vectors_encoded[i]));  
  }
}

Test(test_base_suite, test_base58_btc) {
 char output[1024] = {0};
 uint8_t* input_bytes;
 size_t input_len;
  for (int i = 0; i < sizeof(btc_test_vectors_encoded) / sizeof(btc_test_vectors_encoded[0]); ++i) {    
    size_t output_len = 1024;
    input_bytes = hex_string_to_bytes(btc_test_vectors_decoded_hex_str[i], &input_len);
    cr_assert(input_bytes != NULL);
    bool res = b58enc(input_bytes, input_len, output, &output_len);   
    cr_assert(res);
    cr_expect(eq(str, output, (char*)btc_test_vectors_encoded[i]));  
  }
}
