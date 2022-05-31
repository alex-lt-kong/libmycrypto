#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "base32.h"

#define GROUP_SIZE 8
#define BLOCK_SIZE 5
static const char b32_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

/*
 * @brief Encode a 5-byte long input bytes block to an 8-byte long output base32 char array
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

/*
 * @brief Decode an 8-byte long input char array to a 5-byte long output byte array
 * @param in an 8-byte long input char array
 * @param out an a 5-byte long output bytes array
 */
void decode_group(char* in, unsigned char* out) {
    out[0] = ( in[0] << 0b00000011) +       ((in[1] & 0b00011100) >> 2);
    out[1] = ((in[1]  & 0b00000011) << 6) +  (in[2] << 1) + ((in[3] & 0x10) >> 4);
    out[2] = ((in[3]  & 0b00001111) << 4) + ((in[4] & 0b00011110) >> 1);
    out[3] = ((in[4]  & 0b00000001) << 7) +  (in[5] << 2) + ((in[6] & 0b00011000) >> 3);
    out[4] = ((in[6]  & 0b00000111) << 5) +   in[7];
}

char* encode_bytes_to_base32_string(const unsigned char *input_bytes, size_t input_len, size_t* output_len) {
  int blk_idx = 0;
  int grp_idx = 0;
  char* output = NULL;
  
  int out_pos = 0;
  *output_len = ceil((float)input_len / BLOCK_SIZE) * GROUP_SIZE; /* 5-byte block into 8 groups of 5 bits */  
  char buf[GROUP_SIZE];
  unsigned char tmp[BLOCK_SIZE];

  output = (char *)calloc(*output_len, sizeof(char));
  if (output == NULL) { return NULL; }

  while (input_len--) {
    tmp[blk_idx++] = *(input_bytes++);
    if (blk_idx < BLOCK_SIZE) { continue; }
    // we read BLOCK_SIZE bytes or until the end of input_bytes into tmp
    encode_block(tmp, buf);
    
    for (blk_idx = 0; blk_idx < GROUP_SIZE; blk_idx++) {
      output[out_pos++] = b32_table[buf[blk_idx]];
    }
    blk_idx = 0;
  }

  // handle remainder
  if (blk_idx > 0) {    
    for (grp_idx = blk_idx; grp_idx < BLOCK_SIZE; grp_idx++) {
      tmp[grp_idx] = '\0';
    }
    encode_block(tmp, buf);    
    for (grp_idx = 0; (grp_idx < blk_idx * 8 / BLOCK_SIZE + 1); grp_idx++) {
      output[out_pos++] = b32_table[buf[grp_idx]]; 
    }
    
    while (grp_idx++ < GROUP_SIZE) { // while there is still a remainder append `=' to `output'
      output[out_pos++] = '=';
    }
  }

  return output;
}


unsigned char* decode_base32_string_to_bytes(const char *input_chars, size_t input_len, size_t *decsize) {
  int grp_idx = 0;
  int j = 0;
  int l = 0;
  size_t size = 0;
  size_t output_len = input_len * BLOCK_SIZE / GROUP_SIZE; // There may be a few bytes left un-used
  unsigned char *output_bytes = NULL;
  unsigned char buf[BLOCK_SIZE];
  char tmp[GROUP_SIZE];

  output_bytes = (unsigned char *)calloc(output_len, sizeof(unsigned char));
  if (NULL == output_bytes) { return NULL; }

  while (input_len--) {    
    if ('=' == input_chars[j]) { break; } // reaching the end of the input
    // if not base32 char
    if (!((input_chars[j] >= 'A' && input_chars[j] <= 'Z') || 
          (input_chars[j] >= '2' && input_chars[j] <= '7'))) {
      break; // If a char is invalid, break!
    }
    
    tmp[grp_idx++] = input_chars[j++];
    if (grp_idx < GROUP_SIZE) { continue; }
    // read up to 8 bytes at a time into `tmp'

    // translate values in `tmp' from table
    for (grp_idx = 0; grp_idx < GROUP_SIZE; ++grp_idx) {
      // find translation char in `b32_table'
      for (int i = 0; i < 32; i++) {
        if (tmp[grp_idx] == b32_table[i]) {
          tmp[grp_idx] = i;
          break;
        }
      }
    }
    decode_group(tmp, buf);
    // write decoded buffer to `output_bytes'
    for (int i = 0; i < 5; i++) {
      output_bytes[size++] = buf[i];
    }
    grp_idx = 0;

  }

  // remainder
  if (grp_idx > 0) {
    // fill `tmp' with `\0' at most 8 times
    for (j = grp_idx; j < 8; ++j) {
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
    decode_group(tmp, buf);

    // write remainder decoded buffer to `output_bytes'  
    for (j = 0; (j < grp_idx*5/8); ++j) {
      output_bytes[size++] = buf[j];
    }
  }

  // Make sure we have enough space to add '\0' character at end. 
  output_bytes[size] = '\0';

  // Return back the size of decoded string if demanded.
  if (decsize != NULL) *decsize = size;

  return output_bytes;
}