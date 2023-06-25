/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 10:20:46 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 15:14:43 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

extern t_global	g_global;

void	here_doc_signal(int sig)
{
	if (sig == SIGINT)
		exit(130);
}

typedef struct s_here_doc
{
	int			i;
	char		*str;
	char		*doc_index;
	char		**array;
	int			pid;
	int			status;
}				t_here_doc;

static void	here_doc_readline(t_here_doc *vars, int index, files *node,
		t_env *env)
{
	while (1)
	{
		vars->doc_index = ft_itoa(index);
		vars->doc_index = ft_strjoin(vars->doc_index, "_here_doc> ");
		vars->str = readline(vars->doc_index);
		free(vars->doc_index);
		if (!vars->str)
			break ;
		if (!ft_strcmp(node->del, vars->str))
		{
			free(vars->str);
			break ;
		}
		if (node->expand)
			expand_value(&vars->str, env);
		vars->array = append_string(vars->array, vars->str);
		vars->i++;
		free(vars->str);
	}
}

static int	cleanup_here_doc(t_here_doc *vars)
{
	g_global.heredoc = 1;
	g_global.child = 1;
	waitpid(vars->pid, &vars->status, 0);
	if (WEXITSTATUS(vars->status) == 130)
	{
		g_global.heredoc = 0;
		g_global.child = 0;
		g_global.exit_status = 128 + WTERMSIG(vars->status);
		return (-1);
	}
	g_global.heredoc = 0;
	g_global.child = 0;
	return (0);
}

int	here_doc(files *node, int index, t_env *env)
{
	t_here_doc	vars;

	vars.i = 0;
	vars.pid = fork();
	if (vars.pid == 0)
	{
		signal(SIGINT, here_doc_signal);
		signal(SIGQUIT, SIG_IGN);
		vars.array = malloc(sizeof(char *));
		vars.array[0] = 0;
		here_doc_readline(&vars, index, node, env);
		write_file(vars.array, node->here_doc_fd[1]);
		free(vars.array);
		exit(0);
	}
	if (cleanup_here_doc(&vars) == -1)
		return (-1);
	return (0);
}

int	open_heredocs(Command **cmd, t_env *env)
{
	files	*node;
	int		i;
	int		index;

	i = 0;
	index = 1;
	while (cmd[i])
	{
		node = cmd[i]->files;
		if (open_doc_loop(&index, node, env) == -1)
			return (-1);
		i++;
	}
	return (0);
}
