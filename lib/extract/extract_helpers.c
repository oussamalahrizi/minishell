/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 17:16:01 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/24 17:55:02 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	set_last_infile(t_files *file_list)
{
	t_files	*node;
	t_files	*res;

	res = NULL;
	node = file_list;
	while (node)
	{
		if (node->type == '<')
		{
			if (res)
				res->open = -1;
			res = node;
		}
		node = node->next;
	}
}

void	set_last_outfile(t_files *file_list)
{
	t_files	*node;
	t_files	*res;

	res = NULL;
	node = file_list;
	while (node)
	{
		if (node->type == 'a' || node->type == '>')
		{
			if (res)
				res->open = -1;
			res = node;
		}
		node = node->next;
	}
}

char	*get_userhome(t_env *env)
{
	char	*str;
	char	*temp;

	temp = get_env("HOME", env);
	if (!*temp)
	{
		str = ft_strdup("/Users/");
		free(temp);
		temp = get_env("USER", env);
		if (!*temp)
		{
			free(temp);
			temp = get_env("LOGNAME", env);
			if (!*temp)
			{
				free(str);
				return (temp);
			}
		}
		str = ft_strjoin(str, temp);
		free(temp);
		return (str);
	}
	return (temp);
}

static void	skip_quotes(int *index, char *temp, char **res)
{
	char	quote;
	int		i;

	i = *index;
	quote = temp[i];
	i++;
	while (temp[i] && temp[i] != quote)
	{
		append_character(res, temp[i]);
		i++;
	}
	i++;
	*index = i;
}

char	*clean_command(char *string, t_env *env)
{
	int		i;
	char	*res;
	char	*temp;

	temp = ft_strdup(string);
	temp = expand_tilde(temp, env);
	res = ft_strdup("");
	i = 0;
	while (temp[i])
	{
		if (ft_strchr("'\"", temp[i]))
			skip_quotes(&i, temp, &res);
		else
		{
			append_character(&res, temp[i]);
			i++;
		}
	}
	free(temp);
	return (res);
}
