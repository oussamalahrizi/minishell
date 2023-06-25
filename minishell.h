/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 22:21:23 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 12:30:03 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include "libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

typedef struct s_wc
{
	int				i;
	int				k;
	char			*delimiters;
}					t_wc;

typedef struct s_tok
{
	int				i;
	char			quote;
	char			type;
	char			*string;
	char			*delimiters;
	int				k;
}					t_tok;

typedef struct s_token
{
	char			type;
	char			*value;
}					t_token;

typedef struct s_dollar
{
	char			*skip;
	char			**split;
	t_token			**new_tokens;
	int				i;
	int				len;
	int				start_index;
	int				token_size;
	int				split_size;
	int				j;
	int				size;
	int				k_split;
	int				token_index;
}					t_dollar;

typedef struct s_env
{
	char			*name;
	char			*value;
	int				flag;
	struct s_env	*next;
}					t_env;

typedef struct s_exp
{
	t_env			*env;
	t_token			**tokens;
	int				i;
	int				j;
	char			*new_token;
	char			*string;
	int				new_index;
	t_token			**t_tokens;
	char			*temp;
}					t_exp;

typedef struct global
{
	int				exit_status;
	int				readline;
	int				child;
	int				heredoc;
}					t_global;

extern t_global		g_global;

typedef struct s_files
{
	char			*filename;
	char			type;
	char			*del;
	int				fd;
	struct s_files	*next;
	int				open;
	int				expand;
	int				here_doc_fd[2];
}					t_files;

typedef struct s_command
{
	char			*cmd;
	char			**cmd_args;
	t_files			*files;
}					t_command;

typedef struct s_vars
{
	t_env			*env;
	t_command		**commands;
}					t_vars;

typedef struct s_ext
{
	int				i;
	t_command		**commands;
	int				num;
	int				k;
}					t_ext;

typedef struct s_child
{
	char	**paths;
	char	**env_list;
	int		is_dir;
	char	*cmd_path;
	t_files	*infile;
	t_files	*outfile;
}					t_child;

typedef struct s_args
{
	int	nbr_cmds;
	int	fd_in;
	int	iterator;
	int	fd[2];
}				t_args;

typedef struct s_op_files
{
	int		i;
	t_files	*node;
	int		count;
	int		*new_fds;
	int		try;
	int		failure;
	char	*expanded;
}				t_op_files;

typedef struct s_mini
{
	t_token			**tokens;
	t_command		**commands;
	t_vars			vars;
	struct termios	term;
	struct termios	original;
	char			*input;
	int				size;
	char			*new_input;
}				t_mini;

void			expander(t_token ***tokens_i, t_env *env);
void			tokenize(char *input, t_token **tokens);
int				is_space(char c);
void			free_double(t_token **str);
int				word_count(char *input);
void			append_character(char **string, char c);
void			signal_handler(void);
t_command		**extract(t_token **tokens, t_env *env);
char			**split_by_str(char *s, char *del);
void			free_double_char(char **str);
t_env			*copy_env(char **env);
void			increase_shell_lvl(t_env *env);
char			**get_name_value(char *str);
void			env_add_back(t_env **node, t_env *data);
void			error_cmd(char *str, int code);
int				count_args(char **cmd_args);
void			exec(t_vars *vars);
void			build_exit(char **cmd_args);
void			build_cd(char **cmd, t_env *env);
void			build_pwd(t_env *pwd_fail, t_env *env, int fd);
void			build_echo(char **cmd_args, int fd);
void			child_process(t_vars *vars, t_command *command, t_args *args);
t_token			*new_token(char type, char *value);
t_token			**duplicate_tokens(t_token **tokens);
int				get_tokens_size(t_token **tokens);
void			build_export(char **cmd, t_env *env, int fd);
int				get_env_flag(char *cmd);
void			build_env(t_env *env, int fd);
void			build_unset(char **cmd, t_env **env);
t_files			*get_last_infile(t_files *cmd_files);
t_files			*get_last_outfile(t_files *cmd_files);
char			*get_env(char *string, t_env *env);
char			**convert_env(t_env *env);
void			exec_builtin(t_vars *vars, int i);
int				is_built_in(char *cmd);
t_env			*get_pwd_env(t_env *env);
int				open_files(t_files *file_list, t_env *env);
int				check_ifs(char *string);
int				open_heredocs(t_command **cmd, t_env *env);
char			*expand_file(char *string, t_env *env);
int				is_quote(char c);
t_env			*get_env_node(t_env *env, char *name);
int				delimiter_section(char *input, int *index, int k);
int				count_the_rest(char *input, char *delimiters, int *index);
void			append_and_increment(char **string, char c, int *index);
char			*third_part(char *input, int *index, char *delimiters);
char			token_redir_helper(char **string, char *input, int *index);
char			*skip_quotes_tok(char *input, int *index, char *delimiters);
void			skip_spaces_tok(char *input, int *index);
void			output_pipe(t_tok *vars, char *input, t_token **tokens);
void			free_cmds(t_command **commands);
void			handle_dollar_eof(t_dollar *vars, t_exp *exp, char *string,
					int *index);
void			extract_skip(char *string, t_dollar *vars, int *index,
					t_exp *exp);
void			handle_dollar_alone(t_exp *exp);
void			preparation(t_exp *exp, t_dollar *vars, int token_index,
					int *new_index);
void			init_stuff_wc(t_wc *vars);
void			handle_dollar(int token_index, t_exp *exp);
void			append_before_dollar(t_exp *vars);
void			general_loop(t_exp *vars);
void			set_last_infile(t_files *file_list);
void			set_last_outfile(t_files *file_list);
char			*get_userhome(t_env *env);
char			*expand_tilde(char *string, t_env *env);
char			*clean_command(char *string, t_env *env);
char			**allocate_strings(t_token **tokens, int *index, t_env *env);
t_files			*allocate_files(t_token **tokens, int *index,
					t_files *file_list, t_env *env);
char			**get_path(t_env *env);
int				cmd_count(t_command **cmds);
char			*get_command(char **paths, char *cmd, int *is_dir);
int				get_last_pipe(char *input);
void			free_tokens(t_token **tokens);
void			expand_value(char **string, t_env *env);
char			**append_string(char **array, char *str);
void			write_file(char **array, int fd);
int				open_doc_loop(int *index_readline, t_files *node, t_env *env);
int				here_doc(t_files *node, int index, t_env *env);
void			close_files(int *files, int size);
void			error_message_files(char *str);
int				count_files(t_files *node);
int				check_ambiguous(t_op_files *vars, t_env *env);

#endif
