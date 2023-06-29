#ifndef SHA1_H
#define SHA1_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <sys/types.h>

#define SHA1_HASH_SIZE 20
#define SHA1_CHUNK_SIZE 64      // size in byte
#define SHA1_CHUNK_SIZE_BIT 512 // size in bit

/**
 * @brief Calculate the SHA256 hash value from a given byte array
 * @param input_bytes Pointer to the data the hash shall be calculated on.
 * @param input_len Length of the input data, in byte.
 * @param hash Preallocated 20-byte long array, where the result in bytes is
 * delivered. The array will be unchanged if errors occur. Note that the result
 * is an array of bytes, not a string of hex characters and it is NOT
 * nul-terminated.
 * @returns 0 on success and -1 on error
 */
int cal_sha1_hash(const unsigned char *input_bytes, const size_t input_len,
                  unsigned char hash[SHA1_HASH_SIZE]);

#ifdef __cplusplus
}
#endif

#endif
