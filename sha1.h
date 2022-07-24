#ifndef SHA1_H
#define SHA1_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define SHA1_HASH_SIZE 20
#define SHA1_CHUNK_SIZE 64      //size in byte
#define SHA1_CHUNK_SIZE_BIT 512 // size in bit

/**
 * @brief Calculate the SHA256 hash value from a given byte array
 * @param input_bytes Pointer to the data the hash shall be calculated on.
 * @param input_len Length of the input data, in byte.
 * @param hash Preallocated 20-byte long array, where the result is delivered. The array will be unchanged if errors occur
 */
void cal_sha1_hash(const uint8_t* input_bytes, const size_t input_len, uint8_t hash[SHA1_HASH_SIZE]);

#ifdef __cplusplus
}
#endif

#endif
