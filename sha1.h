#ifndef SHA1_H
#define SHA1_H



#ifdef __cplusplus
extern "C" {
#endif

#define SHA1_HASH_SIZE 20
#define SHA1_BLOCK_SIZE 64

typedef struct sha1_ctx 
{
    uint32_t state[5];
    uint32_t count[2];
    unsigned char buffer[64];
} sha1_ctx;



/**
 * @brief The simple SHA1 calculation function. 
 * @param input_bytes Pointer to the data the hash shall be calculated on.
 * @param input_len Length of the input data, in byte.
 * @param hash Hash array, where the result is delivered.
 *
 * @note If all of the data you are calculating the hash value on is available in a contiguous buffer in memory, this is
 * the function you should use.
 */
void cal_sha1_hash(const unsigned char* input_bytes, const size_t input_len, unsigned char hash[SHA1_HASH_SIZE]);

void sha1_init(sha1_ctx* ctx);

void sha1_update(sha1_ctx* ctx, const unsigned char* data, size_t input_len);

void sha1_final(unsigned char digest[20], sha1_ctx* ctx);

#ifdef __cplusplus
}
#endif

#endif