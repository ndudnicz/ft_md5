NAME = ft_md5

CC = gcc

FLAGS = -Wextra -Wall #-Werror

PATH_SRC = src

PATH_OBJ = obj

PATH_INCLUDES = includes/

SRC =	main.c \
		error.c \
		options.c \
		read_input.c \
		map_file.c

OBJ = $(SRC:%.c=obj/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	make -C libft
	$(CC) $(FLAGS) -o $@ $(OBJ) -L. -lft -L. -lftasm

$(PATH_OBJ)/%.o: $(PATH_SRC)/%.c
	$(CC) $(FLAGS) -o $@ -c $< -I includes -I libft/includes

clean:
	make clean -C libft
	$(RM) $(OBJ)

fclean: clean
	make fclean -C libft
	$(RM) $(NAME) libft_malloc.so

re: clean fclean all
