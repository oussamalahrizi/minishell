/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 21:57:24 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/23 21:58:38 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_double(Token **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]->value);
		free(str[i]);
		i++;
	}
	free(str);
}

void free_double_char(char **str)
{
	int i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void free_cmds(Command **commands)
{
	int i = 0;
	files *node;
	files *temp;
	while(commands[i])
	{
		if (commands[i]->cmd)
			free(commands[i]->cmd);
		if (commands[i]->cmd_args)
			free_double_char(commands[i]->cmd_args);
		if (commands[i]->files)
		{
			node = commands[i]->files;
			while (node)
			{
				temp = node;
				if (node->del)
					free(node->del);
				if (node->filename)
					free(node->filename);
				node = node->next;
				free(temp);
			}
		}
		free(commands[i]);
		i++;
	}
	free(commands);
}
