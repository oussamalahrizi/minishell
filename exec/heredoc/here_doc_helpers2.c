/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_helpers2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 11:19:40 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 11:30:36 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

extern t_global	g_global;

void	write_file(char **array, int fd)
{
	int	i;
	int	res;

	i = 0;
	while (array[i])
	{
		res = write(fd, array[i], ft_strlen(array[i]));
		if (res == -1)
		{
			error_cmd("failed to write to the pipe.\n", 1);
			return ;
		}
		write(fd, "\n", 1);
		i++;
	}
}

int	open_doc_loop(int *index_readline, t_files *node, t_env *env)
{
	int	i;

	i = *index_readline;
	while (node)
	{
		if (node->type == 'h')
		{
			if (pipe(node->here_doc_fd) == -1)
			{
				error_cmd("failed to open a pipe for here_doc.", 1);
				return (-1);
			}
			node->fd = node->here_doc_fd[0];
			if (here_doc(node, i, env) == -1)
			{
				g_global.exit_status = 1;
				return (-1);
			}
			i++;
		}
		node = node->next;
	}
	*index_readline = i;
	return (0);
}
