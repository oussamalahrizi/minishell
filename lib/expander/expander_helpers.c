/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 22:06:05 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/24 14:32:02 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	skip_empty(char **new_token_value, t_dollar *vars)
{
	*new_token_value = ft_strjoin(*new_token_value, vars->skip);
	vars->new_tokens[vars->j] = new_token('s', *new_token_value);
	vars->j++;
}

void	skip_not_empty_helper(char **new_token_value, t_dollar *vars)
{
	free(*new_token_value);
	*new_token_value = ft_strdup(vars->split[vars->k_split - 1]);
}

void	skip_not_empty(t_dollar *vars, char **new_token_value)
{
	vars->k_split = 0;
	if (vars->split_size == 1)
	{
		*new_token_value = ft_strjoin(*new_token_value,
				vars->split[vars->k_split]);
		vars->new_tokens[vars->j] = new_token('s', *new_token_value);
		vars->k_split++;
		vars->j++;
	}
	else if (vars->split_size > 1)
	{
		*new_token_value = ft_strjoin(*new_token_value,
				vars->split[vars->k_split]);
		vars->new_tokens[vars->j] = new_token('s', *new_token_value);
		vars->j++;
		vars->k_split = 1;
		while (vars->k_split < vars->split_size)
		{
			vars->new_tokens[vars->j] = new_token('s',
					vars->split[vars->k_split]);
			vars->k_split++;
			vars->j++;
		}
		skip_not_empty_helper(new_token_value, vars);
	}
}

void	cleanup_dollar_alone(t_dollar *vars, t_exp *exp, int token_index,
		int *new_index)
{
	while (vars->j < vars->size)
	{
		vars->new_tokens[vars->j] = new_token(exp->tokens[token_index]->type,
				exp->tokens[token_index]->value);
		vars->j++;
		token_index++;
	}
	vars->new_tokens[vars->j] = 0;
	if (vars->skip[0])
		*new_index += vars->k_split - 1;
	vars->j = 0;
	free_double_char(vars->split);
	free(vars->skip);
	free_double(exp->tokens);
	exp->tokens = duplicate_tokens(vars->new_tokens);
	free_double(vars->new_tokens);
}

void	handle_dollar_alone(t_exp *exp)
{
	t_dollar	vars;

	vars.skip = NULL;
	vars.new_tokens = NULL;
	vars.i = exp->i + 1;
	vars.start_index = vars.i;
	vars.len = 0;
	if (exp->string[vars.i] == '$' || !exp->string[vars.i])
		handle_dollar_eof(&vars, exp, exp->string, &exp->i);
	else
		extract_skip(exp->string, &vars, &exp->i, exp);
	preparation(exp, &vars, exp->j, &exp->new_index);
	if (vars.skip[0] == 0)
		skip_empty(&exp->new_token, &vars);
	else
		skip_not_empty(&vars, &exp->new_token);
	vars.token_index = exp->j + exp->new_index + 1;
	cleanup_dollar_alone(&vars, exp, exp->j, &exp->new_index);
}
