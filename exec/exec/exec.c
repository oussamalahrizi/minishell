/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 21:34:53 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 10:38:39 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	child_process_supp(t_args *args, t_vars *vars)
{
	files	*node;

	g_global.child = 1;
	if (args->nbr_cmds > 1)
	{
		close(args->fd[1]);
		if (args->fd_in != -1)
			close(args->fd_in);
		args->fd_in = args->fd[0];
	}
	node = vars->commands[args->iterator]->files;
	while (node)
	{
		if (node->type == 'h')
		{
			close(node->here_doc_fd[0]);
			close(node->here_doc_fd[1]);
		}
		else if (node->fd != -1)
			close(node->fd);
		node = node->next;
	}
	args->iterator++;
}

void	valid_child_process(t_vars *vars, t_args *args)
{
	if (open_files(vars->commands[args->iterator]->files, vars->env))
		exit(1);
	child_process(vars, vars->commands[args->iterator], args);
}

void	wait_and_free(t_args *args, int *pids)
{
	int	i;
	int	child_status;

	i = 0;
	while (i < args->nbr_cmds)
	{
		waitpid(pids[i], &child_status, 0);
		i++;
	}
	g_global.child = 0;
	if (WIFEXITED(child_status))
		g_global.exit_status = WEXITSTATUS(child_status);
	else if (WIFSIGNALED(child_status))
		g_global.exit_status = 128 + WTERMSIG(child_status);
	close(args->fd_in);
	free(pids);
	free(args);
}

void	exec_supp(t_args *args, t_vars *vars)
{
	int	pid;
	int	*pids;

	pids = malloc(sizeof(int) * args->nbr_cmds);
	while (vars->commands[args->iterator])
	{
		if (args->nbr_cmds > 1 && pipe(args->fd) == -1)
		{
			error_cmd("pipe failed.\n", 1);
			perror("");
			return ;
		}
		pid = fork();
		pids[args->iterator] = pid;
		if (pid == -1)
		{
			error_cmd("fork failed \n", 1);
			return ;
		}
		if (pid == 0)
			valid_child_process(vars, args);
		else
			child_process_supp(args, vars);
	}
	wait_and_free(args, pids);
}

void	exec(t_vars *vars)
{
	t_args	*args;

	args = malloc(sizeof(t_args));
	args->fd[0] = -1;
	args->fd[1] = -1;
	args->nbr_cmds = cmd_count(vars->commands);
	if (open_heredocs(vars->commands, vars->env) == -1)
		return ;
	if (args->nbr_cmds == 1 && is_built_in(vars->commands[0]->cmd)
		&& !open_files(vars->commands[0]->files, vars->env))
		exec_builtin(vars, 0);
	else
	{
		args->iterator = 0;
		args->fd_in = -1;
		exec_supp(args, vars);
	}
}
