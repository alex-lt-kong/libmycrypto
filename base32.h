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

/*
 * @brief Encode bytes into a base32 string
 * @param input_bytes pointer to an array of input bytes
 * @param input_len length of input_bytes
 * @param output_len pointer to the length of output string
 * @returns pointer to a base32 string representing input_bytes
 */
char* encode_bytes_to_base32_string(const unsigned char* input_bytes , size_t input_len, size_t* output_len);

/**
 * Dencode `char *' source with `size_t' size.
 * Returns a `unsigned char *' base32 decoded string + size of decoded string.
 */
unsigned char* decode_base32_string_to_bytes (const char * input_chars, size_t input_len, size_t *);

#ifdef __cplusplus
}
#endif

#endif