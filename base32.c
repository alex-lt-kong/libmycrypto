#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "base32.h"

#define GROUP_SIZE 8
#define BLOCK_SIZE 5
static const char b32_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
// If need to change this, also need to change the invalid character check in decode_base32_string_to_bytes()

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
    out[1] = ((in[1]  & 0b00000011) << 6) +  (in[2] << 1) + ((in[3] & 0x00000010) >> 4);
    out[2] = ((in[3]  & 0b00001111) << 4) + ((in[4] & 0b00011110) >> 1);
    out[3] = ((in[4]  & 0b00000001) << 7) +  (in[5] << 2) + ((in[6] & 0b00011000) >> 3);
    out[4] = ((in[6]  & 0b00000111) << 5) +   in[7];
}

char* encode_bytes_to_base32_string(const unsigned char *input_bytes, size_t input_len) {
  int blk_idx = 0;
  int grp_idx = 0;
  char* output = NULL;
  
  int out_pos = 0;
  size_t output_len = ceil((float)input_len / BLOCK_SIZE) * GROUP_SIZE; /* 5-byte block into 8 groups of 5 bits */  
  char buf[GROUP_SIZE];
  unsigned char tmp[BLOCK_SIZE];

  output = (char *)calloc(output_len, sizeof(char));
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


unsigned char* decode_base32_string_to_bytes(const char *input_chars, size_t *output_len) {
  
  int out_pos = 0;
  *output_len = strlen(input_chars) * BLOCK_SIZE / GROUP_SIZE;
  // We pre-allocate this number of bytes, but some of them may not be used due to the existence of padding ='s
  // *output_len will be shrunk a bit at the end.
  unsigned char *output_bytes = NULL;
  unsigned char buf[BLOCK_SIZE];
  char tmp[GROUP_SIZE];

  output_bytes = (unsigned char *)calloc(*output_len, sizeof(unsigned char));
  if (NULL == output_bytes) { return NULL; }

  int in_pos = 0;
  int grp_idx = 0;
  for (int i = 0; i < strlen(input_chars); ++i) {
    if ('=' == input_chars[in_pos]) { break; } // reaching the end of the input
    // if not base32 char
    if (!((input_chars[in_pos] >= 'A' && input_chars[in_pos] <= 'Z') || 
          (input_chars[in_pos] >= '2' && input_chars[in_pos] <= '7'))) {
      free(output_bytes);
      return NULL;
    }
    
    tmp[grp_idx++] = input_chars[in_pos++];
    if (grp_idx < GROUP_SIZE) { continue; }

    for (int i = 0; i < GROUP_SIZE; i++) {
      for (int j = 0; j < 32; j++) {
        if (tmp[i] == b32_table[j]) {
          tmp[i] = j; break; // here we translate encoded character back to index (as in b32_table)
        }
      }
    }
    decode_group(tmp, buf);
    memcpy(output_bytes + out_pos, buf, BLOCK_SIZE);
    out_pos += BLOCK_SIZE;
    grp_idx = 0;
  }

  // remainder
  if (grp_idx > 0) {
    // translate remainder
    for (int i = 0; i < GROUP_SIZE; i++) {
        // find translation char in `b32_table'
        for (int j = 0; j < 32; ++j) {
          if (tmp[i] == b32_table[j]) {
            tmp[i] = j;
            break;
          }
        }
    }
    // decode remainder
    decode_group(tmp, buf);

    // write remainder decoded buffer to `output_bytes'  
    for (in_pos = 0; (in_pos < grp_idx*5/8); ++in_pos) {
      output_bytes[out_pos++] = buf[in_pos];
    }
  }

  *output_len = out_pos;
  return output_bytes;
}