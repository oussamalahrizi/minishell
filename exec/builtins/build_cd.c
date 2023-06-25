/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 20:05:40 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 22:51:11 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

t_env	*get_pwd_env(t_env *env)
{
	t_env	*node;

	node = env;
	while (node)
	{
		if (!ft_strcmp(node->name, "PWD"))
			return (node);
		node = node->next;
	}
	return (NULL);
}

void	change_pwd_failed(t_env *env, char *string)
{
	t_env	*node;
	char	*pwd;
	char	*tmp;

	node = env;
	while (node)
	{
		if (!ft_strcmp("PWD", node->name))
		{
			pwd = ft_strdup(string);
			tmp = node->value;
			node->value = ft_strdup(pwd);
			free(tmp);
			free(pwd);
			return ;
		}
		node = node->next;
	}
	if (!node)
		pwd_failed(env, string);
}

void	change_pwd(t_env *env)
{
	t_env	*node;
	char	*pwd;
	char	*tmp;

	node = env;
	while (node)
	{
		if (!ft_strcmp("PWD", node->name))
		{
			pwd = getcwd(NULL, 0);
			if (pwd)
			{
				tmp = node->value;
				node->value = ft_strdup(pwd);
				free(tmp);
				free(pwd);
				return ;
			}
		}
		node = node->next;
	}
	if (!node)
		pwd_supp(env);
}

void	change_oldpwd(t_env *env)
{
	t_env	*node;
	t_env	*pwd;
	char	*tmp;

	node = env;
	while (node)
	{
		if (!ft_strcmp(node->name, "OLDPWD"))
		{
			pwd = get_pwd_env(env);
			if (pwd)
			{
				tmp = node->value;
				node->value = ft_strdup(pwd->value);
				free(tmp);
				return ;
			}
		}
		node = node->next;
	}
	if (!node)
		oldpwd_supp(env);
}

void	build_cd(char **cmd_args, t_env *env)
{
	char	*temp;
	int		flag;

	flag = 0;
	if (if_cd_alone(cmd_args, env))
		return ;
	else
	{
		temp = ft_strdup(cmd_args[1]);
		if ((cmd_args[1][0] == '.' && !cmd_args[1][1]) || (!ft_strcmp("./",
			cmd_args[1])))
		{
			if (get_pwd_env(env))
			{
				flag = 1;
				free(temp);
				temp = ft_strdup(get_pwd_env(env)->value);
			}
		}
		if (cd_supp(temp, env, flag))
			return ;
		(change_oldpwd(env), change_pwd(env));
		free(temp);
		g_global.exit_status = 0;
	}
}
