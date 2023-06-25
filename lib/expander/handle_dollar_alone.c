/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar_alone.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 22:36:02 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/24 11:10:36 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_dollar_eof(t_dollar *vars, t_exp *exp, char *string, int *index)
{
	vars->skip = get_env("$", exp->env);
	if (string[vars->i] == '$')
		*index = vars->i + 1;
	else
		*index = vars->i;
}

void	extract_skip(char *string, t_dollar *vars, int *index, t_exp *exp)
{
	char	*temp;

	while (string[vars->i] && !is_space(string[vars->i])
		&& !is_quote(string[vars->i]) && string[vars->i] != '$'
		&& (ft_isalnum(string[vars->i]) || string[vars->i] == '?'
			|| string[vars->i] == '_'))
	{
		vars->len++;
		if (vars->i == *index + 1 && ft_isdigit(string[vars->i]))
		{
			vars->i++;
			break ;
		}
		if (string[vars->i] == '?')
		{
			vars->i++;
			break ;
		}
		vars->i++;
	}
	vars->skip = ft_substr(string, vars->start_index, vars->len);
	*index = vars->i;
	temp = vars->skip;
	vars->skip = get_env(vars->skip, exp->env);
	free(temp);
}

void	preparation(t_exp *exp, t_dollar *vars, int token_index, int *new_index)
{
	vars->token_size = 0;
	while (exp->tokens[vars->token_size])
		vars->token_size++;
	vars->j = 0;
	vars->split = ft_split(vars->skip, ' ');
	while (vars->split[vars->j])
		vars->j++;
	vars->split_size = vars->j;
	if (vars->skip[0] == 0)
		vars->j = 1;
	vars->new_tokens = malloc(sizeof(t_token *) * (vars->token_size + vars->j));
	vars->size = vars->token_size + vars->j - 1;
	vars->j = 0;
	while (vars->j < token_index + *new_index)
	{
		vars->new_tokens[vars->j] = new_token(exp->tokens[vars->j]->type,
				exp->tokens[vars->j]->value);
		vars->j++;
	}
}
