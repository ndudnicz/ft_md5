#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/mman.h>
#include <stdint.h>

#include "options.h"

uint8_t*
fit_data(uint8_t const *data, t_opt *options) {
	// if (options->size % 64 == 0) {
	// 	return ((uint8_t*)data);
	// } else {
		uint8_t	*new_data = NULL;
		options->new_size = ((options->size / 64) + 1) * 64;

		new_data = (uint8_t*)calloc(1, sizeof(uint8_t) * options->new_size);
		memcpy((void*)new_data, (void*)data, (size_t)options->size);
		new_data[options->size] = 0x80;
		for (int32_t i = 1; i + options->size < options->new_size - 8; ++i) {
			new_data[i + options->size] = 0;
		}
		uint64_t	fill_size = options->size * 8 % ULLONG_MAX;
		new_data[options->new_size - 1] = (uint8_t)((fill_size & 0xff00000000000000) >> 56);
		new_data[options->new_size - 2] = (uint8_t)((fill_size & 0x00ff000000000000) >> 48);
		new_data[options->new_size - 3] = (uint8_t)((fill_size & 0x0000ff0000000000) >> 40);
		new_data[options->new_size - 4] = (uint8_t)((fill_size & 0x000000ff00000000) >> 32);
		new_data[options->new_size - 5] = (uint8_t)((fill_size & 0x00000000ff000000) >> 24);
		new_data[options->new_size - 6] = (uint8_t)((fill_size & 0x0000000000ff0000) >> 16);
		new_data[options->new_size - 7] = (uint8_t)((fill_size & 0x000000000000ff00) >> 8);
		new_data[options->new_size - 8] = (uint8_t)(fill_size & 0x00000000000000ff);
		if (options->options & OPT_FILE)
			munmap((void*)data, options->size);
		else
			free((void*)data);
		return (new_data);
	// }
}
