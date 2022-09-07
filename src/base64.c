/*
 * Base64 encoding/decoding (RFC1341)
 * Copyright (c) 2005, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 */

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "base64.h"

static const uint8_t base64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
// This table is defined in RFC 4648

char* encode_bytes_to_base64_string(const uint8_t* input_bytes, const size_t input_len)
{
	char *output_chars, *pos;
	const size_t CHARS_PER_LINE = 72;
	const uint8_t* input_start = input_bytes;
	const uint8_t* input_end = input_start + input_len;
	size_t olen;
	int line_len;

	olen = input_len * 4 / 3 + 4; /* 3-byte blocks to 4-byte */
	olen += olen / CHARS_PER_LINE; /* line feeds */
	olen++; /* nul termination */
	output_chars = (char *)calloc(olen, sizeof(char));
	if (output_chars == NULL) { return NULL; }

	pos = output_chars;
	line_len = 0;
	while (input_end - input_start >= 3) {
		// Let's have an example here, say we need to encode three 3 bytes 01001111 11100110 10010111
		*pos++ = base64_table[input_start[0] >> 2];
		// The funny (*pos++ = val) means we assign val to *pos and then we point pos to the next address (i.e., pos + 1)
		// encode the first 6 bits, [010011]11 by right shifting them: 00[010011]
		*pos++ = base64_table[((input_start[0] & 0x03) << 4) | (input_start[1] >> 4)];
		// encode the 7th to 12th bits, 010011[11 1110]0110
		// extract the last two bits: input_start[0] & 0x03: 01001111 & 0000011 = 00000011
		// shift them to lower places:  ((input_start[0] & 0x03) << 4): 00110000
		// extract the first 4 bits from the 2nd byte: input_start[1] >> 4: 11100110 >> 4 = 00001110
		// concatenate last 2 bits from 1st byte and first 4 bits from 2nd byte: 00110000 | 00001110 = 00111110
		*pos++ = base64_table[((input_start[1] & 0x0f) << 2) | (input_start[2] >> 6)];
		// Very similar to the above, 0x0f == 0b00001111
		*pos++ = base64_table[input_start[2] & 0x3f];
		// 0x3f = 0b00111111, so we extract the last 6 bits from the 3rd byte.
		input_start += 3;
		line_len += 4;
		if (line_len >= CHARS_PER_LINE) {
			*pos++ = '\n';
			line_len = 0;
		}
	}

	if (input_end - input_start) {
		// There could be one or two remaining bytes, either case, we need to encode the first 6 bits and ensure
		// the final result is 3-byte (i.e., 24-bit) long.
		*pos++ = base64_table[input_start[0] >> 2];
		if (input_end - input_start == 1) {
			// If there is one byte left, say, 11100111, we want to make it 11100111 00000000 00000000
			// we need to encode it into two characters and two extra padding character as follows			
			// [111001][11 0000][0000 00][000000]
			*pos++ = base64_table[(input_start[0] & 0x03) << 4];
			// [111001] is done before if, now we handle the 111001[11 0000]0000 part
			*pos++ = '=';
			// handles 11100111 0000[0000 00]000000 part. The last 6 bits will be handled after if.
		} else {
			// If there are two bytes left, say, 11100111, we want to make it 11100111 01001001 00000000
			*pos++ = base64_table[((input_start[0] & 0x03) << 4) | (input_start[1] >> 4)];
			// handles the 111001[11 0100]1001 00000000 part
			*pos++ = base64_table[(input_start[1] & 0x0f) << 2];
			// handles 11100111 0100[1001 00]000000 part, 0x0f == 0b00001111
		}
		*pos++ = '='; // either case, the last = stands for the last 4 zero bits
		line_len += 4;
	}

	*pos = '\0';
	return output_chars;
}


uint8_t * decode_base64_string_to_bytes(const char *input_chars, size_t *output_len)
{
	uint8_t dtable[256], *out, *pos, in[4], block[4], tmp;
	size_t i, count, olen;

	memset(dtable, 0x80, 256);

	for (i = 0; i < sizeof(base64_table) - 1; i++)
		dtable[base64_table[i]] = (uint8_t) i;
	dtable['='] = 0;
  
	count = 0;
	for (i = 0; i < strlen(input_chars); i++) {
		if (dtable[input_chars[i]] != 0x80)
			count++;
	}
  
	if (count % 4)
		return NULL;

	olen = count / 4 * 3;
	pos = out = (uint8_t*)malloc(olen);
	if (out == NULL) { return NULL; }
  
	count = 0;
	for (i = 0; i < strlen(input_chars); i++) {
		tmp = dtable[input_chars[i]];
		if (tmp == 0x80)
			continue;

		in[count] = input_chars[i];
		block[count] = tmp;
		count++;
		if (count == 4) {
			*pos++ = (block[0] << 2) | (block[1] >> 4);
			*pos++ = (block[1] << 4) | (block[2] >> 2);
			*pos++ = (block[2] << 6) |  block[3];
			count = 0;
		}
	}
  
	if (pos > out) {
		if (in[2] == '=')
			pos -= 2;
		else if (in[3] == '=')
			pos--;
	}
	*output_len = pos - out;
  
	return out;
}