/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 01:30:44 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/10 01:52:48 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	build_unset(char **cmd, t_env **env)
{
	int i;
	t_env *node;
	t_env *tmp;

	i = 1;
	while (cmd[i])
	{
		node = *env;
		if (!ft_strcmp(cmd[i], node->name))
		{
			free(node->name);
			free(node->value);
			*env = node->next;
			free(node);
		}
		else
		{	
			while (node->next)
			{
				if (!ft_strcmp(cmd[i], node->next->name))
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
		i++;
	}
}