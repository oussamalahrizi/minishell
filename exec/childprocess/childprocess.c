/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childprocess.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 00:05:08 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 12:13:49 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	dup_util(t_child *child, t_args *args, t_command *command, t_vars *vars)
{	
	if (child->infile && child->infile->fd != -1)
	{
		dup2(child->infile->fd, STDIN_FILENO);
		if (child->infile->type == 'h')
			close(child->infile->here_doc_fd[1]);
		close(args->fd_in);
		close(args->fd[0]);
	}
	else if (args->iterator > 0)
		dup2(args->fd_in, STDIN_FILENO);
	if (args->fd[0] != -1)
		close(args->fd[0]);
	if (child->outfile && child->outfile->fd != -1)
	{
		dup2(child->outfile->fd, STDOUT_FILENO);
		close(args->fd[1]);
	}
	else if (args->nbr_cmds > 1 && args->iterator != args->nbr_cmds - 1)
		dup2(args->fd[1], STDOUT_FILENO);
	if (is_built_in(command->cmd))
	{
		exec_builtin(vars, args->iterator);
		exit(g_global.exit_status);
	}
}

void	init(t_child *child, t_command *command, t_args *args, t_vars *vars)
{
	child->env_list = convert_env(vars->env);
	child->infile = get_last_infile(command->files);
	child->outfile = get_last_outfile(command->files);
	if (!command->cmd)
		exit(0);
	dup_util(child, args, command, vars);
	if (!ft_strcmp(command->cmd, "."))
	{
		write(2, "filename argument required.\n", 28);
		exit(2);
	}
	child->paths = get_path(vars->env);
	child->cmd_path = get_command(child->paths, command->cmd, &child->is_dir);
}

void	child_process(t_vars *vars, t_command *command, t_args *args)
{
	t_child	child;

	init(&child, command, args, vars);
	if (!child.cmd_path)
	{
		write(2, "command not found: ", 19);
		write(2, command->cmd, ft_strlen(command->cmd));
		write(2, "\n", 1);
		if (child.paths)
			free_double_char(child.paths);
		exit(127);
	}
	if (!*child.cmd_path)
		exit(0);
	execve(child.cmd_path, command->cmd_args, child.env_list);
	write(2, command->cmd, ft_strlen(command->cmd));
	write(2, " : ", 3);
	if (child.is_dir == -1)
		write(2, "is a directory\n", 16);
	else
		perror("");
	exit(127);
}
