/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 14:37:04 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 10:51:29 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

extern t_global	g_global;

char	*get_env(char *string, t_env *env)
{
	char	*variable;
	char	*temp;
	t_env	*node;

	if (*string == '?')
		return (ft_itoa(g_global.exit_status));
	if (*string == '$')
		return (ft_strdup("$"));
	if (!ft_strlen(string))
		return (ft_strdup(""));
	temp = string;
	node = env;
	while (node)
	{
		if (!ft_strcmp(temp, node->name))
		{
			variable = ft_strdup(node->value);
			return (variable);
		}
		node = node->next;
	}
	variable = ft_strdup("");
	return (variable);
}

static void	skip_not_string(t_exp *vars)
{
	while (vars->t_tokens[vars->j]->type != 's')
		vars->j++;
}

void	append_before_dollar(t_exp *vars)
{
	while (vars->string[vars->i] && vars->string[vars->i] != '$'
		&& !is_quote(vars->string[vars->i]))
	{
		append_character(&vars->new_token, vars->string[vars->i]);
		vars->i++;
	}
}

void	expander(Token ***tokens_i, t_env *env)
{
	t_exp	vars;

	vars.env = env;
	vars.j = 0;
	vars.new_index = 0;
	vars.t_tokens = *tokens_i;
	vars.tokens = duplicate_tokens(vars.t_tokens);
	while (vars.t_tokens[vars.j])
	{
		skip_not_string(&vars);
		if (vars.j - 1 >= 0 && vars.t_tokens[vars.j - 1]->type == 'h'
			&& vars.t_tokens[vars.j]->type == 's')
		{
			vars.j++;
			continue ;
		}
		vars.string = vars.t_tokens[vars.j]->value;
		vars.new_token = ft_strdup("");
		general_loop(&vars);
		free(vars.new_token);
		vars.j++;
	}
	free_double(*tokens_i);
	*tokens_i = duplicate_tokens(vars.tokens);
	free_double(vars.tokens);
}
