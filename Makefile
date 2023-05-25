SRCS = minishell.c \
		lib/expander.c \
		lib/tokenizer.c \
		lib/signals.c \
		lib/string_utils.c \
		lib/extract.c

OBJS = ${SRCS:.c=.o}

NAME = minishell

LIBFT = libft/libft.a

LIBS = -L./libft -lft -lreadline -L./readline/8.2.1/lib

CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3 -Ireadline/8.2.1/include

all : ${NAME}

%.o : %.c
	@${CC} ${CFLAGS} -c $< -o $@

${NAME} : ${OBJS} ${LIBFT}
	@${CC} ${CFLAGS} ${OBJS} ${LIBS} -o ${NAME}

${LIBFT}:
	@make -C libft

clean :
	@rm -rf ${OBJS}
	@make clean -C libft

fclean : clean
	@rm -rf ${NAME} libft/libft.a

re : fclean all

.PHONY : all clean fclean re