/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 21:34:53 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/06 23:55:49 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int exit_status;

files *get_last_infile(Command *commands)
{
	files *node;
	files *res;
	res = NULL;
	node = commands->files;
	while (node)
	{
		if (node->type == 'h' || node->type == '<')
			res = node;
		node = node->next;
	}
	return (res);
}

files *get_last_outfile(Command *commands)
{
	files *node;
	files *res;
	res = NULL;
	node = commands->files;
	while (node)
	{
		if (node->type == 'a' || node->type == '>')
			res = node;
		node = node->next;
	}
	return (res);
}

// void child_exec(Command *command, t_env *env, int *fd, int index)
// {
// 	files *infile;
// 	files *outfile;
// 	int infile_fd;

// 	if (!infile)
// 	{
// 		if (index != 0)
// 			dup2(fd[0], STDIN_FILENO);
// 	}
// 	// else
// 	// {
// 	// 	if (infile->filename)
// 	// 	{
// 	// 		dup2(infile->fd, STDIN_FILENO);
// 	// 	}
// 	// }
// }

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

int open_files(Command **commands, int **fds, int *size)
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
	// printf("total files : %d\n", count);
	*size = count;
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
					new_fds[count++] = try;
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
					new_fds[count++] = try;
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
					new_fds[count++] = try;
			}
			node = node->next;
		}
		i++;
	}
	*fds = new_fds;
	return (failure);
}

void exec(t_vars *vars)
{
	// files *infile;

	// int i = 0;
	// int fd[2];
	// int pid_open_files;
	int *array_fd = NULL;
	// int child_status;
	int size;
	// int flag_exit = 0;
	int nbr_cmd = cmd_count(vars->commands);
	// int pid;
	if (nbr_cmd == 1 && !open_files(vars->commands, &array_fd, &size))
	{
		if (!ft_strcmp("exit", vars->commands[0]->cmd))
			build_exit(vars->commands[0]->cmd_args);
		else if (!ft_strcmp("cd", vars->commands[0]->cmd))
			build_cd(vars->commands[0]->cmd_args, vars->env);
		else if (!ft_strcmp("pwd", vars->commands[0]->cmd))
			build_pwd();
		else if (!ft_strcmp("echo", vars->commands[0]->cmd))
			build_echo(vars->commands[0]->cmd_args);
	}
	/*
		free local pointers please
	*/
	free(array_fd);
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