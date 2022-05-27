/**
 * `b32.h' - b32
 *
 * copyright (c) 2016 jay rama fisher
 * copyright (c) 2014 joseph werle
 */

#ifndef B32_H
#define B32_H 1



#ifdef __cplusplus
extern "C" {
#endif

/**
 * Encode `unsigned char *' source with `size_t' size.
 * Returns a `char *' base64 encoded string.
 */

char* encode_bytes_to_base32_string(const unsigned char *, size_t input_len, size_t* output_len);

/**
 * Dencode `char *' source with `size_t' size.
 * Returns a `unsigned char *' base32 decoded string.
 */
unsigned char *
b32_decode (const char *, size_t);

/**
 * Dencode `char *' source with `size_t' size.
 * Returns a `unsigned char *' base32 decoded string + size of decoded string.
 */
unsigned char *
b32_decode_ex (const char *, size_t, size_t *);

#ifdef __cplusplus
}
#endif

#endif