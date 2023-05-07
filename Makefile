SRCS = minishell.c \
		lib/expander.c \
		lib/tokenizer.c \
		lib/signals.c \
		lib/string_utils.c

OBJS = ${SRCS:.c=.o}

NAME = minishell

LIBFT = libft/libft.a

LIBS = -L./libft -lft -lreadline


all : ${NAME}

${NAME} : ${OBJS} ${LIBFT}
	gcc -Wall -Wextra -g3 -fsanitize=address,leak -fno-omit-frame-pointer ${OBJS} ${LIBS} -o ${NAME}

${LIBFT}:
	cd libft && make

clean :
	rm -rf ${OBJS}
	cd libft && make clean

fclean : clean
	rm -rf ${NAME} libft/libft.a

re : fclean all

.PHONY : all clean fclean re