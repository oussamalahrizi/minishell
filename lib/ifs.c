/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ifs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 11:21:49 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/18 14:35:00 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *get_ifs(t_env *env)
{
	char	*variable;
	char	*temp;
	t_env *node;

	temp = "IFS";
	node = env;
	while (node)
	{
		if (!ft_strncmp(temp, node->name, ft_strlen(temp)))
		{
			variable = ft_strdup(node->value);
			return (variable);
		}
		node = node->next;
	}
	variable = NULL;
	return (variable);
}

int check_ifs(char *string, t_env *env)
{
	int count;
	char **splited;
	(void) env;
	
	count = 0;
	if (!*string)
		return (-1);
	splited = split_by_str(string, " \t\n");
	while (splited[count])
		count++;
	free_double_char(splited);
	if (count > 1)
		return (-1);
	return (0);
}
