/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 21:45:51 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/24 17:26:19 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	another_helper(t_tok *vars, char *input, Token **tokens)
{
	vars->type = token_redir_helper(&(vars->string), input, &vars->i);
	tokens[vars->k++] = new_token(vars->type, vars->string);
	vars->i++;
	free(vars->string);
}

static void	another_last_helper(char *input, t_tok *vars, Token **tokens)
{
	vars->string = third_part(input, &vars->i, vars->delimiters);
	tokens[vars->k++] = new_token('s', vars->string);
	free(vars->string);
}

void	quotes_helper(char *input, t_tok *vars, Token **tokens)
{
	vars->string = skip_quotes_tok(input, &vars->i, vars->delimiters);
	tokens[vars->k++] = new_token('s', vars->string);
	free(vars->string);
}

static void	general_loop_tok(t_tok *vars, Token **tokens, char *input)
{
	while (input[vars->i])
	{
		skip_spaces_tok(input, &vars->i);
		if (!input[vars->i])
			break ;
		if (ft_strchr("'\"", input[vars->i]))
		{
			quotes_helper(input, vars, tokens);
			continue ;
		}
		else if (ft_strchr(vars->delimiters, input[vars->i]))
		{
			vars->string = ft_strdup("");
			if (input[vars->i] == '>' && input[vars->i + 1] == '|')
			{
				output_pipe(vars, input, tokens);
				continue ;
			}
			another_helper(vars, input, tokens);
			continue ;
		}
		another_last_helper(input, vars, tokens);
	}
}

void	tokenize(char *input, Token **tokens)
{
	t_tok	vars;

	vars.delimiters = ft_strdup("|><");
	vars.i = 0;
	vars.k = 0;
	general_loop_tok(&vars, tokens, input);
	free(vars.delimiters);
	tokens[vars.k] = NULL;
}
