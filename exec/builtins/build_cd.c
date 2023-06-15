/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 20:05:40 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/15 22:32:14 by olahrizi         ###   ########.fr       */
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

char *get_userhome(t_env *env)
{
	char *str;
	char *temp;

	temp = get_env("HOME", env);
	str = ft_strdup("/Users/");
	if (!*temp)
	{
		free(temp);
		temp = get_env("USER", env);
		if (!*temp)
		{
			free(temp);
			temp = get_env("LOGNAME", env);
			if (!*temp)
			{
				free(str);
				return (temp);
			}
		}
		str = ft_strjoin(str, temp);
	}
	else
	{
		free(str);
		return (temp);
	}
	free(temp);
	return (str);
}

void build_cd(char **cmd_args, t_env *env)
{
	t_env *node;
	int len;
	char *str;
	
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
			error_cmd("cd: HOME not set\n", 1);
			return ;
		}
	}
	else
	{
		if (cmd_args[1][0] == '~' && (cmd_args[1][1] == '/' || !cmd_args[1][1]))
		{
			str = get_userhome(env);
			if (!*str)
			{
				error_cmd("cd: HOME/USER/LOGNAME not set\n", 1);
				free(str);
				return;
			}
		}
		else if (cmd_args[1][0] == '~' && cmd_args[1][1] != '/')
		{
			str = ft_strdup("/Users/");
			int i = 1;
			while (cmd_args[1][i] && cmd_args[1][i] != '/')
				i++;
			char *temp = ft_substr(&cmd_args[1][1], 0, i);
			str = ft_strjoin(str, temp);
			int j = 0;
			if (cmd_args[1][i])
			{
				i++;
				int save = i;
				while (cmd_args[1][i])
				{
					j++;
					i++;
				}
				free(temp);
				temp = ft_substr(&cmd_args[1][save], 0, j);
				str = ft_strjoin(str, temp);
			}
			free(temp);
		}
		else
		{
			str = ft_strdup("");
			str = ft_strjoin(str, cmd_args[1]);
		}
		if (chdir(str))
		{
			write(2, "minishell: cd: ", 16);
			write(2, str, ft_strlen(str));
			write(2, ": ", 2);
			perror("");
			free(str);
			exit_status = 1;
			return ;
		}
		change_oldpwd(env);
		change_pwd(env);
		free(str);
		exit_status = 0;
	}
}