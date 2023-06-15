#ifndef MINISHELL_H

# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <termios.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/wait.h>
# include "libft/libft.h"


typedef struct s_token{
	char type;
	char *value;
}	Token;

typedef struct s_files
{
	char * filename;
	char type;
	char *del;
	int fd;
	struct s_files *next;
} files;

typedef struct
{
	char *cmd;
	char **cmd_args;
	files *files;
}	Command;

typedef struct s_env
{
	char *name;
	char *value;
	int flag;
	struct s_env *next;
}	t_env;

typedef struct s_vars
{
	t_env *env;
	Command **commands;
}	t_vars;


int	expander(Token ***tokens_i, t_env *env);
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
void free_double_char(char **str);
t_env *copy_env(char **env);
void increase_shell_lvl(t_env *env);
char **get_name_value(char *str);
void	env_add_back(t_env **node, t_env *data);
void	error_cmd(char *str, int code);
int count_args(char **cmd_args);
void exec(t_vars *vars);
void	build_exit(char **cmd_args);
void build_cd(char **cmd, t_env *env);
void build_pwd();
void build_echo(char **cmd_args);
void child_process(Command *command, int *fd, t_env *env, int nbr_cmds);
Token* new_token(char type, char* value);
Token **duplicate_tokens(Token **tokens);
int get_tokens_size(Token **tokens);
void build_export(char **cmd ,t_env *env);
int get_env_flag(char *cmd);
void build_env(t_env *env);
void	build_unset(char **cmd, t_env **env);
files *get_last_infile(files *cmd_files);
files *get_last_outfile(files *cmd_files);
char	*get_env(char *string, t_env *env);

#endif // !MINISHELL_H



