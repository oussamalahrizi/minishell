/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 22:07:25 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/24 22:16:19 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_tokens_size(t_token **tokens)
{
	int	len;

	len = 0;
	while (tokens[len])
		len++;
	return (len);
}

t_token	**duplicate_tokens(t_token **tokens)
{
	int		i;
	t_token	**new_tokens;

	i = 0;
	new_tokens = malloc(sizeof(t_token *) * (get_tokens_size(tokens) + 1));
	while (tokens[i])
	{
		new_tokens[i] = new_token(tokens[i]->type, tokens[i]->value);
		i++;
	}
	new_tokens[i] = 0;
	return (new_tokens);
}

void	free_tokens(t_token **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		free(tokens[i]->value);
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

int	get_last_pipe(char *input)
{
	int	i;

	i = ft_strlen(input) - 1;
	while (is_space(input[i]))
	{
		if (i == 0)
			return (0);
		i--;
	}
	if (input[i] == '|')
	{
		i--;
		while (i > 0 && is_space(input[i]))
			i--;
		if ((input[i] == '<' || input[i] == '>') || i == 0)
			return (0);
		return (1);
	}
	return (0);
}
