#ifndef BASE58_H
#define BASE58_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// extern bool (*b58_sha256_impl)(void *, const void *, size_t);

// extern bool b58tobin(void *bin, size_t *binsz, const char *b58, size_t
// b58sz); extern int b58check(const void *bin, size_t binsz, const char *b58,
// size_t b58sz);

/**
 * @brief Empty
 * @param input_bytes the input bytes
 * @param input_len the length of the input bytes
 * @returns the encoded base58 string or NULL if fails to allocate enough memory
 * (users are reminded to free() the pointer after use)
 */
char *encode_bytes_to_base58_string(const unsigned char *input_bytes,
                                    const size_t input_len);

#ifdef __cplusplus
}
#endif

#endif
