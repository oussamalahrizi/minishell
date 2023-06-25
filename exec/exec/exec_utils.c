/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 09:12:55 by idelfag           #+#    #+#             */
/*   Updated: 2023/06/25 12:13:13 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_files	*get_last_infile(t_files *cmd_files)
{
	t_files	*node;
	t_files	*res;

	res = NULL;
	node = cmd_files;
	while (node)
	{
		if (node->type == 'h' || node->type == '<')
			res = node;
		node = node->next;
	}
	return (res);
}

t_files	*get_last_outfile(t_files *cmd_files)
{
	t_files	*node;
	t_files	*res;

	res = NULL;
	node = cmd_files;
	while (node)
	{
		if (node->type == 'a' || node->type == '>')
			res = node;
		node = node->next;
	}
	return (res);
}

int	cmd_count(t_command **cmds)
{
	int	i;

	i = 0;
	while (cmds[i])
		i++;
	return (i);
}

int	is_built_in(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strcmp("exit", cmd))
		return (1);
	else if (!ft_strcmp("cd", cmd))
		return (1);
	else if (!ft_strcmp("pwd", cmd))
		return (1);
	else if (!ft_strcmp("echo", cmd))
		return (1);
	else if (!ft_strcmp("export", cmd))
		return (1);
	else if (!ft_strcmp("env", cmd))
		return (1);
	else if (!ft_strcmp("unset", cmd))
		return (1);
	return (0);
}

void	exec_builtin(t_vars *vars, int i)
{
	t_files	*outfile;
	int		fd_out;

	outfile = get_last_outfile(vars->commands[i]->files);
	fd_out = STDOUT_FILENO;
	if (outfile && outfile->fd != -1)
		fd_out = outfile->fd;
	if (!ft_strcmp("exit", vars->commands[i]->cmd))
		build_exit(vars->commands[i]->cmd_args);
	else if (!ft_strcmp("cd", vars->commands[i]->cmd))
		build_cd(vars->commands[i]->cmd_args, vars->env);
	else if (!ft_strcmp("pwd", vars->commands[i]->cmd))
		build_pwd(get_pwd_env(vars->env)->value, vars->env, fd_out);
	else if (!ft_strcmp("echo", vars->commands[i]->cmd))
		build_echo(vars->commands[i]->cmd_args, fd_out);
	else if (!ft_strcmp("export", vars->commands[i]->cmd))
		build_export(vars->commands[i]->cmd_args, vars->env, fd_out);
	else if (!ft_strcmp("env", vars->commands[i]->cmd))
		build_env(vars->env, fd_out);
	else if (!ft_strcmp("unset", vars->commands[i]->cmd))
		build_unset(vars->commands[i]->cmd_args, &vars->env);
	if (outfile && outfile->fd != -1)
		close(outfile->fd);
}
