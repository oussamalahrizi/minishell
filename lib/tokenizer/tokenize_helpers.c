/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 19:34:49 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/23 21:45:32 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

typedef struct s_shelp
{
	int		i;
	char	quote;
	char	*string;
}			t_shelp;

void	append_and_increment(char **string, char c, int *index)
{
	append_character(string, c);
	++(*index);
}

static void	skip_quotes_helper(char *input, int *index, char **string)
{
	char	quote;
	int		i;

	i = *index;
	quote = input[i];
	append_character(string, input[i]);
	i++;
	while (input[i] && input[i] != quote)
	{
		append_character(string, input[i]);
		i++;
	}
	append_character(string, input[i]);
	i++;
	*index = i;
}

char	*skip_quotes_tok(char *input, int *index, char *delimiters)
{
	t_shelp	vars;

	vars.i = *index;
	vars.quote = input[vars.i];
	vars.string = ft_strdup("");
	append_and_increment(&vars.string, input[vars.i], &vars.i);
	while (input[vars.i] && input[vars.i] != vars.quote)
		append_and_increment(&vars.string, input[vars.i], &vars.i);
	append_and_increment(&vars.string, input[vars.i], &vars.i);
	while (input[vars.i] && !ft_strchr(delimiters, input[vars.i])
		&& !is_space(input[vars.i]))
	{
		if (ft_strchr("'\"", input[vars.i]))
		{
			skip_quotes_helper(input, &vars.i, &vars.string);
			continue ;
		}
		append_and_increment(&vars.string, input[vars.i], &vars.i);
	}
	*index = vars.i;
	return (vars.string);
}

char	*third_part(char *input, int *index, char *delimiters)
{
	char	*string;
	int		i;
	char	quote;

	i = *index;
	string = ft_strdup("");
	while (input[i] && !ft_strchr(delimiters, input[i]) && !is_space(input[i]))
	{
		if (ft_strchr("'\"", input[i]))
		{
			quote = input[i];
			append_and_increment(&string, input[i], &i);
			while (input[i] && input[i] != quote)
				append_and_increment(&string, input[i], &i);
			append_and_increment(&string, input[i], &i);
			continue ;
		}
		append_and_increment(&string, input[i], &i);
	}
	*index = i;
	return (string);
}

char	token_redir_helper(char **string, char *input, int *index)
{
	int		i;
	char	type;

	i = *index;
	append_character(string, input[i]);
	type = input[i];
	if (input[i + 1] == input[i] && input[i] == '<')
	{
		append_character(string, input[i]);
		i++;
		type = 'h';
	}
	else if (input[i + 1] == input[i] && input[i] == '>')
	{
		append_character(string, input[i]);
		i++;
		type = 'a';
	}
	*index = i;
	return (type);
}
