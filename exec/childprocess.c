/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childprocess.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 00:05:08 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/15 20:17:32 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
        return (paths);
}

char    *get_command(char **paths, char *cmd)
{
        char    *temp;
        char    *command;
        int             i;

        i = 0;
        while (paths[i])
        {
                temp = ft_strjoin(paths[i], "/");
                command = ft_strjoin(temp, cmd);
                if (!access(command, X_OK))
                        return (command);
                free(command);
                i++;
        }
        return (NULL);
}

void child_process(Command *command, int *fd, t_env *env, int nbr_cmds, int fd_in, int iterator, int is_failed)
{
	char **paths;
	char **env_list = convert_env(env);

	files *infile = get_last_infile(command->files);
	files *outfile = get_last_outfile(command->files);

	if (is_failed == -1)
		exit(1);
	if(infile && infile->fd != -1)
	{
		dup2(infile->fd, STDIN_FILENO);
		close(fd_in);
	}
	else if (iterator > 0)
		dup2(fd_in, STDIN_FILENO);
	if(outfile && outfile->fd != -1)
	{
		dup2(outfile->fd, STDOUT_FILENO);
		close(fd[1]);
	}
	else if (nbr_cmds > 1 && iterator != nbr_cmds - 1)
		dup2(fd[1], STDOUT_FILENO);
	paths = get_path(env);
	char *cmd_path = get_command(paths, command->cmd);
	if (!cmd_path)
	{
		error_cmd("command not found: ", 127);	
		perror("");
		exit(127);
	}
	execve(cmd_path, command->cmd_args, env_list);
	error_cmd("failed to execute command\n", 1);
}