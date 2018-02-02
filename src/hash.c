#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <stdint.h>

#include "options.h"

/*
** https://en.wikipedia.org/wiki/MD5
*/

#define SWAP(X) (X << 24 | ((X & 0x0000ff00) << 8) | ((X & 0x00ff0000) >> 8) | X >> 24)
#define LEFTROTATE(X,Y) ((X) << (Y) | (X) >> (32 - (Y)))
static int32_t r[64] = {
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

#define STEP0_15(tmp, i, f, g, a, b, c, d, k, w, r) do { \
	f = (b & c) | (~b & d); \
	g = i; \
	tmp = d; \
	d = c; \
	c = b; \
	b = LEFTROTATE(a + f + k[i] + w[g], r[i]) + b; \
	a = tmp; \
} while (0);

#define STEP16_31(tmp, i, f, g, a, b, c, d, k, w, r) do { \
	f = (d & b) | (~d & c); \
	g = ((5 * i) + 1) & 0x10; \
	tmp = d; \
	d = c; \
	c = b; \
	b = LEFTROTATE(a + f + k[i] + w[g], r[i]) + b; \
	a = tmp; \
} while (0);

#define STEP32_47(tmp, i, f, g, a, b, c, d, k, w, r) do { \
	f = b ^ c ^ d; \
	g = ((3 * i) + 5) & 0x10; \
	tmp = d; \
	d = c; \
	c = b; \
	b = LEFTROTATE(a + f + k[i] + w[g], r[i]) + b; \
	a = tmp; \
} while (0);

#define STEP48_63(tmp, i, f, g, a, b, c, d, k, w, r) do { \
	f = c ^ (b | ~d); \
	g = (7 * i) & 0x10; \
	tmp = d; \
	d = c; \
	c = b; \
	b = LEFTROTATE(a + f + k[i] + w[g], r[i]) + b; \
	a = tmp; \
} while (0);

