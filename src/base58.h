#ifndef LIBBASE58_H
#define LIBBASE58_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

//extern bool (*b58_sha256_impl)(void *, const void *, size_t);

//extern bool b58tobin(void *bin, size_t *binsz, const char *b58, size_t b58sz);
//extern int b58check(const void *bin, size_t binsz, const char *b58, size_t b58sz);

/*
 * @brief Empty
 * @param input_bytes the input bytes
 * @param input_len the length of the input bytes
 * @param output_char pointer to the base58 string
 * @param output_len pointer to the size of the output base58 string 
 */
extern bool b58enc(
  const uint8_t *input_bytes, const size_t input_len, char *output_char, size_t *output_len
);
//extern bool b58check_enc(char *b58c, size_t *b58c_sz, uint8_t ver, const void *data, size_t datasz);

#ifdef __cplusplus
}
#endif

#endif
