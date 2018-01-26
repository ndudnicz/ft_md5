#ifndef OPTIONS_H
# define OPTIONS_H

#include <sys/stat.h>

# define OPT_FILE		0x01
# define OPT_FILE_CHAR	'f'
# define PARAMS_STR		"f"

typedef struct	s_opt {
	unsigned char	options;
	char			*filename;
	struct stat		s;
}				t_opt;

int		get_options(t_opt *options, int *ac, char **av);

#endif /* OPTIONS_H */
