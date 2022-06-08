#ifndef CRYPTO_UTILS_H
#define CRYPTO_UTILS_H

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>


static_assert (CHAR_BIT == 8);

#ifdef __cplusplus
extern "C" {
#endif

bool is_big_endian();

/**
 * @brief Switch the endianness of a given uint32_t variable
 * @param val pointer to the uint32_t variable whose endianness will be switched
 */
void switch_endianness(uint32_t* val);

int hex_string_to_bytes(const char* s, unsigned char * buff, int length);

/**
 * @brief Convert a bytes array to a null-terminated hexadecimal string.
 * @param input_bytes Pointer to an array of bytes
 * @param input_len Length of input_bytes
 * @param upper Hexadecimal digits be in uppercase
 * @returns Pointer to the null-terminated hexadecimal string. Users are reminded to free() the pointer after use.
 */
char* bytes_to_hex_string(const unsigned char* input_bytes, const size_t input_len, bool upper);

#ifdef __cplusplus
}
#endif
#endif