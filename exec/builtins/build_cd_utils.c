/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 03:08:50 by idelfag           #+#    #+#             */
/*   Updated: 2023/06/25 22:08:41 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	cd_error(char *cmd)
{
	write(2, "minishell: cd: ", 16);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": ", 2);
	perror("");
	g_global.exit_status = 1;
	free(cmd);
}

void	cd_error_two(char *temp, t_env *env)
{
	write(2, "minishell: cd: ", 16);
	write(2, temp, ft_strlen(temp));
	write(2, ": ", 2);
	perror("");
	temp = ft_strjoin(temp, "/.");
	change_oldpwd(env);
	change_pwd_failed(env, temp);
	g_global.exit_status = 1;
	free(temp);
}

int	cd_alone_supp(t_env *env)
{
	t_env	*node;

	node = env;
	while (node)
	{
		if (!ft_strcmp(node->name, "HOME"))
		{
			if (chdir(node->value))
			{
				error_cmd("failed to change dir\n", 1);
				return (1);
			}
			change_oldpwd(env);
			change_pwd(env);
			g_global.exit_status = 0;
			return (1);
		}
		node = node->next;
	}
	return (0);
}

int	cd_supp(char *temp, t_env *env, int flag)
{
	if (flag && access(temp, F_OK))
	{
		cd_error_two(temp, env);
		return (1);
	}
	else
	{
		if (chdir(temp))
		{
			cd_error(temp);
			return (1);
		}
	}
	return (0);
}

int	if_cd_alone(char **cmd_args, t_env *env)
{
	t_env	*node;
	int		len;

	len = count_args(cmd_args);
	if (len == 1)
	{
		node = env;
		if (cd_alone_supp(node))
			return (1);
		if (!node)
		{
			error_cmd("cd: HOME not set\n", 1);
			return (1);
		}
	}
	return (0);
}
