#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base32.h"

#define GROUP_SIZE 8
#define BLOCK_SIZE 5
static const char b32_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
// There are a few different encoding schemes. The current one is from RFC
// 4648/3548 If need to change this, also need to change the invalid character
// check in decode_base32_string_to_bytes()

/**
 * @brief Encode a 5-byte long input bytes block to an 8-byte long output base32
 * char array
 * @param in a 5-byte long input block
 * @param out an array of 8 bytes of base32 character
 */
void encode_block(uint8_t *in, char *out) {
  out[0] = (in[0] & 0xf8) >> 3;
  out[1] = ((in[0] & 0x07) << 2) | ((in[1] & 0xc0) >> 6);
  out[2] = (in[1] & 0x3e) >> 1;
  out[3] = ((in[1] & 0x01) << 4) | ((in[2] & 0xf0) >> 4);
  out[4] = ((in[2] & 0x0f) << 1) | ((in[3] & 0x80) >> 7);
  out[5] = (in[3] & 0x7c) >> 2;
  out[6] = ((in[3] & 0x03) << 3) | ((in[4] & 0xe0) >> 5);
  out[7] = in[4] & 0x1f;
}

/**
 * @brief Decode an 8-byte long input char array to a 5-byte long output byte
 * array
 * @param in an 8-byte long input char array
 * @param out an a 5-byte long output bytes array
 */
void decode_group(char *in, uint8_t *out) {
  out[0] = (in[0] << 0x03) + ((in[1] & 0x1c) >> 2);
  out[1] = ((in[1] & 0x03) << 6) + (in[2] << 1) + ((in[3] & 0x10) >> 4);
  out[2] = ((in[3] & 0x0f) << 4) + ((in[4] & 0x1e) >> 1);
  out[3] = ((in[4] & 0x01) << 7) + (in[5] << 2) + ((in[6] & 0x18) >> 3);
  out[4] = ((in[6] & 0x07) << 5) + in[7];
}

char *encode_bytes_to_base32_string(const uint8_t *input_bytes,
                                    size_t input_len) {
  int blk_idx = 0;
  int grp_idx = 0;
  char *output = NULL;

  int out_pos = 0;
  /* 5-byte block into 8 groups of 5 bits */
  size_t output_len = ceil((float)input_len / BLOCK_SIZE) * GROUP_SIZE + 1;

  char buf[GROUP_SIZE];
  uint8_t tmp[BLOCK_SIZE];

  output = (char *)calloc(output_len, sizeof(char));
  if (output == NULL) {
    return NULL;
  }

  while (input_len--) {
    tmp[blk_idx++] = *(input_bytes++);
    if (blk_idx < BLOCK_SIZE) {
      continue;
    }
    // we read BLOCK_SIZE bytes or until the end of input_bytes into tmp
    encode_block(tmp, buf);

    for (blk_idx = 0; blk_idx < GROUP_SIZE; blk_idx++) {
      output[out_pos++] = b32_table[(int8_t)buf[blk_idx]];
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
      output[out_pos++] = b32_table[(int8_t)buf[grp_idx]];
    }

    while (
        grp_idx++ <
        GROUP_SIZE) { // while there is still a remainder append `=' to `output'
      output[out_pos++] = '=';
    }
  }

  return output;
}

uint8_t *decode_base32_string_to_bytes(const char *input_chars,
                                       ssize_t *output_len) {

  int out_pos = 0;
  *output_len = strlen(input_chars) * BLOCK_SIZE / GROUP_SIZE;
  // We pre-allocate this number of bytes, but some of them may not be used due
  // to the existence of padding ='s *output_len will be shrunk a bit at the
  // end.
  uint8_t *output_bytes = NULL;
  uint8_t buf[BLOCK_SIZE];
  char tmp[GROUP_SIZE] = {0};

  output_bytes = (uint8_t *)malloc(*output_len * sizeof(uint8_t));
  if (NULL == output_bytes) {
    *output_len = -2;
    return NULL;
  }

  int in_pos = 0;
  int grp_idx = 0;
  for (size_t i = 0; i < strlen(input_chars); ++i) {
    if ('=' == input_chars[in_pos]) {
      break;
    } // reaching the end of the input
    // if not base32 char
    if (!((input_chars[in_pos] >= 'A' && input_chars[in_pos] <= 'Z') ||
          (input_chars[in_pos] >= '2' && input_chars[in_pos] <= '7'))) {
      free(output_bytes);
      *output_len = -1;
      return NULL;
    }

    tmp[grp_idx++] = input_chars[in_pos++];
    if (grp_idx < GROUP_SIZE) {
      continue;
    }

    for (int i = 0; i < GROUP_SIZE; i++) {
      for (int j = 0; j < 32; j++) {
        if (tmp[i] == b32_table[j]) {
          tmp[i] = j;
          break; // here we translate encoded character back to index (as in
                 // b32_table)
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
    for (in_pos = 0; (in_pos < grp_idx * 5 / 8); ++in_pos) {
      output_bytes[out_pos++] = buf[in_pos];
    }
  }

  *output_len = out_pos;
  return output_bytes;
}
