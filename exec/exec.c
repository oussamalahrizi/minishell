/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 21:34:53 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/19 14:12:16 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int exit_status;

files *get_last_infile(files *cmd_files)
{
	files *node;
	files *res;
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

files *get_last_outfile(files *cmd_files)
{
	files *node;
	files *res;
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

int cmd_count(Command **cmds)
{
	int i = 0;
	while (cmds[i])
		i++;
	return (i);
}

int is_built_in(char *cmd)
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

void exec_builtin(t_vars *vars, int i)
{
	if (!ft_strcmp("exit", vars->commands[i]->cmd))
		build_exit(vars->commands[i]->cmd_args);
	else if (!ft_strcmp("cd", vars->commands[i]->cmd))
		build_cd(vars->commands[i]->cmd_args, vars->env);
	else if (!ft_strcmp("pwd", vars->commands[i]->cmd))
		build_pwd(get_pwd_env(vars->env)->value, vars->env);
	else if (!ft_strcmp("echo", vars->commands[i]->cmd))
		build_echo(vars->commands[i]->cmd_args);
	else if (!ft_strcmp("export", vars->commands[i]->cmd))
		build_export(vars->commands[i]->cmd_args, vars->env);
	else if (!ft_strcmp("env", vars->commands[i]->cmd))
		build_env(vars->env);
	else if (!ft_strcmp("unset", vars->commands[i]->cmd))
		build_unset(vars->commands[i]->cmd_args, &vars->env);
}

void exec(t_vars *vars)
{
	int i;
	int fd[2] = {-1, -1};
	int child_status;
	int nbr_cmd = cmd_count(vars->commands);
	int pid;
	if (nbr_cmd == 1 && is_built_in(vars->commands[0]->cmd) && !open_files(vars->commands, NULL, vars->env))
		exec_builtin(vars, 0);
	else
	{
		i = 0;
		int *failed_cmd = malloc(sizeof(int) * nbr_cmd);
		int *pids = malloc(sizeof(int) * nbr_cmd);
		int fd_in = -1;
		open_files(vars->commands, failed_cmd, vars->env);
		while (vars->commands[i])
		{
			if (nbr_cmd > 1)
				pipe(fd);
			pid = fork();
			pids[i] = pid;
			if (pid == -1)
				error_cmd("fork failed \n", 1);
			else if (pid == 0)
				child_process(vars, vars->commands[i], fd, vars->env, nbr_cmd, fd_in, i, failed_cmd[i]);
			else
			{
				if (nbr_cmd > 1)
				{
					close(fd[1]);
					(fd_in != -1) && close(fd_in);
					fd_in = fd[0];
				}
				files *node = vars->commands[i]->files;
				while (node)
				{
					if (node->fd != -1)
						close(node->fd);
					node = node->next;
				}
				unlink(".tmp");
				i++;
			}
		}
		i = 0;
		while (i < nbr_cmd)
		{
			waitpid(pids[i], &child_status, 0);
			i++;
		}
		if (WIFEXITED(child_status))
			exit_status = WEXITSTATUS(child_status);
		close(fd_in);
		free(failed_cmd);
		free(pids);
	}
	return ;
}
