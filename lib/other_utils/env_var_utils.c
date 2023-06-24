/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 22:15:21 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/24 22:15:22 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	count_env(char **env)
{
	int	len;

	len = 0;
	while (env[len])
		len++;
	return (len);
}

int	get_env_flag(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

void	env_add_back(t_env **node, t_env *data)
{
	t_env	*temp;

	if (*node)
	{
		temp = *node;
		while (temp->next)
			temp = temp->next;
		temp->next = data;
	}
	else
		*node = data;
}

t_env	*copy_env(char **env)
{
	t_env	*new_env;
	t_env	*node;
	char	**temp;
	int		i;

	new_env = NULL;
	i = 0;
	while (env[i])
	{
		node = malloc(sizeof(t_env));
		temp = get_name_value(env[i]);
		node->name = ft_strdup(temp[0]);
		node->value = ft_strdup(temp[1]);
		node->flag = get_env_flag(env[i]);
		free_double_char(temp);
		node->next = NULL;
		env_add_back(&new_env, node);
		i++;
	}
	return (new_env);
}

void	increase_shell_lvl(t_env *env)
{
	t_env	*node;
	char	*temp;
	int		n;

	node = env;
	while (node)
	{
		if (!ft_strncmp("SHLVL", node->name, 6))
		{
			temp = node->value;
			n = ft_atoi(node->value);
			node->value = ft_itoa(n + 1);
			free(temp);
			return ;
		}
		node = node->next;
	}
}
