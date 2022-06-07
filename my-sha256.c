#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


unsigned char* sha256(const unsigned char* input_bytes, size_t input_len) {
    if (input_len > 2147483647) {
        return NULL; // we support up to this length only
    }

    unsigned int h[8] = {
        0x6a09e667,
        0xbb67ae85,
        0x3c6ef372,
        0xa54ff53a,
        0x510e527f,
        0x9b05688c,
        0x1f83d9ab,
        0x5be0cd19
    };
    unsigned int k[64] ={
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    /**
     * Pre-processing (Padding):
     * begin with the original message of length L bits
     * append a single '1' bit
     * append K '0' bits, where K is the minimum number >= 0 such that (L + 1 + K + 64) is a multiple of 512
     * append L as a 64-bit big-endian integer, making the total post-processed length a multiple of 512 bits
     * such that the bits in the message are: <original message of length L> 1 <K zeros> <L as 64 bit integer> , (the number of bits will be a multiple of 512)
     * 
     */
    size_t padded_len = input_len + (512 - input_len * 8 % 512) / 8;
    if (padded_len - input_len < 9) {
        padded_len += 64;
    }
    printf("input_len=%d, padded_len=%d\n", input_len, padded_len);

    unsigned char* padded_bytes = (unsigned char*)calloc(padded_len, sizeof(unsigned char));
    memcpy(padded_bytes, input_bytes, input_len);                                   // begin with the original message of length L bits    
    padded_bytes[input_len] = 0x80;                                                 // append a single '1' bit
    
    // append K '0' bits is not needed as we are using calloc();

    // append L as a 64-bit big-endian integer, making the total post-processed length a multiple of 512 bits
    padded_bytes[padded_len - 4] = (unsigned char)(input_len >> 24);
    padded_bytes[padded_len - 3] = (unsigned char)(input_len >> 16);
    padded_bytes[padded_len - 2] = (unsigned char)(input_len >>  8);
    padded_bytes[padded_len - 1] = (unsigned char)(input_len >>  0);
    
    for (int i = 0; i < padded_len; ++i) {
        printf("%02x", padded_bytes[i]);
    }

    free(padded_bytes);
    return NULL;
}

int main() {
    unsigned char input_bytes[] = "The quick brown fox jumps over the lazy dog. The quick ";
    // Useful ASCII encoding tool: https://www.dcode.fr/ascii-code
    sha256(input_bytes, strlen(input_bytes));
    return 0;
}