#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base58.h"

// From: https://github.com/bitcoin/libbase58/blob/master/base58.c

static const char b58_table[] =
    "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

char *encode_bytes_to_base58_string(const uint8_t *input_bytes,
                                    const size_t input_len) {

  size_t zeros = 0;
  size_t size = (input_len - zeros) * 138 / 100 + 1;
  /*
   * How do we get the size in advance? We can consider it this way--In base58
   * encoding, we use 58 characters to encode bytes (or bits), how many bits can
   * be represented by one character? It is log(2)58 ≈ 5.8579 bits. Is it
   * possible for base58 character to present 6 bits? No, 2^6 = 64, that is to
   * say, characters will be exhausted before 0b11 1111 can be encoded. Since 1
   * byte = 8 bits, there are (input_len * 8) bits to be represented. Therefore,
   * we need: (input_len * 8) / log(2)58 = (input_len * log(2)256) / log(2)58 =
   * input_len * log(58)256 ≈ input_len * 1.36565 Then we round it up.
   */
  char *output = (char *)calloc(size, sizeof(char));
  size_t high = size - 1;
  while (zeros < input_len && input_bytes[zeros] == 0)
    ++zeros;

  uint8_t buf[size];
  memset(buf, 0, size);

  size_t buf_idx;
  for (size_t i = zeros; i < input_len; ++i) {
    buf_idx = size - 1;
    int carry = input_bytes[i];
    while ((buf_idx > high) || carry) {
      carry += 256 * buf[buf_idx];
      buf[buf_idx] = carry % 58;
      carry /= 58;
      if (buf_idx == 0) {
        // Otherwise buf_idx wraps to maxint which is > high
        break;
      }
      --buf_idx;
    }
    high = buf_idx;
  }

  size_t k = 0;
  while (k < size && !buf[k]) {
    ++k;
  }

  if (zeros) {
    memset(output, '1', zeros);
  }
  int i = 0;
  for (i = zeros; k < size; ++i, ++k) {
    output[i] = b58_table[buf[k]];
  }
  output[i] = '\0';

  return output;
}
