SRCS = minishell.c

OBJS = ${SRCS:.c=.o}

NAME = minishell

LIBFT = libft/libft.a

LIBS = -L./libft -lft -lreadline


all : ${NAME}

${NAME} : ${OBJS} ${LIBFT}
	cc -Wall -Wextra  ${OBJS} ${LIBS} -o ${NAME}

${LIBFT}:
	cd libft && make

clean :
	rm -rf ${OBJS}
	cd libft && make clean

fclean : clean
	rm -rf ${NAME} libft/libft.a

re : fclean all

.PHONY : all clean fclean re