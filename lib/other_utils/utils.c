/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 22:07:25 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 12:12:26 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_tokens_size(Token **tokens)
{
	int	len;

	len = 0;
	while (tokens[len])
		len++;
	return (len);
}

Token	**duplicate_tokens(Token **tokens)
{
	int		i;
	Token	**new_tokens;

	i = 0;
	new_tokens = malloc(sizeof(Token *) * (get_tokens_size(tokens) + 1));
	while (tokens[i])
	{
		new_tokens[i] = new_token(tokens[i]->type, tokens[i]->value);
		i++;
	}
	new_tokens[i] = 0;
	return (new_tokens);
}
