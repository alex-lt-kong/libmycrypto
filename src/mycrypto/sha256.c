/**
 * A very straightforward and no-frills C implementaion of SHA256 based on the
 * pseudocode at https://en.wikipedia.org/wiki/SHA-2
 */

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "misc.h"
#include "sha256.h"

void cal_sha256_hash(const uint8_t *input_bytes, const size_t input_len,
                     uint8_t *hash) {
  if (input_len > 2147483647) {
    return; // we support up to this length only
  }
  // Initialize hash values:
  // first 32 bits of the fractional parts of the square roots of the first 8
  // primes 2..19:
  uint32_t h0 = 0x6a09e667;
  uint32_t h1 = 0xbb67ae85;
  uint32_t h2 = 0x3c6ef372;
  uint32_t h3 = 0xa54ff53a;
  uint32_t h4 = 0x510e527f;
  uint32_t h5 = 0x9b05688c;
  uint32_t h6 = 0x1f83d9ab;
  uint32_t h7 = 0x5be0cd19;

  // Initialize array of round constants:
  // (first 32 bits of the fractional parts of the cube roots of the first 64
  // primes 2..311):
  uint32_t k[64] = {
      0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
      0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
      0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
      0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
      0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
      0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
      0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
      0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
      0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
      0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
      0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

  /**
   * Pre-processing (Padding):
   * begin with the original message of length L bits
   * append a single '1' bit
   * append K '0' bits, where K is the minimum number >= 0 such that (L + 1 + K
   * + 64) is a multiple of 512 append L as a 64-bit big-endian integer, making
   * the total post-processed length a multiple of 512 bits such that the bits
   * in the message are: <original message of length L> 1 <K zeros> <L as 64 bit
   * integer> , (the number of bits will be a multiple of 512)
   *
   */
  size_t padded_len =
      input_len +
      (SHA256_CHUNK_SIZE_BIT - input_len * CHAR_BIT % SHA256_CHUNK_SIZE_BIT) /
          CHAR_BIT;
  if (padded_len - input_len < 9) {
    padded_len += 64;
  }

  uint8_t *padded_bytes = (uint8_t *)calloc(padded_len, sizeof(uint8_t));
  memcpy(padded_bytes, input_bytes,
         input_len); // begin with the original message of length L bits
  padded_bytes[input_len] = 0x80; // append a single '1' bit

  // append K '0' bits is not needed as we are using calloc();

  // append L as a 64-bit big-endian integer, making the total post-processed
  // length a multiple of 512 bits
  padded_bytes[padded_len - 4] = (uint8_t)(input_len * CHAR_BIT >> 24);
  padded_bytes[padded_len - 3] = (uint8_t)(input_len * CHAR_BIT >> 16);
  padded_bytes[padded_len - 2] = (uint8_t)(input_len * CHAR_BIT >> 8);
  padded_bytes[padded_len - 1] = (uint8_t)(input_len * CHAR_BIT >> 0);
  // such that the bits in the message are: <original message of length L> 1 <K
  // zeros> <L as 64 bit integer> , (the number of bits will be a multiple of
  // 512)

  const int chunk_count = padded_len / SHA256_CHUNK_SIZE;
  // Process the message in successive 512-bit chunks:
  for (int i = 0; i < chunk_count; ++i) {
    uint32_t w[SHA256_CHUNK_SIZE]; // create a 64-entry message schedule array
                                   // w[0..63] of 32-bit words. The initial
                                   // values in w[0..63] don't matter.
    uint8_t *chunk_pos = padded_bytes + i * SHA256_CHUNK_SIZE;
    for (int j = 0; j < 16; ++j) {
      // copy chunk into first 16 words w[0..15] of the message schedule array
      // (to handle endianness properly, we cant simply use memcpy()
      w[j] = (uint32_t)(*(chunk_pos++)) << 24;
      w[j] |= (uint32_t)(*(chunk_pos++)) << 16;
      w[j] |= (uint32_t)(*(chunk_pos++)) << 8;
      w[j] |= (uint32_t)(*(chunk_pos++));
    }
    // Extend the first 16 words into the remaining 48 words w[16..63] of the
    // message schedule array:
    for (int j = 16; j < 64; j++) {
      const uint32_t s0 = rotate(w[j - 15], 7, false) ^
                          rotate(w[j - 15], 18, false) ^ (w[j - 15] >> 3);
      const uint32_t s1 = rotate(w[j - 2], 17, false) ^
                          rotate(w[j - 2], 19, false) ^ (w[j - 2] >> 10);
      w[j] = w[j - 16] + s0 + w[j - 7] + s1;
    }
    // Initialize working variables to current hash value:
    uint32_t a = h0;
    uint32_t b = h1;
    uint32_t c = h2;
    uint32_t d = h3;
    uint32_t e = h4;
    uint32_t f = h5;
    uint32_t g = h6;
    uint32_t h = h7;

    // Compression function main loop:
    for (int j = 0; j < 64; ++j) {
      const uint32_t S1 =
          rotate(e, 6, false) ^ rotate(e, 11, false) ^ rotate(e, 25, false);
      const uint32_t ch = (e & f) ^ ((~e) & g);
      const uint32_t temp1 = h + S1 + ch + k[j] + w[j];
      const uint32_t S0 =
          rotate(a, 2, false) ^ rotate(a, 13, false) ^ rotate(a, 22, false);
      const uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
      const uint32_t temp2 = S0 + maj;

      h = g;
      g = f;
      f = e;
      e = d + temp1;
      d = c;
      c = b;
      b = a;
      a = temp1 + temp2;
    }
    // Add the compressed chunk to the current hash value:
    h0 += a;
    h1 += b;
    h2 += c;
    h3 += d;
    h4 += e;
    h5 += f;
    h6 += g;
    h7 += h;
  }
  free(padded_bytes);
  if (is_big_endian() == false) {
    switch_endianness(&h0);
    switch_endianness(&h1);
    switch_endianness(&h2);
    switch_endianness(&h3);
    switch_endianness(&h4);
    switch_endianness(&h5);
    switch_endianness(&h6);
    switch_endianness(&h7);
  }
  memcpy(hash + 0 * sizeof(uint32_t), &h0, sizeof(uint32_t));
  memcpy(hash + 1 * sizeof(uint32_t), &h1, sizeof(uint32_t));
  memcpy(hash + 2 * sizeof(uint32_t), &h2, sizeof(uint32_t));
  memcpy(hash + 3 * sizeof(uint32_t), &h3, sizeof(uint32_t));
  memcpy(hash + 4 * sizeof(uint32_t), &h4, sizeof(uint32_t));
  memcpy(hash + 5 * sizeof(uint32_t), &h5, sizeof(uint32_t));
  memcpy(hash + 6 * sizeof(uint32_t), &h6, sizeof(uint32_t));
  memcpy(hash + 7 * sizeof(uint32_t), &h7, sizeof(uint32_t));
}