int
hash_this(uint8_t *const data, t_opt *const options) {
	uint32_t	g = 0, f = 0;
	uint32_t 	h0 = 0x67452301;
	uint32_t 	h1 = 0xEFCDAB89;
	uint32_t 	h2 = 0x98BADCFE;
	uint32_t 	h3 = 0x10325476;
	uint32_t	k[64];
	uint32_t	a = h0;
	uint32_t	b = h1;
	uint32_t	c = h2;
	uint32_t	d = h3;
	uint32_t	tmp = 0;
	uint32_t	w[16];
	for (int32_t o = 0; o < 64; o++) {k[o] = floor((fabs(sin(o+1))) * 4294967296);}
	for (uint64_t offset = 0; offset < options->new_size / 4; offset += 16) {
		for (uint32_t i = 0; i < 16; ++i) {
			w[i] = ((uint32_t*)data)[offset + i];
		}
		// memcpy(w, &data[offset], 64);
		// for (int32_t i = 0; i < 64; i++) {
		// 	if (i < 16) {
		// 		// f = (b & c) | (~b & d);
		// 		// g = i;
		// 	} else if (i < 32) {
		// 		// f = (d & b) | (~d & c);
		// 		// g = ((5 * i) + 1) & 0x10;
		// 	} else if (i < 48) {
		// 		// f = b ^ c ^ d;
		// 		// g = ((3 * i) + 5) & 0x10;
		// 	} else {
		// 		// f = c ^ (b | ~d);
		// 		// g = (7 * i) & 0x10;
		// 	}
		// 	tmp = d;
		// 	d = c;
		// 	c = b;
		// 	b = LEFTROTATE(a + f + k[i] + w[g], r[i]) + b;
		// 	a = tmp;
		// }
		STEP0_15(tmp, 0, f, g, b, a, c, d, k, w, r)
		STEP0_15(tmp, 1, f, g, b,a, c, d, k, w, r)
		STEP0_15(tmp, 2, f, g, b,a, c, d, k, w, r)
		STEP0_15(tmp, 3, f, g, b,a, c, d, k, w, r)
		STEP0_15(tmp, 4, f, g, b,a, c, d, k, w, r)
		STEP0_15(tmp, 5, f, g, b,a, c, d, k, w, r)
		STEP0_15(tmp, 6, f, g, b,a, c, d, k, w, r)
		STEP0_15(tmp, 7, f, g, b,a, c, d, k, w, r)
		STEP0_15(tmp, 8, f, g, b,a, c, d, k, w, r)
		STEP0_15(tmp, 9, f, g, b,a, c, d, k, w, r)
		STEP0_15(tmp, 10, f, g, a, b, c, d, k, w, r)
		STEP0_15(tmp, 11, f, g, a, b, c, d, k, w, r)
		STEP0_15(tmp, 12, f, g, a, b, c, d, k, w, r)
		STEP0_15(tmp, 13, f, g, a, b, c, d, k, w, r)
		STEP0_15(tmp, 14, f, g, a, b, c, d, k, w, r)
		STEP0_15(tmp, 15, f, g, a, b, c, d, k, w, r)

		STEP16_31(tmp, 16, f, g, a, b, c, d, k, w, r)
		STEP16_31(tmp, 17, f, g, a, b, c, d, k, w, r)
		STEP16_31(tmp, 18, f, g, a, b, c, d, k, w, r)
		STEP16_31(tmp, 19, f, g, a, b, c, d, k, w, r)
		STEP16_31(tmp, 20, f, g, a, b, c, d, k, w, r)
		STEP16_31(tmp, 21, f, g, a, b, c, d, k, w, r)
		STEP16_31(tmp, 22, f, g, a, b, c, d, k, w, r)
		STEP16_31(tmp, 23, f, g, a, b, c, d, k, w, r)
		STEP16_31(tmp, 24, f, g, a, b, c, d, k, w, r)
		STEP16_31(tmp, 25, f, g, a, b, c, d, k, w, r)
		STEP16_31(tmp, 26, f, g, a, b, c, d, k, w, r)
		STEP16_31(tmp, 27, f, g, a, b, c, d, k, w, r)
		STEP16_31(tmp, 28, f, g, a, b, c, d, k, w, r)
		STEP16_31(tmp, 29, f, g, a, b, c, d, k, w, r)
		STEP16_31(tmp, 30, f, g, a, b, c, d, k, w, r)
		STEP16_31(tmp, 31, f, g, a, b, c, d, k, w, r)

		STEP32_47(tmp, 32, f, g, a, b, c, d, k, w, r)
		STEP32_47(tmp, 33, f, g, a, b, c, d, k, w, r)
		STEP32_47(tmp, 34, f, g, a, b, c, d, k, w, r)
		STEP32_47(tmp, 35, f, g, a, b, c, d, k, w, r)
		STEP32_47(tmp, 36, f, g, a, b, c, d, k, w, r)
		STEP32_47(tmp, 37, f, g, a, b, c, d, k, w, r)
		STEP32_47(tmp, 38, f, g, a, b, c, d, k, w, r)
		STEP32_47(tmp, 39, f, g, a, b, c, d, k, w, r)
		STEP32_47(tmp, 40, f, g, a, b, c, d, k, w, r)
		STEP32_47(tmp, 41, f, g, a, b, c, d, k, w, r)
		STEP32_47(tmp, 42, f, g, a, b, c, d, k, w, r)
		STEP32_47(tmp, 43, f, g, a, b, c, d, k, w, r)
		STEP32_47(tmp, 44, f, g, a, b, c, d, k, w, r)
		STEP32_47(tmp, 45, f, g, a, b, c, d, k, w, r)
		STEP32_47(tmp, 46, f, g, a, b, c, d, k, w, r)
		STEP32_47(tmp, 47, f, g, a, b, c, d, k, w, r)

		STEP48_63(tmp, 48, f, g, a, b, c, d, k, w, r)
		STEP48_63(tmp, 49, f, g, a, b, c, d, k, w, r)
		STEP48_63(tmp, 50, f, g, a, b, c, d, k, w, r)
		STEP48_63(tmp, 51, f, g, a, b, c, d, k, w, r)
		STEP48_63(tmp, 52, f, g, a, b, c, d, k, w, r)
		STEP48_63(tmp, 53, f, g, a, b, c, d, k, w, r)
		STEP48_63(tmp, 54, f, g, a, b, c, d, k, w, r)
		STEP48_63(tmp, 55, f, g, a, b, c, d, k, w, r)
		STEP48_63(tmp, 56, f, g, a, b, c, d, k, w, r)
		STEP48_63(tmp, 57, f, g, a, b, c, d, k, w, r)
		STEP48_63(tmp, 58, f, g, a, b, c, d, k, w, r)
		STEP48_63(tmp, 59, f, g, a, b, c, d, k, w, r)
		STEP48_63(tmp, 60, f, g, a, b, c, d, k, w, r)
		STEP48_63(tmp, 61, f, g, a, b, c, d, k, w, r)
		STEP48_63(tmp, 62, f, g, a, b, c, d, k, w, r)
		STEP48_63(tmp, 63, f, g, a, b, c, d, k, w, r)
		// tmp = d;
		// d = c;
		// c = b;
		// b = LEFTROTATE(a + f + k[i] + w[g], r[i]) + b;
		// a = tmp;
		h0 += a;
		h1 += b;
		h2 += c;
		h3 += d;
		a = h0;
		b = h1;
		c = h2;
		d = h3;
	}
	if (options->options & OPT_FILE) {
		printf("MD5 (%s) = %08x%08x%08x%08x\n", options->filename, SWAP(h0), SWAP(h1), SWAP(h2), SWAP(h3));
		munmap((void*)data, options->size);
	}
	else {
		printf("%08x%08x%08x%08x\n", SWAP(h0), SWAP(h1), SWAP(h2), SWAP(h3));
		free((void*)data);
	}
	return (0);
}
