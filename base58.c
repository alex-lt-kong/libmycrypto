#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// From: https://github.com/bitcoin/libbase58/blob/master/base58.c

static const char b58digits_ordered[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

bool b58enc(char *b58, size_t *b58sz, const void *data, size_t binsz)
{
	const unsigned char *bin = data;
	int carry;
	size_t i, j, high, zcount = 0;
	size_t size;
	
	while (zcount < binsz && !bin[zcount])
		++zcount;
	
	size = (binsz - zcount) * 138 / 100 + 1;
  /*
  这个公式看起来有点让人摸不着头脑，但经过一些思考，我想明白了。

  在编码后的字符串中，每一个字符都是从58个字符中当中选择出来的。那么这58种不同的信息可以用多少bit来代替呢？显然答案是log(2)58。换句话说，每一个字母代表的信息量是log(2)58 bit。

  我们输入的字节流信息量是固定的：(byte_length * 8)bit

  那么，需要预留的字符数量就是（byte_length * 8）/ log258

  经过换算，

  (byte_length * 8)/ log258 =
  (byte_length * log2256) / log258 =
  byte_length * 1.38

  最后，还需要加向上取整

  最终结果就是 byte_length * 1.37 + 1
  */
	unsigned char buf[size];
	memset(buf, 0, size);
	
	for (i = zcount, high = size - 1; i < binsz; ++i, high = j)
	{
		for (carry = bin[i], j = size - 1; (j > high) || carry; --j)
		{
			carry += 256 * buf[j];
			buf[j] = carry % 58;
			carry /= 58;
			if (!j) {
				// Otherwise j wraps to maxint which is > high
				break;
			}
		}
	}
	
	for (j = 0; j < size && !buf[j]; ++j);
	
	if (*b58sz <= zcount + size - j)
	{
		*b58sz = zcount + size - j + 1;
    printf("False!");
		return false;
	}
	
	if (zcount)
		memset(b58, '1', zcount);
	for (i = zcount; j < size; ++i, ++j)
		b58[i] = b58digits_ordered[buf[j]];
	b58[i] = '\0';
	*b58sz = i + 1;
	
	return true;
}