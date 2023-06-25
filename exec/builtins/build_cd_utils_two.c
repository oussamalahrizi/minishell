/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cd_utils_two.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 04:28:36 by idelfag           #+#    #+#             */
/*   Updated: 2023/06/25 06:07:50 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	oldpwd_supp(t_env *env)
{
	t_env	*node;
	t_env	*pwd;
	t_env	*new;

	node = env;
	while (node)
		node = node->next;
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

void	pwd_supp(t_env *env)
{
	t_env	*node;
	t_env	*new;
	char	*pwd;

	node = env;
	while (node)
		node = node->next;
	if (!node)
	{
		pwd = getcwd(NULL, 0);
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

void	pwd_failed(t_env *env, char *string)
{
	t_env	*node;
	t_env	*new;
	char	*pwd;

	node = env;
	while (node)
		node = node->next;
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
