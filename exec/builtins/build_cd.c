/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 20:05:40 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/10 00:49:51 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

extern int exit_status;


t_env *get_pwd_env(t_env *env)
{
	t_env *node;

	node = env;
	while(node)
	{
		if (!ft_strcmp(node->name, "PWD"))
			return (node);
		node = node->next;
	}
	return (NULL);
}

void change_pwd(t_env *env)
{
	t_env *node;
	t_env *new;
	char *pwd;
	char *tmp;

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
	{
		pwd  = getcwd(NULL, 0);
		if (pwd)
		{
			new = malloc(sizeof(t_env));
			new->name = ft_strdup("PWD");
			new->value = ft_strdup(pwd);
			env_add_back(&env, new);
			free(pwd);
			return ;
		}
	}
}

void change_oldpwd(t_env *env)
{
	t_env *node;
	t_env *new;
	t_env *pwd;
	char *tmp;

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
	{
		pwd = get_pwd_env(env);
		if (pwd)
		{
			new = malloc(sizeof(t_env));
			new->name = ft_strdup("OLDPWD");
			new->value = ft_strdup(pwd->value);
			env_add_back(&env, new);
			return ;
		}
	}
}

void build_cd(char **cmd_args, t_env *env)
{
	t_env *node;
	int len;
	
	len = count_args(cmd_args);
	if (len == 1)
	{
		node = env;
		while (node)
		{
			if (!ft_strcmp(node->name, "HOME"))
			{
				if(chdir(node->value))
				{
					error_cmd("failed to change dir\n", 1);
					return ;
				}
				change_oldpwd(env);
				change_pwd(env);
				exit_status = 0;
				return;
			}
			node = node->next;
		}
		if (!node)
		{
			error_cmd("cd : HOME not set\n", 1);
			return ;
		}
	}
	else
	{
		if (chdir(cmd_args[1]))
		{
			write(2, "cd : failed to change dir\n", 27);
			exit_status = 1;
			return ;
		}
		change_oldpwd(env);
		change_pwd(env);
		exit_status = 0;
	}
}