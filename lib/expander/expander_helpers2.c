/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helpers2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 14:46:13 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/24 14:54:25 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	inside_double(t_exp *vars)
{
	append_character(&vars->new_token, vars->string[vars->i]);
	vars->i += 1;
	while (vars->string[vars->i] && vars->string[vars->i] != '\"')
	{
		if (vars->string[vars->i] == '$')
		{
			if (vars->string[vars->i + 1] == '"'
				|| is_space(vars->string[vars->i + 1]))
			{
				append_character(&vars->new_token, '$');
				vars->i++;
			}
			else
				handle_dollar(vars->j, vars);
		}
		else
		{
			append_character(&vars->new_token, vars->string[vars->i]);
			vars->i++;
		}
	}
	append_character(&vars->new_token, vars->string[vars->i]);
	vars->i++;
}

static void	inside_single(t_exp *vars)
{
	append_character(&vars->new_token, vars->string[vars->i]);
	vars->i++;
	while (vars->string[vars->i] && vars->string[vars->i] != '\'')
	{
		append_character(&vars->new_token, vars->string[vars->i]);
		vars->i++;
	}
	append_character(&vars->new_token, vars->string[vars->i]);
	vars->i++;
}

static void	cleanup_tokens(t_exp *vars)
{
	vars->temp = vars->tokens[vars->j + vars->new_index]->value;
	vars->tokens[vars->j + vars->new_index]->value = ft_strdup(vars->new_token);
	free(vars->temp);
}

void	general_loop(t_exp *vars)
{
	while (vars->string[0])
	{
		vars->i = 0;
		append_before_dollar(vars);
		if (vars->string[vars->i] == '$')
		{
			if (vars->j - 1 >= 0 && (vars->t_tokens[vars->j - 1]->type == '>'
					|| vars->t_tokens[vars->j - 1]->type == '<'
					|| vars->t_tokens[vars->j - 1]->type == 'a'))
				break ;
			handle_dollar_alone(vars);
		}
		else if (vars->string[vars->i] == '\"')
			inside_double(vars);
		else if (vars->string[vars->i] == '\'')
			inside_single(vars);
		if (!vars->string[vars->i] && ft_strcmp(vars->tokens[vars->j
					+ vars->new_index]->value, vars->new_token))
		{
			cleanup_tokens(vars);
			break ;
		}
		vars->string += vars->i;
	}
}
