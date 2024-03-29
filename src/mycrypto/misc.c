#include "misc.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t rotate(const uint32_t value, uint32_t bits, const bool is_left) {
  const uint32_t bit_width = sizeof(uint32_t) * CHAR_BIT;
  bits = (bits % bit_width);
  if (is_left) {
    return value << bits | value >> ((bit_width - bits) % bit_width);
  } else {
    return value >> bits | value << ((bit_width - bits) % bit_width);
  }
}

bool is_big_endian(void) {
  volatile uint32_t i = 0x01234567;
  return (*((unsigned char *)(&i))) != 0x67;
}

unsigned char char_to_hex(const char c) {
  if ('0' <= c && c <= '9')
    return (unsigned char)(c - '0');
  if ('A' <= c && c <= 'F')
    return (unsigned char)(c - 'A' + 10);
  if ('a' <= c && c <= 'f')
    return (unsigned char)(c - 'a' + 10);
  return 0xFF;
}

unsigned char *hex_string_to_bytes(const char *input_chars,
                                   ssize_t *output_len) {
  if (strlen(input_chars) % 2 != 0) {
    fprintf(stderr,
            "[%s] not a valid hex_string: length not a multiple of 2.\n",
            input_chars);
    *output_len = -1;
    return NULL;
  }

  *output_len = strlen(input_chars) / 2;
  unsigned char *output_bytes =
      (unsigned char *)calloc(*output_len, sizeof(unsigned char));
  if (output_bytes == NULL) {
    *output_len = -2;
    fprintf(stderr, "[%s] malloc() failed\n", __func__);
    return NULL;
  }
  unsigned char msn, lsn, byte;
  int out_idx = 0;
  while (*input_chars) {
    msn = char_to_hex(*input_chars++);
    if (msn == 0xFF) {
      *output_len = -3;
      fprintf(stderr, "%c is invalid\n", *(input_chars - 1));
      return NULL;
    }
    lsn = char_to_hex(*input_chars++);
    if (lsn == 0xFF) {
      *output_len = -3;
      fprintf(stderr, "%c is invalid\n", *(input_chars - 1));
      return NULL;
    }
    byte = (msn << 4) + lsn;

    *(output_bytes + out_idx++) = byte;
  }
  return output_bytes;
}

char *bytes_to_hex_string(const unsigned char *input_bytes,
                          const size_t input_len, const bool upper) {
  char hex_table[] = "0123456789abcdef";
  if (upper) {
    for (int i = 10; i < 16; ++i) {
      hex_table[i] -= 32;
    }
  }
  char *output_chars = (char *)calloc(input_len * 2 + 1, sizeof(char));
  if (output_chars == NULL) {
    fprintf(stderr, "[%s] calloc() failed\n", __func__);
    return NULL;
  }
  int out_idx = 0;

  unsigned char in_idx;
  for (size_t i = 0; i < input_len; ++i) {
    in_idx = *(input_bytes++);

    output_chars[out_idx++] = hex_table[(in_idx >> 4) & 0x0F];
    output_chars[out_idx++] = hex_table[in_idx & 0x0F];
  }
  return output_chars;
}

void switch_endianness(uint32_t *val) {
  *val = ((*val & 0xff000000) >> 24 | (*val & 0x00ff0000) >> 8 |
          (*val & 0x0000ff00) << 8 | (*val & 0x000000ff) << 24);
}
