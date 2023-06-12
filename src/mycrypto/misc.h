#ifndef CRYPTO_UTILS_H
#define CRYPTO_UTILS_H

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Rotate a 32-bit value by a number of bits.
 * @param value The value to be rotated.
 * @param bits The number of bits to rotate by.
 * @param is_left Should the ratotation be to left or to right
 * @returns The rotated value.
 */
uint32_t rotate(const uint32_t value, uint32_t bits, const bool is_left);

/**
 * @brief Check if the current system is in big or little endian
 * @returns If the system is in big endian
 */
bool is_big_endian(void);

/**
 * @brief Switch the endianness of a given uint32_t variable
 * @param val pointer to the uint32_t variable whose endianness will be switched
 */
void switch_endianness(uint32_t *val);

/**
 * @brief Convert null-terminated hexadecimal string to a byte array
 * @param input_chars Pointer to a null-terminated hexadecimal string
 * @param output_len: Pointer to integer where the length of the output_chars
 * will be saved to. On error: -1 if input_chars' length is invalid (not a
 * multiple of 2). -2 if malloc() failed. -3 if input_chars' value is invalid
 * (not from '0' - 'e').
 * @returns Pointer to a byte array converted from input_chars or NULL on error
 * or if an empty string is passed. Users are reminded to free() the pointer
 * after use.
 */
uint8_t *hex_string_to_bytes(const char *input_chars, ssize_t *output_len);

/**
 * @brief Convert a byte array to a null-terminated hexadecimal string.
 * @param input_bytes Pointer to an array of bytes
 * @param input_len Length of input_bytes
 * @param upper Hexadecimal digits should be in uppercase
 * @returns Pointer to the null-terminated hexadecimal string or NULL on error.
 * As an empty null-termianted string is one-byte long (i.e., '\0'), it is not
 * possible for a successful call to return NULL. Users are reminded to free()
 * the pointer after use.
 */
char *bytes_to_hex_string(const uint8_t *input_bytes, const size_t input_len,
                          const bool upper);

#ifdef __cplusplus
}
#endif
#endif
