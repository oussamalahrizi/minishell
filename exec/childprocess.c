/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childprocess.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 00:05:08 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 10:49:02 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
extern t_global g_global;

char    **get_path(t_env *env)
{
	char    *str;
	char    **paths;
	t_env *node;

	str = NULL;
	node = env;
	while (node)
	{
		if (!ft_strcmp("PATH", node->name))
		{
			str = ft_strdup(node->value);
			break ;
		}
		node = node->next;
	}
	if (!str)
		return (0);
	paths = ft_split(str, ':');
	free(str);
	return (paths);
}

char    *get_command(char **paths, char *cmd, int *is_dir)
{
	char    *command;
	int             i;
	struct stat pathInfo;

	i = 0;
	if (!*cmd)
		return (ft_strdup(""));
	if (ft_strrchr(cmd, '/') && !access(cmd, F_OK))
	{
		stat(cmd, &pathInfo);
		if (S_ISDIR(pathInfo.st_mode))
			*is_dir = -1;
		return (ft_strdup(cmd));
	}
	while (paths && paths[i])
	{
		command = ft_strdup(paths[i]);
		command = ft_strjoin(command, "/");
		command = ft_strjoin(command, cmd);
		if (!access(command, F_OK))
			return (command);
		free(command);
		i++;
	}
	return (NULL);
}

void child_process(t_vars *vars, Command *command, int *fd, t_env *env, int nbr_cmds, int fd_in, int iterator)
{
	char **paths;
	char **env_list = convert_env(env);
	int is_dir;
	files *infile = get_last_infile(command->files);
	files *outfile = get_last_outfile(command->files);

	if (!command->cmd)
		exit(0);
	if(infile && infile->fd != -1)
	{
		dup2(infile->fd, STDIN_FILENO);
		if (infile->type == 'h')
			close(infile->here_doc_fd[1]);
		close(fd_in);
		close(fd[0]);
	}
	else if (iterator > 0)
		dup2(fd_in, STDIN_FILENO);
	if (fd[0] != -1)
		close(fd[0]);
	if(outfile && outfile->fd != -1)
	{
		dup2(outfile->fd, STDOUT_FILENO);
		close(fd[1]);
	}
	else if (nbr_cmds > 1 && iterator != nbr_cmds - 1)
		dup2(fd[1], STDOUT_FILENO);
	if (is_built_in(command->cmd))
		(exec_builtin(vars, iterator), exit(g_global.exit_status));
	else
	{
		if (!ft_strcmp(command->cmd, "."))
		{
			write(2, "filename argument required.\n", 28);
			exit(2);
		}
		paths = get_path(env);
		char *cmd_path = get_command(paths, command->cmd, &is_dir);
		if (!cmd_path)
		{
			// error_cmd("command not found: ", 127);
			write(2, "command not found: ", 19);
			write(2, command->cmd, ft_strlen(command->cmd));
			write(2, "\n", 1);
			if (paths)
				free_double_char(paths);
			exit(127);
		}
		if (!*cmd_path)
			exit(0);
		execve(cmd_path, command->cmd_args, env_list);
		write(2, command->cmd, ft_strlen(command->cmd));
		write(2, " : ", 3);
		if (is_dir == -1)
			write(2, "is a directory\n", 16);
		else
			perror("");
		exit(126);
	}
}