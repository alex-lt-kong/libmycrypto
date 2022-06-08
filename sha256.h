#ifndef SHA256_H
#define SHA256_H

#include <string.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SHA256_HASH_SIZE 32

/**
 * @brief The simple SHA-256 calculation function. 
 * @param input_bytes Pointer to the data the hash shall be calculated on.
 * @param input_len Length of the input_bytes data, in byte.
 * @param hash Preallocated byte array, where the result is delivered.
 *
 * @note If all of the data you are calculating the hash value on is available in a contiguous buffer in memory, this is
 * the function you should use.
 */
void cal_sha256_hash(const unsigned char* input_bytes, const size_t input_len, unsigned char* hash);

#ifdef __cplusplus
}
#endif

#endif