/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 21:34:53 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/13 21:16:29 by olahrizi         ###   ########.fr       */
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

void close_files(int *files, int size)
{
	int i = 0;
	while (i < size - 1)
	{
		close(files[size]);
		i++;
	}
}

int cmd_count(Command **cmds)
{
	int i = 0;
	while (cmds[i])
		i++;
	return (i);
}

int open_files(Command **commands)
{
	int i = 0;
	files *node;
	int count = 0;
	int *new_fds;
	int try;
	int failure = 0;
	while (commands[i])
	{
		node = commands[i]->files;
		while (node)
		{
			count++;
			node = node->next;
		}
		i++;
	}
	new_fds = malloc(sizeof(int) * count);
	i = 0;
	count = 0;
	while (commands[i])
	{
		node = commands[i]->files;
		while (node)
		{
			if (node->type == '<')
			{
				try = open(node->filename, O_RDONLY);
				if (try == -1)
				{
					close_files(new_fds, count);
					write(2, "minishell: ", 12);
					ft_putstr_fd(node->filename, 2);
					write(2, ": ", 3);
					perror("");
					exit_status = 1;
					failure = -1;
				}
				else
				{
					new_fds[count++] = try;
					node->fd = try;
				}
			}
			else if (node->type == '>')
			{
				try = open(node->filename, O_TRUNC | O_CREAT | O_RDWR, 0644);
				if (try == -1)
				{
					close_files(new_fds, count);
					write(2, "minishell: ", 12);
					ft_putstr_fd(node->filename, 2);
					write(2, ": ", 3);
					perror("");
					exit_status = 1;
					failure = -1;
				}
				else
				{
					new_fds[count++] = try;
					node->fd = try;
				}
			}
			else if (node->type == 'a')
			{
				try = open(node->filename, O_CREAT | O_APPEND | O_RDWR, 0644);
				if (try == -1)
				{
					close_files(new_fds, count);
					write(2, "minishell: ", 12);
					ft_putstr_fd(node->filename, 2);
					write(2, ": ", 3);
					perror("");
					exit_status = 1;
					failure = -1;
				}
				else
				{
					new_fds[count++] = try;
					node->fd = try;
				}
			}
			else if (node->type == 'h')
			{
				/*
					open pipes instead of hidden file
				*/
				try = open(".hidden", O_CREAT | O_TRUNC | O_RDWR, 0644);
				if (try == -1)
				{
					close_files(new_fds, count);
					write(2, "minishell: ", 12);
					ft_putstr_fd(node->filename, 2);
					write(2, ": ", 3);
					perror("");
					exit_status = 1;
					failure = -1;
				}
				else
				{
					new_fds[count++] = try;
					node->fd = try;
				}
			}
			node = node->next;
		}
		i++;
	}
	free(new_fds);
	return (failure);
}

void exec(t_vars *vars)
{

	// int i = 0;
	// int fd[2];
	// int child_status;
	// int flag_exit = 0;
	int nbr_cmd = cmd_count(vars->commands);
	// int pid;
	if (nbr_cmd == 1 && !open_files(vars->commands))
	{
		if (!ft_strcmp("exit", vars->commands[0]->cmd))
			build_exit(vars->commands[0]->cmd_args);
		else if (!ft_strcmp("cd", vars->commands[0]->cmd))
			build_cd(vars->commands[0]->cmd_args, vars->env);
		else if (!ft_strcmp("pwd", vars->commands[0]->cmd))
			build_pwd();
		else if (!ft_strcmp("echo", vars->commands[0]->cmd))
			build_echo(vars->commands[0]->cmd_args);
		else if (!ft_strcmp("export", vars->commands[0]->cmd))
			build_export(vars->commands[0]->cmd_args, vars->env);
		else if (!ft_strcmp("env", vars->commands[0]->cmd))
			build_env(vars->env);
		else if (!ft_strcmp("unset", vars->commands[0]->cmd))
			build_unset(vars->commands[0]->cmd_args, &vars->env);
	}
	// else
	// {
	// 	int i = 0;
	// 	open_files(vars->commands);
	// 	while (vars->commands[i])
	// 	{
	// 		if (nbr_cmd > 1)
	// 			pipe(fd);
	// 		pid = fork();
	// 		if (pid == 0)
	// 			child_process(vars->commands[i], fd, vars->env);
	// 		else
	// 		{
	// 			waitpid(pid, &child_status, 0);
	// 			if (WIFEXITED(child_status))
	// 				exit_status = WEXITSTATUS(child_status);
	// 			i++;
	// 		}
	// 	}
	// }
	/*
		free local pointers please
	*/
	return ;
	// // if (cmd_count(vars->commands) == 1)
	// // 	exec_one_command(vars->commands, t_env *env)
	// while (vars->commands[i])
	// {
	// 	if (pipe(fd) == -1)
	// 	{
	// 		error_cmd("failed to create a pipe\n", 1);
	// 		exit(1);
	// 	}
	// 	pid = fork();
	// 	if (pid == -1)
	// 	{
	// 		error_cmd("failed to fork\n", 1);
	// 		exit(1);
	// 	}
	// 	if (pid == 0)
	// 		child_exec(vars->commands[i], vars->env, fd, i);
	// 	else
	// 		i++;
	// }
}