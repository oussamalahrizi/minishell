#ifndef MINISHELL_H

# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"

#define MAX_LENGTH 99999999999

typedef struct s_token{
	char type;
	char *value;
}	Token;

typedef struct s_files
{
	char * filename;
	char type;
	char *del;
	struct s_files *next;
} files;

typedef struct
{
	char *cmd;
	char **cmd_args;
	files *files;
}	Command;

int expander(Token **tokens, char **env);
void tokenize(char *input, Token **tokens);
int is_space(char c);
void free_double(Token **str);
int word_count(char *input);
void print_split(char **str);
void append_character(char **string, char c);
void signal_handler();
int error(char *str);
Command **extract(Token **tokens);
char **split_by_str(char *s, char *del);


#endif // !MINISHELL_H



