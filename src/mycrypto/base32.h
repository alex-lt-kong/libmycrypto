#ifndef LMC_BASE32_H
#define LMC_BASE32_H

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Encode a byte array into a printable and null-terminated base32 char
 * array
 * @param input_bytes Pointer to an array of bytes
 * @param input_len Length of input_bytes
 * @returns Pointer to a null-terminated char array or NULL if fails to allocate
 * enough memory, users are reminded to free() the pointer after use.
 */
char *encode_bytes_to_base32_string(const unsigned char *input_bytes,
                                    const size_t input_len);

/**
 * @brief Decode a null-terminated base32-encoded char array into original bytes
 * @param input_chars Pointer to an array of base32-encoded chars
 * @param output_len Pointer to the length of the output byte array. On error:
 * -1 if input_chars is not a valid base32 string.
 * -2 if malloc() failed.
 * @returns Pointer to a byte array or NULL (if error occurs), users are
 * reminded to free() the pointer after use. Note that the byte array returned
 * is NOT guaranteed to be null-terminated.
 */
unsigned char *decode_base32_string_to_bytes(const char *input_chars,
                                             ssize_t *output_len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* LMC_BASE32_H */
