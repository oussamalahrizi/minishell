/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_count.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 10:23:26 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/24 12:36:09 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	skip_quotes(char *input, int *index)
{
	char	quote;
	int		i;

	i = *index;
	quote = input[i];
	i++;
	while (input[i] && input[i] != quote)
		i++;
	if (!input[i])
	{
		*index = i;
		return (-1);
	}
	i++;
	*index = i;
	return (0);
}

int	inside_quotes(char *input, int *index, char *delimiters, int *k)
{
	int	i;

	if (skip_quotes(input, index) == -1)
		return (-1);
	i = *index;
	while (input[i] && !ft_strchr(delimiters, input[i]) && !is_space(input[i]))
	{
		if (ft_strchr("'\"", input[i]))
		{
			if (skip_quotes(input, &i) == -1)
			{
				*index = i;
				return (-1);
			}
			continue ;
		}
		i++;
	}
	*index = i;
	++(*k);
	return (0);
}

int	delimiter_and_the_rest(char *input, int *index, int *k, char *delimiters)
{
	int	i;

	i = *index;
	if (ft_strchr(delimiters, input[i]))
	{
		if (delimiter_section(input, &i, *k) == -1)
		{
			*index = i;
			return (-1);
		}
		++(*k);
		i++;
	}
	else if (count_the_rest(input, delimiters, &i) == -1)
	{
		*index = i;
		return (-1);
	}
	*index = i;
	++(*k);
	return (0);
}

static int	skip_spaces(char *input, int *index)
{
	int	i;

	i = *index;
	while (input[i] && is_space(input[i]))
		i++;
	*index = i;
	if (!input[i])
		return (0);
	return (1);
}

int	word_count(char *input)
{
	t_wc	vars;

	init_stuff_wc(&vars);
	while (input[vars.i])
	{
		if (skip_spaces(input, &vars.i) == 0)
			break ;
		if (input[vars.i] == '\'' || input[vars.i] == '\"')
		{
			if (inside_quotes(input, &vars.i, vars.delimiters, &vars.k) == -1)
			{
				free(vars.delimiters);
				return (-1);
			}
		}
		else if (delimiter_and_the_rest(input, &vars.i, &vars.k,
				vars.delimiters) == -1)
		{
			free(vars.delimiters);
			return (-1);
		}
	}
	free(vars.delimiters);
	return (vars.k);
}
