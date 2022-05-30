#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "base32.h"

static const char b32_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
static const size_t IN_BLK_SIZE = 5;
static const size_t OUT_GRP_SIZE = 8;

/*
 * @brief Encode
 * @param in a 5-byte long input block
 * @param out an array of 8 bytes of base32 character
 */
void encode_block(unsigned char* in, char* out) {
    out[0] =  (in[0] & 0b11111000) >> 3;
    out[1] = ((in[0] & 0b00000111) << 2) | ((in[1] & 0b11000000) >> 6);
    out[2] =  (in[1] & 0b00111110) >> 1;
    out[3] = ((in[1] & 0b00000001) << 4) | ((in[2] & 0b11110000) >> 4);
    out[4] = ((in[2] & 0b00001111) << 1) | ((in[3] & 0b10000000) >> 7);
    out[5] =  (in[3] & 0b01111100) >> 2;
    out[6] = ((in[3] & 0b00000011) << 3) | ((in[4] & 0b11100000) >> 5);
    out[7] =   in[4] & 0b00011111;
}


char* encode_bytes_to_base32_string(const unsigned char *input_bytes, size_t input_len, size_t* output_len) {
  int i = 0;
  int j = 0;
  char* output = NULL;
  
  int out_pos = 0;
  *output_len = ceil((float)input_len / IN_BLK_SIZE) * OUT_GRP_SIZE; /* 5-byte block into 8 groups of 5 bits */  
  char buf[OUT_GRP_SIZE];
  unsigned char tmp[IN_BLK_SIZE];

  output = (char *)calloc(*output_len, sizeof(char));
  if (output == NULL) { return NULL; }

  while (input_len--) {
    tmp[i++] = *(input_bytes++);
    if (i < IN_BLK_SIZE) { continue; }
    // we read IN_BLK_SIZE bytes or until the end of input_bytes into tmp
    encode_block(tmp, buf);
    
    for (i = 0; i < OUT_GRP_SIZE; ++i) {
      output[out_pos++] = b32_table[buf[i]];
    }
    i = 0;
  }

  // remainder
  if (i > 0) {
    // fill `tmp' with `\0' at most 5 times
    for (j = i; j < IN_BLK_SIZE; ++j) {
      tmp[j] = '\0';
    }
    encode_block(tmp, buf);    
    for (j = 0; (j < i*8/IN_BLK_SIZE + 1); ++j) {
      output[out_pos++] = b32_table[buf[j]];
    }
    
    while ((j++ < OUT_GRP_SIZE)) { // while there is still a remainder append `=' to `output'
      output[out_pos++] = '=';
    }
  }

  return output;
}


unsigned char* decode_base32_string_to_bytes(const char *src, size_t len, size_t *decsize) {
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
      buf[0] = ( tmp[0] << 0b00000011) +       ((tmp[1] & 0b00011100) >> 2);
      buf[1] = ((tmp[1]  & 0b00000011) << 6) +  (tmp[2] << 1) + ((tmp[3] & 0x10) >> 4);
      buf[2] = ((tmp[3]  & 0b00001111) << 4) + ((tmp[4] & 0b00011110) >> 1);
      buf[3] = ((tmp[4]  & 0b00000001) << 7) +  (tmp[5] << 2) + ((tmp[6] & 0b00011000) >> 3);
      buf[4] = ((tmp[6]  & 0b00000111) << 5) +   tmp[7];

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