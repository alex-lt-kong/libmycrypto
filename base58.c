#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// From: https://github.com/bitcoin/libbase58/blob/master/base58.c

static const char b58_table[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

bool b58enc(const unsigned char *input_bytes, const size_t input_len, char *output_chars, size_t *output_len)
{
	
	size_t j, high, zcount = 0;
	size_t size;
	
	while (zcount < input_len && !input_bytes[zcount])
		++zcount;
	
	size = (input_len - zcount) * 138 / 100 + 1;
	/*
	 * How do we get the size in advance? We can consider it this way--In base58 encoding, we use 58 characters to encode
	 * bytes (or bits), how many bits can be represented by one character? It is log(2)58 ≈ 5.8579 bits.
	 * Is it possible for base58 character to present 6 bits? No, 2^6 = 64, that is to say, characters will be exhausted
	 * before 0b11 1111 can be encoded.
	 * Since 1 byte = 8 bits, there are (input_len * 8) bits to be represented. Therefore, we need:
	 * (input_len * 8) / log(2)58 = (input_len * log(2)256) / log(2)58 = input_len * log(58)256 ≈ input_len * 1.36565
	 * Then we round it up.
	 */
	unsigned char buf[size];
	memset(buf, 0, size);
	
	for (int i = zcount, high = size - 1; i < input_len; ++i, high = j) {
	  int carry = 0;
		printf("[New loop]");
		for (carry = input_bytes[i], j = size - 1; (j > high) || carry; --j) {
			printf("%d,", carry);
			carry += 256 * buf[j];
			buf[j] = carry % 58;
			carry /= 58;
			if (!j) {
				// Otherwise j wraps to maxint which is > high
				break;
			}
			printf("%d;", (j > high) || carry);
		}
	}
	
	for (j = 0; j < size && !buf[j]; ++j);
	
	if (*output_len <= zcount + size - j)
	{
		*output_len = zcount + size - j + 1;
    printf("False!");
		return false;
	}
	
	if (zcount)
		memset(output_chars, '1', zcount);
	int i = 0;
	for (i = zcount; j < size; ++i, ++j)
		output_chars[i] = b58_table[buf[j]];
	output_chars[i] = '\0';
	*output_len = i + 1;
	
	return true;
}