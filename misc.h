#ifndef CRYPTO_UTILS_H
#define CRYPTO_UTILS_H

#include <assert.h>
#include <limits.h>
#include <string>

static_assert (CHAR_BIT == 8);

using namespace std;
/*
 * @brief Convert a bytes array to a hex std::string
 * @param input A pointer to an array of bytes.
 * @param input_len length of input
 */
string bytes_to_hex_string(const unsigned char* input, const size_t input_len);
/*
 * @brief Convert a hex std::string to an array of bytes
 * @param hex_input A std:string containing the hex-encoded string.
 * @param output: a pointer to an array of bytes converted from hex_input
 */
void hex_string_to_bytes(const string& hex_input, unsigned char* output);

#endif