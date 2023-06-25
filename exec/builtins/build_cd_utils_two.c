/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_cd_utils_two.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 04:28:36 by idelfag           #+#    #+#             */
/*   Updated: 2023/06/25 21:37:40 by olahrizi         ###   ########.fr       */
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
			new->flag = 1;
			new->next = NULL;
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
			new->flag = 1;
			new->next = NULL;
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
		new->next = NULL;
		new->flag = 1;
		env_add_back(&env, new);
		free(pwd);
		return ;
	}
}
