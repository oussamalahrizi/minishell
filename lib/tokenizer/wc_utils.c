/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wc_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 13:53:28 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 12:09:31 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	count_the_rest(char *input, char *delimiters, int *index)
{
	int		i;
	char	quote;

	i = *index;
	while (input[i] && !ft_strchr(delimiters, input[i]) && !is_space(input[i]))
	{
		if (ft_strchr("'\"", input[i]))
		{
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
			continue ;
		}
		i++;
	}
	*index = i;
	return (0);
}

Token	*new_token(char type, char *value)
{
	Token	*token;

	token = (Token *)malloc(sizeof(Token));
	token->type = type;
	token->value = ft_strdup(value);
	return (token);
}

void	init_stuff_wc(t_wc *vars)
{
	vars->k = 0;
	vars->i = 0;
	vars->delimiters = ft_strdup("|><");
}
