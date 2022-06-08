#include <stdint.h>
#include <stdlib.h>
#include "misc.h"


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

int hex_string_to_bytes(const char* s, unsigned char * buff, int length)
{
    int result;
    if (!s || !buff || length <= 0) return -1;

    for (result = 0; *s; ++result)
    {
        unsigned char msn = HexChar(*s++);
        if (msn == 0xFF) return -1;
        unsigned char lsn = HexChar(*s++);
        if (lsn == 0xFF) return -1;
        unsigned char bin = (msn << 4) + lsn;

        if (length-- <= 0) return -1;
        *buff++ = bin;
    }
    return result;
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