/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 01:30:44 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/16 00:25:43 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

extern int exit_status;

int check_syntax_unset(char *cmd)
{
    int i;

    i = 0;
    if(!ft_isalpha(cmd[i]) && cmd[i] != '_')
        return(0);
    i++;
    while(cmd[i])
    {
        if (ft_isalpha(cmd[i]) || ft_isdigit(cmd[i]) || cmd[i] == '_')
            i++;
        else
            return(0);
    }
    return(1);
}

void	build_unset(char **cmd, t_env **env)
{
	int i;
	t_env *node;
	t_env *tmp;

	i = 1;
	exit_status = 0;
	while (cmd[i])
	{
		node = *env;
		if (!check_syntax_unset(cmd[i]))
		{
			ft_putstr_fd("unset: ", 2);
               ft_putchar_fd('`', 2);
               ft_putstr_fd(cmd[i], 2);
               ft_putchar_fd('\'', 2);
               ft_putendl_fd(" : not a valid identifier", 2);
               exit_status = 1;
		}
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