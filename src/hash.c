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

int
hash_this(uint8_t *const data, t_opt *const options) {
	uint32_t	g, f;
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
	int32_t		r[64] = {
		7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
		5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
		4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
		6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
	};
	for (int32_t o = 0; o < 64; o++) {k[o] = floor((fabs(sin(o+1))) * 4294967296);}
	for (uint64_t offset = 0; offset < options->new_size / 4; offset += 16) {
		for (uint32_t i = 0; i < 16; ++i) {
			w[i] = ((uint32_t*)data)[offset + i];
		}
		// memcpy(w, &data[offset], 64);
		for (int32_t i = 0; i < 64; i++) {
			if (i < 16) {
				f = (b & c) | (~b & d);
				g = i;
			} else if (i < 32) {
				f = (d & b) | (~d & c);
				g = ((5 * i) + 1) % 16;
			} else if (i < 48) {
				f = b ^ c ^ d;
				g = ((3 * i) + 5) % 16;
			} else {
				f = c ^ (b | ~d);
				g = (7 * i) % 16;
			}
			tmp = d;
			d = c;
			c = b;
			b = LEFTROTATE(a + f + k[i] + w[g], r[i]) + b;
			a = tmp;
		}
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
