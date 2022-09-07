#ifndef SHA256_H
#define SHA256_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdint.h>

#define SHA256_HASH_SIZE 32
#define SHA256_CHUNK_SIZE 64      // size in byte
#define SHA256_CHUNK_SIZE_BIT 512 // size in bit

/**
 * @brief Calculate the SHA256 hash value from a given byte array
 * @param input_bytes Pointer to the data the hash shall be calculated on.
 * @param input_len Length of the input_bytes data, in byte.
 * @param hash Preallocated 32-byte long array, where the result is delivered.
 * The array will be unchanged if errors occur
 */
void cal_sha256_hash(const uint8_t* input_bytes, const size_t input_len, uint8_t* hash);

#ifdef __cplusplus
}
#endif

#endif