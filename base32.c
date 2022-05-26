
/**
 * `encode.c' - b32
 *
 * copyright (c) 2016 jay rama fisher
 * copyright (c) 2014 joseph werle
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "base32.h"

char *
b32_encode (const unsigned char *src, size_t len) {
  int i = 0;
  int j = 0;
  char *enc = NULL;
  size_t size = 0;
  unsigned char buf[8];
  unsigned char tmp[5];

  // alloc
  enc = (char *) malloc(0);
  if (NULL == enc) { return NULL; }

  // parse until end of source
  while (len--) {
    // read up to 5 bytes at a time into `tmp'
    tmp[i++] = *(src++);

    // if 5 bytes read then encode into `buf'
    if (5 == i) {
      buf[0] = (tmp[0] & 0xf8) >> 3;
      buf[1] = ((tmp[0] & 0x07) << 2) + ((tmp[1] & 0xc0) >> 6);
      buf[2] = ((tmp[1] & 0x3e) >> 1);
      buf[3] = ((tmp[1] & 0x01) << 4) + ((tmp[2] & 0xf0) >> 4);
      buf[4] = ((tmp[2] & 0x0f) << 1) + ((tmp[3] & 0x80) >> 7);
      buf[5] = (tmp[3] & 0x7c) >> 2;
      buf[6] = ((tmp[3] & 0x03) << 3) + ((tmp[4] & 0xe0) >> 5);
      buf[7] = tmp[4] & 0x1f;

      // allocate 8 new bytes for `enc` and
      // then translate each encoded buffer
      // part by index from the base 32 index table
      // into `enc' unsigned char array
      enc = (char *) realloc(enc, size + 8);
      for (i = 0; i < 8; ++i) {
        enc[size++] = b32_table[buf[i]];
      }

      // reset index
      i = 0;
    }
  }

  // remainder
  if (i > 0) {
    // fill `tmp' with `\0' at most 5 times
    for (j = i; j < 5; ++j) {
      tmp[j] = '\0';
    }

    // perform same codec as above
    buf[0] = (tmp[0] & 0xf8) >> 3;
    buf[1] = ((tmp[0] & 0x07) << 2) + ((tmp[1] & 0xc0) >> 6);
    buf[2] = ((tmp[1] & 0x3e) >> 1);
    buf[3] = ((tmp[1] & 0x01) << 4) + ((tmp[2] & 0xf0) >> 4);
    buf[4] = ((tmp[2] & 0x0f) << 1) + ((tmp[3] & 0x80) >> 7);
    buf[5] = (tmp[3] & 0x7c) >> 2;
    buf[6] = ((tmp[3] & 0x03) << 3) + ((tmp[4] & 0xe0) >> 5);
    buf[7] = tmp[4] & 0x1f;

    // perform same write to `enc` with new allocation
    for (j = 0; (j < i*8/5 + 1); ++j) {
      enc = (char *) realloc(enc, size + 1);
      enc[size++] = b32_table[buf[j]];
    }

    // while there is still a remainder
    // append `=' to `enc'
    while ((j++ < 8)) {
      enc = (char *) realloc(enc, size + 1);
      enc[size++] = '=';
    }
  }

  // Make sure we have enough space to add '\0' character at end.
  enc = (char *) realloc(enc, size + 1);
  enc[size] = '\0';

  return enc;
}


/**
 * `decode.c' - b32
 *
 * copyright (c) 2016 jay rama fisher
 * copyright (c) 2014 joseph werle
 */

unsigned char *
b32_decode (const char *src, size_t len) {
  return b32_decode_ex(src, len, NULL);
}

unsigned char *
b32_decode_ex (const char *src, size_t len, size_t *decsize) {
  int i = 0;
  int j = 0;
  int l = 0;
  size_t size = 0;
  unsigned char *dec = NULL;
  unsigned char buf[5];
  unsigned char tmp[8];

  // alloc
  dec = (unsigned char *) malloc(0);
  if (NULL == dec) { return NULL; }

  // parse until end of source
  while (len--) {
    // break if char is `=' or not base32 char
    if ('=' == src[j]) { break; }
    if (!(isalnum(src[j]) || '+' == src[j] || '/' == src[j])) { break; }

    // read up to 8 bytes at a time into `tmp'
    tmp[i++] = src[j++];

    // if 8 bytes read then decode into `buf'
    if (8 == i) {
      // translate values in `tmp' from table
      for (i = 0; i < 8; ++i) {
        // find translation char in `b32_table'
        for (l = 0; l < 32; ++l) {
          if (tmp[i] == b32_table[l]) {
            tmp[i] = l;
            break;
          }
        }
      }

      // decode
      buf[0] = (tmp[0] << 3) + ((tmp[1] & 0x1c) >> 2);
      buf[1] = ((tmp[1] & 0x03) << 6) + (tmp[2] << 1) + ((tmp[3] & 0x10) >> 4);
      buf[2] = ((tmp[3] & 0x0f) << 4) + ((tmp[4] & 0x1e) >> 1);
      buf[3] = ((tmp[4] & 0x01) << 7) + (tmp[5] << 2) + ((tmp[6] & 0x18) >> 3);
      buf[4] = ((tmp[6] & 0x07) << 5) + tmp[7];

      // write decoded buffer to `dec'
      dec = (unsigned char *) realloc(dec, size + 5);
      for (i = 0; i < 5; ++i) {
        dec[size++] = buf[i];
      }

      // reset
      i = 0;
    }
  }

  // remainder
  if (i > 0) {
    // fill `tmp' with `\0' at most 8 times
    for (j = i; j < 8; ++j) {
      tmp[j] = '\0';
    }

    // translate remainder
    for (j = 0; j < 8; ++j) {
        // find translation char in `b32_table'
        for (l = 0; l < 32; ++l) {
          if (tmp[j] == b32_table[l]) {
            tmp[j] = l;
            break;
          }
        }
    }

    // decode remainder
    buf[0] = (tmp[0] << 3) + ((tmp[1] & 0x1c) >> 2);
    buf[1] = ((tmp[1] & 0x03) << 6) + (tmp[2] << 1) + ((tmp[3] & 0x10) >> 4);
    buf[2] = ((tmp[3] & 0x0f) << 4) + ((tmp[4] & 0x1e) >> 1);
    buf[3] = ((tmp[4] & 0x01) << 7) + (tmp[5] << 2) + ((tmp[6] & 0x18) >> 3);
    buf[4] = ((tmp[6] & 0x07) << 5) + tmp[7];

    // write remainder decoded buffer to `dec'
    dec = (unsigned char *) realloc(dec, size + i*5/8);
    for (j = 0; (j < i*5/8); ++j) {
      dec[size++] = buf[j];
    }
  }

  // Make sure we have enough space to add '\0' character at end.
  dec = (unsigned char *) realloc(dec, size + 1);
  dec[size] = '\0';

  // Return back the size of decoded string if demanded.
  if (decsize != NULL) *decsize = size;

  return dec;
}