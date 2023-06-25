/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 01:30:44 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 10:38:39 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	check_syntax_unset(char *cmd)
{
	int	i;

	i = 0;
	if (!ft_isalpha(cmd[i]) && cmd[i] != '_')
		return (0);
	i++;
	while (cmd[i])
	{
		if (ft_isalpha(cmd[i]) || ft_isdigit(cmd[i]) || cmd[i] == '_')
			i++;
		else
			return (0);
	}
	return (1);
}

void	error_unset_syntax(char *cmd)
{
	ft_putstr_fd("unset: ", 2);
	ft_putchar_fd('`', 2);
	ft_putstr_fd(cmd, 2);
	ft_putchar_fd('\'', 2);
	ft_putendl_fd(" : not a valid identifier", 2);
	g_global.exit_status = 1;
}

void	unset_head(t_env **env)
{
	t_env	*node;

	node = *env;
	free(node->name);
	free(node->value);
	*env = node->next;
	free(node);
}

void	unset_from_list(t_env **env, char *cmd)
{
	t_env	*tmp;
	t_env	*node;

	node = *env;
	while (node->next)
	{
		if (!ft_strcmp(cmd, node->next->name))
		{
			tmp = node->next;
			node->next = node->next->next;
			free(tmp->name);
			free(tmp->value);
			free(tmp);
			break ;
		}
		node = node->next;
	}
}

void	build_unset(char **cmd, t_env **env)
{
	int		i;
	t_env	*node;

	i = 0;
	g_global.exit_status = 0;
	while (cmd[i])
	{
		node = *env;
		if (!check_syntax_unset(cmd[i]))
			error_unset_syntax(cmd[i]);
		if (!ft_strcmp(cmd[i], node->name))
			unset_head(env);
		else
			unset_from_list(env, cmd[i]);
		i++;
	}
}
