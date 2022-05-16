/*
    hmac_sha256.h
    Originally written by https://github.com/h5p9sl
*/

#ifndef _HMAC_SHA256_H_
#define _HMAC_SHA256_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include <stddef.h>
#include <assert.h>
#include <limits.h>

static_assert (CHAR_BIT == 8);

/*
 * @brief The simple HMAC-SHA256 calculation function.
 * @param key key used to generate HMAC-SHA256 checksum
 * @param keylen the length of the key. Should be at least 32 bytes long for optimal security.
 * @param data The data to hash alongside the key.
 * @param datalen Length of the input data, in byte.
 * @param out Hash array, where the result is delivered.
 */
void hmac_sha256(
    const void* key, const size_t keylen, const void* data, const size_t datalen, void* out
);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // _HMAC_SHA256_H_