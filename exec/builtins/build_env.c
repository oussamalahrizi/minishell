/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 00:56:19 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 11:13:12 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	build_env(t_env *env, int fd)
{
	t_env	*node;

	node = env;
	while (node)
	{
		if (!ft_strcmp("PATH", node->name))
			break ;
		node = node->next;
	}
	if (!node)
	{
		error_cmd("env: No such file or directory\n", 127);
		return ;
	}
	while (env)
	{
		if (env->flag)
		{
			ft_putstr_fd(env->name, fd);
			ft_putchar_fd('=', fd);
			ft_putstr_fd(env->value, fd);
			ft_putchar_fd('\n', fd);
		}
		env = env->next;
	}
}
