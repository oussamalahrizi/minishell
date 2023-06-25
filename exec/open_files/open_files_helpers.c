/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 15:16:19 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 15:57:07 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

extern t_global	g_global;

char	*expand_file(char *string, t_env *env)
{
	int		c;
	int		i;
	char	*skip;
	char	*temp;
	char	*rest;

	rest = ft_strdup("");
	if (!string[0])
		return (rest);
	i = 1;
	while (string[i] && string[i] != '$')
		i++;
	c = i;
	if (string[i] == '$')
		c = i - 1;
	skip = ft_substr(string, 1, c);
	temp = skip;
	skip = get_env(skip, env);
	free(temp);
	if (string[i] == '$')
		rest = expand_file(string + i, env);
	skip = ft_strjoin(skip, rest);
	free(rest);
	return (skip);
}

void	close_files(int *files, int size)
{
	int	i;

	i = 0;
	while (i < size - 1)
	{
		close(files[size]);
		i++;
	}
}

void	error_message_files(char *str)
{
	write(2, "minishell: ", 12);
	ft_putstr_fd(str, 2);
	write(2, ": ", 3);
	perror("");
	g_global.exit_status = 1;
}

int	count_files(files *node)
{
	int	count;

	count = 0;
	while (node)
	{
		count++;
		node = node->next;
	}
	return (count);
}

int	check_ambiguous(t_op_files *vars, t_env *env)
{
	if (vars->node->filename[0] == '$')
	{
		vars->expanded = expand_file(vars->node->filename, env);
		if (check_ifs(vars->expanded) == -1 || vars->expanded[0] == 0)
		{
			error_cmd("ambiguous redirect\n", 1);
			free(vars->expanded);
			vars->failure = -1;
			return (-1);
		}
		else
		{
			free(vars->node->filename);
			vars->node->filename = vars->expanded;
		}
	}
	return (0);
}
