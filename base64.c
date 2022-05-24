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

static const unsigned char base64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
// This table is defined in RFC 4648

char* encode_bytes_to_base64_string(const unsigned char* input_bytes, const size_t input_len, size_t *out_len)
{
	char *output_chars, *pos;
	const unsigned char* input_start = input_bytes;
	const unsigned char* input_end = input_start + input_len;
	size_t olen;
	int line_len;

	olen = input_len * 4 / 3 + 4; /* 3-byte blocks to 4-byte */
	olen += olen / 72; /* line feeds */
	olen++; /* nul termination */
	output_chars = (char *)calloc(olen, sizeof(char));
	if (output_chars == NULL)
		return NULL;

	pos = output_chars;
	line_len = 0;
	while (input_end - input_start >= 3) {
		*pos++ = base64_table[input_start[0] >> 2];
		*pos++ = base64_table[((input_start[0] & 0x03) << 4) | (input_start[1] >> 4)];
		*pos++ = base64_table[((input_start[1] & 0x0f) << 2) | (input_start[2] >> 6)];
		*pos++ = base64_table[input_start[2] & 0x3f];
		input_start += 3;
		line_len += 4;
		if (line_len >= 72) {
			*pos++ = '\n';
			line_len = 0;
		}
	}

	if (input_end - input_start) {
		*pos++ = base64_table[input_start[0] >> 2];
		if (input_end - input_start == 1) {
			*pos++ = base64_table[(input_start[0] & 0x03) << 4];
			*pos++ = '=';
		} else {
			*pos++ = base64_table[((input_start[0] & 0x03) << 4) |
					       (input_start[1] >> 4)];
			*pos++ = base64_table[(input_start[1] & 0x0f) << 2];
		}
		*pos++ = '=';
		line_len += 4;
	}

	if (line_len)
		*pos++ = '\n';

	*pos = '\0';
	if (out_len)
		*out_len = pos - output_chars;
	return output_chars;
}

/**
 * base64_decode - Base64 decode
 * @input_bytes: Data to be decoded
 * @len: Length of the data to be decoded
 * @out_len: Pointer to output length variable
 * Returns: Allocated buffer of out_len bytes of decoded data,
 * or %NULL on failure
 *
 * Caller is responsible for freeing the returned buffer.
 */
unsigned char * decode_base64_string_to_bytes(const char *input_bytes, size_t len, size_t *out_len)
{
	unsigned char dtable[256], *out, *pos, in[4], block[4], tmp;
	size_t i, count, olen;

	memset(dtable, 0x80, 256);

	for (i = 0; i < sizeof(base64_table) - 1; i++)
		dtable[base64_table[i]] = (unsigned char) i;
	dtable['='] = 0;
  
	count = 0;
	for (i = 0; i < len; i++) {
		if (dtable[input_bytes[i]] != 0x80)
			count++;
	}
  
	if (count % 4)
		return NULL;

	olen = count / 4 * 3;
	pos = out = (unsigned char*)malloc(olen);
	if (out == NULL)
		return NULL;
  
	count = 0;
	for (i = 0; i < len; i++) {
		tmp = dtable[input_bytes[i]];
		if (tmp == 0x80)
			continue;

		in[count] = input_bytes[i];
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
	*out_len = pos - out;
  
	return out;
}