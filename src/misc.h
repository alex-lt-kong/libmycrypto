#ifndef CRYPTO_UTILS_H
#define CRYPTO_UTILS_H

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

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
uint32_t rotate(const uint32_t value, const unsigned int bits, const bool is_left);

/**
 * @brief Check if the current system is in big or little endian
 * @returns If the system is in big endian
 */
bool is_big_endian();

/**
 * @brief Switch the endianness of a given uint32_t variable
 * @param val pointer to the uint32_t variable whose endianness will be switched
 */
void switch_endianness(uint32_t* val);

/**
 * @brief Convert null-terminated hexadecimal string to a byte array
 * @param input_chars Pointer to a null-terminated hexadecimal string
 * @param output_len: Pointer to integer where the length of the output_chars will saved to
 * @returns Pointer a byte array converted from input_chars. Users are reminded to free() the pointer after use.
 */
uint8_t* hex_string_to_bytes(const char* input_chars, size_t* output_len);

/**
 * @brief Convert a byte array to a null-terminated hexadecimal string.
 * @param input_bytes Pointer to an array of bytes
 * @param input_len Length of input_bytes
 * @param upper Hexadecimal digits should be in uppercase
 * @returns Pointer to the null-terminated hexadecimal string. Users are reminded to free() the pointer after use.
 */
char* bytes_to_hex_string(const uint8_t* input_bytes, const size_t input_len, const bool upper);

#ifdef __cplusplus
}
#endif
#endif