/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childprocess.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 00:05:08 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/12 05:07:42 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// char *get_command(char *cmd, t_env *env)
// {
// 	char *path = get_env("PATH", env);
// 	char *cmd_path = NULL;
// 	if (!path)
// 		return (NULL);
// 	char *splited = ft_split(path, ':');
// 	int i = 0;
// 	// while(splited[i])
// 	// {
// 	// 	if ()
// 	// }
// 	return (cmd_path);
// }

void child_process(Command *command, int *fd, t_env *env)
{
	(void) command;
	(void) fd;
	(void) env;
	// files *infile = get_last_infile(command->files);
	// files *outfile = get_last_outfile(command->files);

	// if(infile->fd != -1)
	// 	dup2(infile->fd, STDIN_FILENO);
	// if(outfile->fd != -1)
	// 	dup2(outfile->fd, STDOUT_FILENO);
	// char *cmd_path = get_command(command->cmd, env);
	// if (!*cmd_path)
	// {
	// 	error_cmd("command not found\n", 127);
	// 	return ;
	// }
	
}