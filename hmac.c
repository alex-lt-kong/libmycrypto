/*
   hmac_sha256.c
   Originally written by https://github.com/h5p9sl
 */

#include "hmac.h"
#include "sha1.h"
#include "sha256.h"

#include <stdlib.h>
#include <string.h>


static void* H(
  void (*hash_func)(unsigned char*, const unsigned char*, size_t),
  const void* x, const size_t xlen, const void* y, const size_t ylen, void* out
) {
  size_t buflen = (xlen + ylen);
  unsigned char* buf = (unsigned char*)malloc(buflen);

  memcpy(buf, x, xlen);
  memcpy(buf + xlen, y, ylen);
  (*hash_func)(out, buf, buflen);

  free(buf);
}

void hmac(
  const size_t BLOCK_SIZE, const size_t HASH_SIZE,
  void (*hash_func)(unsigned char*, const unsigned char*, size_t),
  const void* key, const size_t key_len,
  const void* msg, const size_t msg_len, void* out) {
  unsigned char k_prime[BLOCK_SIZE];
  unsigned char ipad[BLOCK_SIZE]; // ipad is the block-sized inner padding, consisting of repeated bytes valued 0x36
  unsigned char opad[BLOCK_SIZE]; // opad is the block-sized outer padding, consisting of repeated bytes valued 0x5c
  unsigned char ihash[HASH_SIZE];
  unsigned char ohash[HASH_SIZE];
  int i;

  memset(k_prime, 0, sizeof(k_prime));
  memset(ipad, 0x36, BLOCK_SIZE);
  memset(opad, 0x5c, BLOCK_SIZE);

  if (key_len > BLOCK_SIZE) { (*hash_func)(k_prime, key, key_len); }
  else { memcpy(k_prime, key, key_len); }

  for (int i = 0; i < BLOCK_SIZE; i++) {
    ipad[i] ^= k_prime[i]; // ^: bitwise XOR
    opad[i] ^= k_prime[i];
  }

  // Perform HMAC algorithm: ( https://tools.ietf.org/html/rfc2104 )
  //      `H(K XOR opad, H(K XOR ipad, data))`
  H(hash_func, ipad, BLOCK_SIZE, msg, msg_len, ihash);
  H(hash_func, opad, BLOCK_SIZE, ihash, HASH_SIZE, ohash);

  memcpy(out, ohash, HASH_SIZE);
}

void hmac_sha1(const void* key, const size_t key_len, const void* msg, const size_t msg_len, void* out) {
  hmac(SHA1_BLOCK_SIZE, SHA1_HASH_SIZE, cal_sha1_hash, key, key_len, msg, msg_len, out);
}

void hmac_sha256(const void* key, const size_t key_len, const void* msg, const size_t msg_len, void* out) {
  hmac(SHA256_BLOCK_SIZE, SHA256_HASH_SIZE, cal_sha256_hash, key, key_len, msg, msg_len, out);
}