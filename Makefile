SRCS = minishell.c \
		lib/expander.c \
		lib/tokenizer.c \
		lib/signals.c \
		lib/string_utils.c \
		lib/extract.c

OBJS = ${SRCS:.c=.o}

NAME = minishell

LIBFT = libft/libft.a

LIBS = -L./libft -lft -lreadline


all : ${NAME}

%.o : %.c
	@gcc -Wall -Wextra -g3 -c $< -o $@

${NAME} : ${OBJS} ${LIBFT}
	@gcc -Wall -Wextra -g3 -fsanitize=address,leak -fno-omit-frame-pointer ${OBJS} ${LIBS} -o ${NAME}

${LIBFT}:
	@make -C libft

clean :
	@rm -rf ${OBJS}
	@make clean -C libft

fclean : clean
	@rm -rf ${NAME} libft/libft.a

re : fclean all

.PHONY : all clean fclean re