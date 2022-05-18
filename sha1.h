#ifndef SHA1_H
#define SHA1_H

/*
   SHA-1 in C
   By Steve Reid <steve@edmweb.com>
   100% Public Domain
 */

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SHA1_HASH_SIZE 20
#define SHA1_BLOCK_SIZE 64

typedef struct
{
    uint32_t state[5];
    uint32_t count[2];
    unsigned char buffer[64];
} SHA1_CTX;

void sha1_transform(uint32_t state[5], const unsigned char buffer[64] );

void sha1_init(SHA1_CTX * context);

void SHA1Update(SHA1_CTX * context, const unsigned char *data, size_t len);

void sha1_final(unsigned char digest[20], SHA1_CTX * context);

/*
 * @brief The simple SHA1 calculation function.
 * @param hash Hash array, where the result is delivered.
 * @param input Pointer to the data the hash shall be calculated on.
 * @param len Length of the input data, in byte.
 *
 * @note If all of the data you are calculating the hash value on is available in a contiguous buffer in memory, this is
 * the function you should use.
 */
void cal_sha1_hash(unsigned char* hash, const unsigned char* str, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* SHA1_H */