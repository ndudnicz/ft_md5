#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "options.h"
#include "map_file.h"
#include "read_input.h"
#include "hash.h"
#include "fit_data.h"

int
main(int ac, char **av) {
	t_opt	opt;
	uint8_t	*data;

	data = NULL;
	bzero(&opt, sizeof(t_opt));
	get_options(&opt, &ac, av);
	if (opt.options & OPT_FILE) {
		data = (uint8_t*)map_file(&opt);
		if (data == NULL) {
			return (1);
		}
	} else {
		data = read_input();
		if (data == NULL) {
			data = (uint8_t*)calloc(1, 1);
			opt.size = 0;
		} else {
			opt.size = (uint32_t)strlen((char const*)data);
		}
	}
	data = fit_data(data, &opt);
	return (hash_this(data, &opt));
}
