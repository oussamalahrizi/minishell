#include "../minishell.h"

char	**append_string(char **array, char *str)
{
	char	**new;
	int		i;

	i = 0;
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

void		write_file(char **array, int fd);

static void	expand_value(char **string, t_env *env)
{
	int		i;
	char	*new_value;
	char	*original;
	char	*skip;
	char	*temp;
	int		start_index;
	int		len;

	i = 0;
	original = *string;
	new_value = ft_strdup("");
	while (*original)
	{
		i = 0;
		while (original[i] && original[i] != '$')
		{
			append_character(&new_value, original[i]);
			i++;
		}
		if (!original[i])
			break ;
		i++;
		if (original[i] == '?' || original[i] == '$')
		{
			skip = ft_substr(original, i, 1);
			temp = skip;
			skip = get_env(skip, env);
			free(temp);
			new_value = ft_strjoin(new_value, skip);
			free(skip);
			i++;
		}
		else if (ft_isdigit(original[i]))
		{
			append_character(&new_value, original[i]);
			i++;
		}
		else
		{
			start_index = i;
			len = 0;
			while (original[i] && original[i] != '$' && !is_space(original[i])
				&& !is_quote(original[i]) && (ft_isalnum(original[i])
					|| original[i] == '_'))
			{
				i++;
				len++;
			}
			skip = ft_substr(original, start_index, len);
			temp = skip;
			skip = get_env(skip, env);
			free(temp);
			new_value = ft_strjoin(new_value, skip);
			free(skip);
		}
		original += i;
	}
	free(*string);
	*string = new_value;
}

void	here_doc_signal(int sig)
{
	if (sig == SIGINT)
		exit(130);
}

int	here_doc(char *del, int index, int fd, t_env *env, int flag)
{
	int		i;
	char	*str;
	char	*doc_index;
	char	**array;
	char	**temp;
	int		pid;
	int		status;

	i = 0;
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, here_doc_signal);
		signal(SIGQUIT, SIG_IGN);
		array = malloc(sizeof(char *));
		*array = 0;
		while (1)
		{
			doc_index = ft_itoa(index);
			doc_index = ft_strjoin(doc_index, "_here_doc> ");
			str = readline(doc_index);
			free(doc_index);
			if (!str)
				break ;
			if (!ft_strcmp(del, str))
			{
				free(str);
				break ;
			}
			temp = array;
			if (flag)
				expand_value(&str, env);
			array = append_string(array, str);
			free_double_char(temp);
			i++;
			free(str);
		}
		write_file(array, fd);
		free(array);
		exit(0);
	}
	g_global.heredoc = 1;
	g_global.child = 1;
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status) == 130)
	{
		g_global.heredoc = 0;
		g_global.child = 0;
		g_global.exit_status = 128 + WTERMSIG(status);
		return (-1);
	}
	g_global.heredoc = 0;
	g_global.child = 0;
	return (0);
}

void	write_file(char **array, int fd)
{
	int	i;
	int	res;

	i = 0;
	while (array[i])
	{
		res = write(fd, array[i], ft_strlen(array[i]));
		if (res == -1)
		{
			error_cmd("failed to write to the pipe.\n", 1);
			return ;
		}
		write(fd, "\n", 1);
		i++;
	}
}

int	open_heredocs(Command **cmd, t_env *env)
{
	int i;
	files *node;
	int index = 1;
	i = 0;
	while (cmd[i])
	{
		node = cmd[i]->files;
		while (node)
		{
			if (node->type == 'h')
			{
				if (pipe(node->here_doc_fd) == -1)
				{
					error_cmd("failed to open a pipe for here_doc.", 1);
					return (-1);
				}
				node->fd = node->here_doc_fd[0];
				if (here_doc(node->del, index, node->here_doc_fd[1], env,
						node->expand) == -1)
				{
					g_global.exit_status = 1;
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