/**
 * A very straightforward and no-frills C implementaion of SHA1 based on the pseudocode at
 * https://en.wikipedia.org/wiki/SHA-1
 */

#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "sha1.h"
#include "misc.h"


void cal_sha1_hash(const uint8_t* input_bytes, const size_t input_len, uint8_t* hash) {
    if (input_len > 2147483647) {
        return; // we support up to this length only
    }
    
    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xEFCDAB89;
    uint32_t h2 = 0x98BADCFE;
    uint32_t h3 = 0x10325476;
    uint32_t h4 = 0xC3D2E1F0;
    
    /**
     * Pre-processing (Padding):
     * ml = message length in bits (always a multiple of the number of bits in a character).
     * begin with the original message of length L bits
     * append the bit '1' to the message e.g. by adding 0x80 if message length is a multiple of 8 bits.
     * append 0 ≤ k < 512 bits '0', such that the resulting message length in bits is congruent to −64 ≡ 448 (mod 512)
     * append ml, the original message length in bits, as a 64-bit big-endian integer.
     * Thus, the total length is a multiple of 512 bits.
     * 
     * The padding scheme is the same as that of SHA256.
     */
    size_t padded_len = input_len + (SHA1_CHUNK_SIZE_BIT - input_len * CHAR_BIT % SHA1_CHUNK_SIZE_BIT) / CHAR_BIT;
    if (padded_len - input_len < 9) {
        padded_len += 64;
    }

    uint8_t* padded_bytes = (uint8_t*)calloc(padded_len, sizeof(uint8_t));
    memcpy(padded_bytes, input_bytes, input_len);                                           // begin with the original message of length L bits    
    padded_bytes[input_len] = 0b10000000;                                                   // append a single '1' bit
    
    // append k '0' bits is not needed as we are using calloc();

    // append ml, the original message length in bits, as a 64-bit big-endian integer. Thus, the total length is a multiple of 512 bits.
    padded_bytes[padded_len - 4] = (uint8_t)(input_len * CHAR_BIT >> 24);
    padded_bytes[padded_len - 3] = (uint8_t)(input_len * CHAR_BIT >> 16);
    padded_bytes[padded_len - 2] = (uint8_t)(input_len * CHAR_BIT >>  8);
    padded_bytes[padded_len - 1] = (uint8_t)(input_len * CHAR_BIT >>  0);
    // such that the bits in the message are: <original message of length L> 1 <K zeros> <L as 64 bit integer> , (the number of bits will be a multiple of 512)

    const int chunk_count = padded_len / SHA1_CHUNK_SIZE;

    // Process the message in successive 512-bit chunks:
    for (int i = 0; i < chunk_count; ++i) {
        uint32_t w[16 + SHA1_CHUNK_SIZE];                                                    // break chunk into sixteen 32-bit big-endian words w[i], 0 ≤ i ≤ 15
        uint8_t* chunk_pos = padded_bytes + i * SHA1_CHUNK_SIZE;
        for (int j = 0; j < 16; ++j) {                                                       // (to handle endianness properly, we cant simply use memcpy()
			w[j] = (uint32_t)(*(chunk_pos++)) << 24 | (uint32_t)(*(chunk_pos++)) << 16 | (uint32_t)(*(chunk_pos++)) << 8 | (uint32_t)(*(chunk_pos++));
        }
        //  Message schedule: extend the sixteen 32-bit words into eighty 32-bit words:
        for (int j = 16; j < 16 + SHA1_CHUNK_SIZE; j++) {            
            w[j] = rotate(w[j-3] ^ w[j-8] ^ w[j-14] ^ w[j-16], 1, true);
        }
        // Initialize hash value for this chunk:
        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        uint32_t e = h4;

        uint32_t f;
        uint32_t k;
        
        // Compression function main loop:
        for (int j = 0; j < 16 + SHA1_CHUNK_SIZE; ++j) {
            if (j >= 0 && j <= 19) {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999;
            } else if (j >= 20 && j <= 39) {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            } else if (j >= 40 && j <= 59) {
                f = (b & c) | (b & d) | (c & d) ;
                k = 0x8F1BBCDC;
            } else if (j >= 60 && j <= 79) {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }
            // Add the compressed chunk to the current hash value:
            const uint32_t temp =  rotate(a, 5, true) + f + e + k + w[j];
            e = d;
            d = c;
            c = rotate(b, 30, true);
            b = a;
            a = temp;
            
        }
        // Add this chunk's hash to result so far:
        h0 = h0 + a;
        h1 = h1 + b;
        h2 = h2 + c;
        h3 = h3 + d;
        h4 = h4 + e;
    }
    h0 = rotate(h0, 128, true);
    h1 = rotate(h1,  96, true);
    h2 = rotate(h2,  64, true);
    h3 = rotate(h3,  32, true);
    h4 = rotate(h4,   0, true);
    free(padded_bytes);
    if (is_big_endian() == false) {
        switch_endianness(&h0);
        switch_endianness(&h1);
        switch_endianness(&h2);
        switch_endianness(&h3);
        switch_endianness(&h4);
    }
    memcpy(hash + 0 * sizeof(uint32_t), &h0, sizeof(uint32_t));
    memcpy(hash + 1 * sizeof(uint32_t), &h1, sizeof(uint32_t));
    memcpy(hash + 2 * sizeof(uint32_t), &h2, sizeof(uint32_t));
    memcpy(hash + 3 * sizeof(uint32_t), &h3, sizeof(uint32_t));
    memcpy(hash + 4 * sizeof(uint32_t), &h4, sizeof(uint32_t));
}