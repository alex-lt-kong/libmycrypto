#include <stdint.h>
#include <stdlib.h>
#include "misc.h"

/**
 * @brief Rotate a 32-bit value by a number of bits to the right.
 * @param value The value to be rotated.
 * @param bits The number of bits to rotate by.
 * @param is_left Should the ratote be to left or to right
 * @returns The rotated value.
 */
uint32_t rotate(const uint32_t value, const unsigned int bits, const bool is_left) {
    if (is_left) { return value << bits | value >> (32 - bits);  }
    else { return value >> bits | value << (32 - bits); }
}

bool is_big_endian() {
    volatile uint32_t i=0x01234567;    
    return (*((uint8_t*)(&i))) != 0x67;
}

unsigned char HexChar (char c)
{
    if ('0' <= c && c <= '9') return (unsigned char)(c - '0');
    if ('A' <= c && c <= 'F') return (unsigned char)(c - 'A' + 10);
    if ('a' <= c && c <= 'f') return (unsigned char)(c - 'a' + 10);
    return 0xFF;
}

unsigned char* hex_string_to_bytes(const char* input_chars, size_t* output_len) {
    if (strlen(input_chars) % 2 != 0) { return NULL; }

    *output_len = strlen(input_chars) / 2;
    unsigned char* output_bytes = (unsigned char*)calloc(*output_len, sizeof(unsigned char));
    unsigned char msn, lsn, byte;
    int out_idx = 0;
    while (*input_chars) {
        msn = HexChar(*input_chars++);
        if (msn == 0xFF) { return NULL; }
        lsn = HexChar(*input_chars++);
        if (lsn == 0xFF) { return NULL; }
        byte = (msn << 4) + lsn;

        *(output_bytes + out_idx++) = byte;
    }
    return output_bytes;
}

char* bytes_to_hex_string(const unsigned char* input_bytes, const size_t input_len, bool upper) {  
    char hex_table[] = "0123456789abcdef";
    if (upper) {
        for (int i = 10; i < 16; ++i) {  hex_table[i] -= 32; }
    }
    char* output_chars = (char*)calloc(input_len * 2 + 1, sizeof(char));
    int out_idx = 0;

    unsigned char in_idx;
    for (int i = 0; i < input_len; ++i) {
        in_idx = *(input_bytes++);

        output_chars[out_idx++] = hex_table[(in_idx >> 4) & 0x0F];
        output_chars[out_idx++] = hex_table[in_idx & 0x0F];
    }
    return output_chars;
}

void switch_endianness(uint32_t* val) {
    *val = (*val & 0xff000000) >> 24 | (*val & 0x00ff0000) >> 8 | (*val & 0x0000ff00) << 8 | (*val & 0x000000ff) << 24;
}