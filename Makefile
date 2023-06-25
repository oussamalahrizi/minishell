SRCS = 	minishell.c \
		lib/signals.c \
		lib/expander/expander.c \
		lib/expander/expander_helpers.c \
		lib/expander/expander_helpers2.c \
		lib/expander/handle_dollar_alone.c \
		lib/expander/handle_dollar_quotes.c \
		lib/tokenizer/tokenizer.c \
		lib/tokenizer/tokenize_helpers.c \
		lib/tokenizer/tokenize_helpers2.c \
		lib/tokenizer/word_count.c \
		lib/tokenizer/wc_utils.c \
		lib/tokenizer/delimiter_section.c \
		lib/extract/extract.c \
		lib/extract/extract_helpers.c \
		lib/extract/extract_helpers2.c \
		lib/extract/allocate_files.c \
		lib/other_utils/utils.c \
		lib/other_utils/utils2.c \
		lib/other_utils/env_var_utils.c \
		lib/other_utils/env_var_utils2.c \
		lib/other_utils/ifs.c \
		lib/other_utils/string_utils.c \
		lib/other_utils/free_utils.c \
		exec/exec.c \
		exec/builtins/error.c \
		exec/builtins/build_exit.c \
		exec/builtins/build_pwd.c \
		exec/builtins/build_echo.c \
		exec/builtins/build_cd.c \
		exec/builtins/export.c \
		exec/builtins/build_env.c \
		exec/builtins/build_unset.c \
		exec/builtins/utils.c \
		exec/open_files/open_files.c \
		exec/open_files/open_files_helpers.c \
		exec/here_doc/here_doc.c \
		exec/here_doc/here_doc_helpers.c \
		exec/here_doc/here_doc_helpers2.c \
		exec/childprocess.c

OBJS = ${SRCS:.c=.o}

NAME = minishell

LIBFT = libft/libft.a

LIBS = -L./libft -lft -lreadline -L/Users/olahrizi/goinfre/homebrew/Cellar/readline/8.2.1/lib

CC = cc

CFLAGS = -Wall -Wextra -Werror -g3 ${sanitize} -I/Users/olahrizi/goinfre/homebrew/Cellar/readline/8.2.1/include

# sanitize= -fsanitize=address

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
