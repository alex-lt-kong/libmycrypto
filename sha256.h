#ifndef SHA256_H
#define SHA256_H


#ifdef __cplusplus
extern "C" {
#endif

#define SHA256_HASH_SIZE 32
#define SHA256_BLOCK_SIZE 64
#define SHA256_CHUNK_SIZE SHA256_BLOCK_SIZE * 1

typedef struct sha256_ctx {
	unsigned char *hash;
	unsigned char chunk[SHA256_CHUNK_SIZE];
	unsigned char *chunk_pos;
	size_t space_left;
	size_t total_len;
	uint32_t h[8];
} sha256_ctx;

/**
 * @brief The simple SHA-256 calculation function. 
 * @param input_bytes Pointer to the data the hash shall be calculated on.
 * @param input_len Length of the input_bytes data, in byte.
 * @param hash Hash array, where the result is delivered.
 *
 * @note If all of the data you are calculating the hash value on is available in a contiguous buffer in memory, this is
 * the function you should use.
 */
void cal_sha256_hash(const unsigned char* input_bytes, const size_t input_len, unsigned char hash[SHA256_HASH_SIZE]);

void sha256_init(sha256_ctx* sha_256, unsigned char hash[SHA256_HASH_SIZE]);

void sha256_update(sha256_ctx* sha_256, const unsigned char *data, size_t input_len);

void sha256_final(sha256_ctx* sha_256);

#ifdef __cplusplus
}
#endif

#endif