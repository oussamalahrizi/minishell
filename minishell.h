#ifndef MINISHELL_H

# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"

#define MAX_LENGTH 99999999999

void expander(char **tokens, char **env);
void tokenize(char *input, char **tokens);
int is_space(char c);
void free_double(char **str);
int word_count(char *input);
void print_split(char **str);
char *append_character(char **string, char c);
void signal_handler();
#endif // !MINISHELL_H



