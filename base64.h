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

#ifndef BASE64_H
#define BASE64_H

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include <sys/types.h>

/**
 * base64_encode - Base64 encode
 * @param input_bytes Data in bytes to be encoded
 * @param input_len Length of the data to be encoded
 * @param out_len Pointer to output length variable, or %NULL if not used
 * Returns: Allocated buffer of out_len bytes of encoded data,
 * or %NULL on failure
 *
 * Caller is responsible for freeing the returned buffer. Returned buffer is
 * nul terminated to make it easier to use as a C string. The nul terminator is
 * not included in out_len.
 */
char* encode_bytes_to_base64_string(const unsigned char *input_bytes, const size_t input_len, size_t *out_len);
unsigned char* decode_base64_string_to_bytes(const char *src, size_t len, size_t *out_len);
            
#ifdef __cplusplus
}
#endif  // __cplusplus

#endif /* BASE64_H */
