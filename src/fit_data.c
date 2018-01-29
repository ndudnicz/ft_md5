#include <stdlib.h>
#include <string.h>

#include "options.h"

char*
fit_data(char const *data, t_opt *options) {
	if (options->size % 64 == 0) {
		return ((char*)data);
	} else {
		char	*new_data = NULL;
		options->new_size = ((options->size / 64) + 1) * 64;

		new_data = (char*)malloc(sizeof(char) * options->new_size);
		memcpy((void*)new_data, (void*)data, (size_t)options->size);
		new_data[options->size] = 0xff;
		for (int32_t i = 1; i + options->size < options->new_size - 4; ++i) {
			new_data[i + options->size] = 0;
		}
		new_data[options->new_size - 1] = (uint8_t)(options->size >> 24);
		new_data[options->new_size - 2] = (uint8_t)((options->size & 0x00ff0000) >> 16);
		new_data[options->new_size - 3] = (uint8_t)((options->size & 0x0000ff00) >> 8);
		new_data[options->new_size - 4] = (uint8_t)(options->size & 0x000000ff);
		return (new_data);
	}
}
