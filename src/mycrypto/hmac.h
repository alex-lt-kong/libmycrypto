#ifndef LMC_HMAC_H
#define LMC_HMAC_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

typedef int hash_func(const unsigned char *, size_t, unsigned char[]);

/**
 * @brief The simple HMAC-SHA1 calculation function.
 * @param key key used to generate HMAC-SHA256 checksum
 * @param key_len the length of the key. Should be at least 32 bytes long for
 * optimal security.
 * @param data The data to hash alongside the key.
 * @param data_len Length of the input data, in byte.
 * @param out Pre-allocated bytes array, where the hash is delivered. Its length
 * is always SHA1_HASH_SIZE. Note that the result is an array of bytes,
 * not a string of hex characters and it is NOT nul-terminated. If you would
 * like to get the hash value in hex string, call bytes_to_hex_string()
 * declared misc.h.
 */
void hmac_sha1(const unsigned char *key, const size_t key_len,
               const unsigned char *data, const size_t data_len,
               unsigned char *out);

/**
 * @brief The simple HMAC-SHA256 calculation function.
 * @param key key used to generate HMAC-SHA256 checksum
 * @param key_len the length of the key. Should be at least 32 bytes long for
 * optimal security.
 * @param data The data to hash alongside the key.
 * @param data_len Length of the input data, in byte.
 * @param out Pre-allocated bytes array, where the hash is delivered. Its length
 * is always SHA256_HASH_SIZE. Note that the result is an array of bytes,
 * not a string of hex characters and it is NOT nul-terminated. If you would
 * like to get the hash value in hex string, call bytes_to_hex_string()
 * declared misc.h.
 */
void hmac_sha256(const unsigned char *key, const size_t key_len,
                 const unsigned char *data, const size_t data_len,
                 unsigned char *out);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // LMC_HMAC_H
