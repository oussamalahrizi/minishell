/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar_quotes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 12:38:17 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/24 14:55:10 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	first_case(char *string, t_exp *exp, int *index, t_quote *vars)
{
	vars->skip = get_env("$", exp->env);
	if (string[vars->i] == '$')
		*index = vars->i + 1;
	else
		*index = vars->i;
}

static void	second_case(char *string, t_quote *vars, t_exp *exp, int *index)
{
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
	vars->temp = vars->skip;
	vars->skip = get_env(vars->skip, exp->env);
	free(vars->temp);
}

void	handle_dollar(int token_index, t_exp *exp)
{
	t_quote	vars;

	vars.skip = NULL;
	vars.i = exp->i + 1;
	vars.start_index = vars.i;
	vars.len = 0;
	if (exp->string[vars.i] == '$')
		first_case(exp->string, exp, &exp->i, &vars);
	else
		second_case(exp->string, &vars, exp, &exp->i);
	exp->new_token = ft_strjoin(exp->new_token, vars.skip);
	free(vars.skip);
	vars.old = exp->tokens[token_index + exp->new_index]->value;
	exp->tokens[token_index
		+ exp->new_index]->value = ft_strdup(exp->new_token);
	free(vars.old);
}
