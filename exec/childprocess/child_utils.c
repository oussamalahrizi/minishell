/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 06:20:44 by idelfag           #+#    #+#             */
/*   Updated: 2023/06/25 09:34:17 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**get_path(t_env *env)
{
	char	*str;
	char	**paths;
	t_env	*node;

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

char	*search_cmd(char **paths, char *cmd)
{
	int		i;
	char	*command;

	i = 0;
	if (!paths)
		return (NULL);
	while (paths[i])
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

char	*get_command(char **paths, char *cmd, int *is_dir)
{
	struct stat	pathinfo;

	if (!*cmd)
		return (ft_strdup(""));
	if (ft_strrchr(cmd, '/') && !access(cmd, F_OK))
	{
		stat(cmd, &pathinfo);
		if (S_ISDIR(pathinfo.st_mode))
			*is_dir = -1;
		return (ft_strdup(cmd));
	}
	return (search_cmd(paths, cmd));
}
