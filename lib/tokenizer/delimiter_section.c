/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delimiter_section.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 10:48:49 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/23 14:02:27 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_pipe(char *input, int *index)
{
	int	i;
	int	temp;

	i = *index;
	if (input[i] == '|')
	{
		temp = i + 1;
		while (input[temp] && is_space(input[temp]))
			temp++;
		if (!input[temp] || input[temp] == '|')
		{
			*index = i;
			return (-1);
		}
	}
	*index = i;
	return (0);
}

int	check_redir_output(char *input, int *index)
{
	int	i;
	int	temp;
	int	start;

	i = *index;
	if (input[i] == '>')
	{
		start = 0;
		temp = i + 1;
		while (input[temp] && is_space(input[temp]))
		{
			temp++;
			start++;
		}
		if (input[temp] == '|' && start != 0)
		{
			*index = i;
			return (-1);
		}
	}
	*index = i;
	return (0);
}

typedef struct s_chb
{
	int		i;
	int		temp;
	char	c;
	char	l;
}			t_chb;

int	check_both_redir(char *input, int *index)
{
	t_chb	vars;

	vars.i = *index;
	if (input[vars.i] == '>' || input[vars.i] == '<')
	{
		vars.c = input[vars.i];
		if (vars.c == '>')
			vars.l = '<';
		else
			vars.l = '>';
		vars.temp = vars.i + 1;
		if (input[vars.temp] == vars.c)
			vars.temp++;
		while (input[vars.temp] && is_space(input[vars.temp]))
			vars.temp++;
		if (!input[vars.temp] || input[vars.temp] == vars.c
			|| input[vars.temp] == vars.l)
		{
			*index = vars.i;
			return (-1);
		}
	}
	*index = vars.i;
	return (0);
}

int	check_redir_input(char *input, int *index)
{
	int	i;
	int	temp;

	i = *index;
	if (input[i] == '<')
	{
		temp = i + 1;
		while (input[temp] && is_space(input[temp]))
			temp++;
		if (input[temp] == '|')
		{
			*index = i;
			return (-1);
		}
	}
	*index = i;
	return (0);
}

int	delimiter_section(char *input, int *index, int k)
{
	int	i;

	i = *index;
	if ((input[i] == '|' && k == 0))
	{
		*index = i;
		return (-1);
	}
	if (check_pipe(input, index) == -1)
		return (-1);
	if (check_redir_output(input, index) == -1)
		return (-1);
	if (check_both_redir(input, index) == -1)
		return (-1);
	if (check_redir_input(input, index) == -1)
		return (-1);
	return (0);
}
