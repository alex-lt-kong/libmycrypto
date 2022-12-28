#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "misc.h"

uint32_t rotate(const uint32_t value, const unsigned int bits, const bool is_left) {
    if (is_left) { return value << bits | value >> (32 - bits);  }
    else { return value >> bits | value << (32 - bits); }
}

bool is_big_endian() {
    volatile uint32_t i=0x01234567;    
    return (*((uint8_t*)(&i))) != 0x67;
}

uint8_t char_to_hex (const char c) {
    if ('0' <= c && c <= '9') return (uint8_t)(c - '0');
    if ('A' <= c && c <= 'F') return (uint8_t)(c - 'A' + 10);
    if ('a' <= c && c <= 'f') return (uint8_t)(c - 'a' + 10);
    return 0xFF;
}

uint8_t* hex_string_to_bytes(const char* input_chars, size_t* output_len) {
    if (strlen(input_chars) % 2 != 0) { return NULL; }

    *output_len = strlen(input_chars) / 2;
    uint8_t* output_bytes = (uint8_t*)calloc(*output_len, sizeof(uint8_t));
    if (output_bytes == NULL) { return NULL; }
    uint8_t msn, lsn, byte;
    int out_idx = 0;
    while (*input_chars) {
        msn = char_to_hex(*input_chars++);
        if (msn == 0xFF) { return NULL; }
        lsn = char_to_hex(*input_chars++);
        if (lsn == 0xFF) { return NULL; }
        byte = (msn << 4) + lsn;

        *(output_bytes + out_idx++) = byte;
    }
    return output_bytes;
}

char* bytes_to_hex_string(const uint8_t* input_bytes, const size_t input_len, const bool upper) {  
    char hex_table[] = "0123456789abcdef";
    if (upper) {
        for (int i = 10; i < 16; ++i) {  hex_table[i] -= 32; }
    }
    char* output_chars = (char*)calloc(input_len * 2 + 1, sizeof(char));
    if (output_chars == NULL) {
        fprintf(stderr, "calloc() failed\n");
        return NULL;
    }
    int out_idx = 0;

    uint8_t in_idx;
    for (size_t i = 0; i < input_len; ++i) {
        in_idx = *(input_bytes++);

        output_chars[out_idx++] = hex_table[(in_idx >> 4) & 0x0F];
        output_chars[out_idx++] = hex_table[in_idx & 0x0F];
    }
    return output_chars;
}

void switch_endianness(uint32_t* val) {
    *val = (*val & 0xff000000) >> 24 | (*val & 0x00ff0000) >> 8 | (*val & 0x0000ff00) << 8 | (*val & 0x000000ff) << 24;
}
