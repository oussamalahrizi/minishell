/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_helpers2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 21:46:58 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/23 21:49:40 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	skip_spaces_tok(char *input, int *index)
{
	int	i;

	i = *index;
	while (input && is_space(input[i]))
		i++;
	*index = i;
}

void	output_pipe(t_tok *vars, char *input, Token **tokens)
{
	vars->string = ft_strdup("");
	append_character(&(vars->string), input[vars->i]);
	tokens[vars->k++] = new_token(input[vars->i], vars->string);
	vars->i += 2;
	free(vars->string);
}