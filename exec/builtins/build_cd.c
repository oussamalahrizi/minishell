/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 20:05:40 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/24 21:49:01 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


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

void change_pwd_failed(t_env *env, char *string)
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
	{
		pwd = ft_strdup(string);
		new = malloc(sizeof(t_env));
		new->name = ft_strdup("PWD");
		new->value = ft_strdup(pwd);
		env_add_back(&env, new);
		free(pwd);
		return ;
	}
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
				global.exit_status = 0;
				return;
			}
			node = node->next;
		}
		if (!node)
		{
			error_cmd("cd: HOME not set\n", 1);
			return ;
		}
	}
	else
	{
		char *temp;
		if ((cmd_args[1][0] == '.' && !cmd_args[1][1])
			|| (!ft_strcmp("./", cmd_args[1])))
				temp = ft_strdup(get_pwd_env(env)->value);
		else
			temp = ft_strdup(cmd_args[1]);
		if (access(temp, F_OK))
		{
			write(2, "minishell: cd: ", 16);
			write(2, temp, ft_strlen(temp));
			write(2, ": ", 2);
			perror("");
			temp = ft_strjoin(temp, "/.");
			change_oldpwd(env);
			change_pwd_failed(env, temp);
			global.exit_status = 1;
			free(temp);
			return ;
		}
		else
		{
			if (chdir(temp))
			{
				write(2, "minishell: cd: ", 16);
				write(2, temp, ft_strlen(temp));
				write(2, ": ", 2);
				perror("");
				global.exit_status = 1;
				free(temp);
				return ;
			}
		}
		change_oldpwd(env);
		change_pwd(env);
		free(temp);
		global.exit_status = 0;
	}
}