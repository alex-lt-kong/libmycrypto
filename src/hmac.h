#ifndef HMAC_SHA_H
#define HMAC_SHA_H

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include <stddef.h>
#include <assert.h>
#include <limits.h>
#include <stdint.h>

/**
 * @brief The simple HMAC-SHA1 calculation function.
 * @param key key used to generate HMAC-SHA256 checksum
 * @param key_len the length of the key. Should be at least 32 bytes long for optimal security.
 * @param data The data to hash alongside the key.
 * @param data_len Length of the input data, in byte.
 * @param out Hash array, where the result is delivered.
 */
void hmac_sha1(
    const uint8_t* key, const size_t key_len,
    const uint8_t* data, const size_t data_len,
    uint8_t* out
);

/**
 * @brief The simple HMAC-SHA256 calculation function.
 * @param key key used to generate HMAC-SHA256 checksum
 * @param key_len the length of the key. Should be at least 32 bytes long for optimal security.
 * @param data The data to hash alongside the key.
 * @param data_len Length of the input data, in byte.
 * @param out Hash array, where the result is delivered.
 */
void hmac_sha256(
    const uint8_t* key, const size_t key_len,
    const uint8_t* data, const size_t data_len,
    uint8_t* out
);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // HMAC_SHA_H