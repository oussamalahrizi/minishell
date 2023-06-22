# include "../minishell.h"

char **append_string(char **array, char *str)
{
	char **new;

	int i = 0;
	while (array[i])
		i++;
	new = malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (array[i])
	{
		new[i] = ft_strdup(array[i]);
		i++;
	}
	new[i++] = ft_strdup(str);
	new[i] = 0;
	return (new);
}


void write_file(char **array, int fd);

static void str_copy(char *src, char *dest, int start, int end)
{
	int i = 0 ;
	while (src[i] && start < end)
		dest[start++] = src[i++];
}

static void expand_value(char *string, t_env *env)
{
	int i = 0;
	char *to_expand;
	int c;
	while (string[i])
	{
		while (string[i] && string[i] != '$')
			i++;
		c = 0;
		if (!string[i])
			break;
		i++;
		int start_index = i;
		while (string[i] && string[i] != '$' && !is_space(string[i]
			&& !is_quote(string[i]) && (ft_isalnum(string[i]) || string[i] == '_')))
		{
			c++;
			i++;
		}
		if (string[i])
			c--;
		to_expand = ft_substr(string, start_index, c);
		char *temp = to_expand;
		printf("value to expand : %s|\n", to_expand);
		to_expand = get_env(to_expand, env);
		free(temp);
		printf("value after expand : %s|\n", to_expand);
		str_copy(to_expand, string, start_index - 1, i);
	}
}


void here_doc_signal(int sig)
{
	if (sig == SIGINT)
		exit(130);
}

int here_doc(char *del, int index, int fd, t_env *env)
{
	int i;
	char *str;
	char *doc_index;
	char **array;
	char **temp;
	int pid;
	int status;
	i = 0;
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, here_doc_signal);
		signal(SIGQUIT, SIG_IGN);
		array = malloc(sizeof(char *));
		*array = 0;
		while(1)
		{
			doc_index = ft_itoa(index);
			doc_index = ft_strjoin(doc_index, "_here_doc> ");
			str = readline(doc_index);
			free(doc_index);
			if (!str)
				break;
			if (!ft_strcmp(del, str))
			{
				free(str);
				break ;
			}
			temp = array;
			if (!ft_strchr(del, '\"') || !ft_strchr(del, '\''))
				expand_value(str, env);
			array = append_string(array, str);
			free_double_char(temp);
			i++;
			free(str);
		}
		write_file(array, fd);
		free(array);
		exit(0);
	}
	global.heredoc = 1;
	global.child = 1;
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status) == 130)
	{
		global.exit_status = 128 + WTERMSIG(status);
		return (-1);
	}
	global.heredoc = 0;
	global.child = 0;
	return (0);
}

void write_file(char **array, int fd)
{
	int i = 0;
	int res;

	while (array[i])
	{
		res = write(fd, array[i], ft_strlen(array[i]));
		if (res == -1)
		{
			error_cmd("failed to write to the pipe.\n", 1);
			return;
		}
		write(fd, "\n", 1);
		i++;
	}
}

int open_heredocs(Command **cmd, t_env *env)
{
	int i;
	files *node;
	int index = 1;
	i = 0;
	while(cmd[i])
	{
		node = cmd[i]->files;
		while(node)
		{
			if (node->type == 'h')
			{
				if (pipe(node->here_doc_fd) == -1)
				{
					error_cmd("failed to open a pipe for here_doc.", 1);
        			return (-1);
				}
				node->fd = node->here_doc_fd[0];
				if (here_doc(node->del, index, node->here_doc_fd[1], env) == -1)
				{
					global.exit_status = 1;
					return (-1);
				}
				index++;
			}
			node = node->next;
		}
		i++;
	}
	return (0);
}