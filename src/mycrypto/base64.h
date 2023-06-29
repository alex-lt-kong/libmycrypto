#ifndef BASE64_H
#define BASE64_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Encode a byte array into a printable and null-terminated base32 char
 * array
 * @param input_bytes Pointer to an array of bytes
 * @param input_len Length of input_bytes
 * @param chars_per_line wrap encoded lines. Use 0 to disable line wrapping. To
 * be RFC 4648-compliant, pass 76 Given how this function is designed, it has to
 * be a multiple of 4
 * @returns Pointer to a null-terminated char array or NULL if fails to allocate
 * enough memory, users are reminded to free() the pointer after use.
 */
char *encode_bytes_to_base64_string(const unsigned char *input_bytes,
                                    const size_t input_len,
                                    const size_t chars_per_line);

/**
 * @brief Decode a null-terminated base64-encoded char array into original bytes
 * @param input_chars Pointer to an array of base64-encoded chars
 * @param output_len Pointer to the length of the output byte array. On error:
 * -1 if input_chars is not a valid base64 string.
 * -2 if malloc() failed.
 * @returns Pointer to a byte array or NULL (if error occurs), users are
 * reminded to free() the pointer after use. Note that the byte array returned
 * is NOT guaranteed to be null-terminated. It is guaranteed that the pointer
 * is non NULL if output_len >= 0.
 */
unsigned char *decode_base64_string_to_bytes(const char *input_chars,
                                             int64_t *output_len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* BASE64_H */
