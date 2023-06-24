/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 17:15:48 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/24 20:16:17 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	calculate_cmds(Token **tokens)
{
	int	i;
	int	num;

	i = 0;
	num = 1;
	while (tokens[i])
	{
		if (tokens[i]->type == '|')
			num++;
		i++;
	}
	return (num);
}

static void	init_stuff(t_ext *vars)
{
	int	i;

	i = 0;
	while (i < vars->num)
	{
		vars->commands[i] = malloc(sizeof(Command));
		vars->commands[i]->cmd = NULL;
		vars->commands[i]->cmd_args = NULL;
		vars->commands[i]->files = NULL;
		i++;
	}
}

static void	command_case(t_ext *vars, Token **tokens, t_env *env)
{
	char	*temp;

	if (vars->commands[vars->k]->cmd == NULL)
	{
		temp = clean_command(tokens[vars->i]->value, env);
		vars->commands[vars->k]->cmd = ft_strdup(temp);
		free(temp);
	}
	else
		vars->commands[vars->k]->cmd_args = allocate_strings(tokens, &vars->i,
				env);
}

static void	general_loop_ext(t_ext *vars, Token **tokens, t_env *env)
{
	while (tokens[vars->i])
	{
		if (tokens[vars->i]->type == 's')
			command_case(vars, tokens, env);
		else
		{
			if (tokens[vars->i]->type == '|' || !tokens[vars->i])
			{
				vars->k++;
				if (tokens[vars->i] && tokens[vars->i]->type == '|')
					vars->i++;
				continue ;
			}
			else if (tokens[vars->i]->type == '>'
				|| tokens[vars->i]->type == '<'
				|| tokens[vars->i]->type == 'h'
				|| tokens[vars->i]->type == 'a')
			{
				vars->i++;
				if (tokens[vars->i]->type == 's')
					vars->commands[vars->k]->files = allocate_files(tokens,
							&vars->i, vars->commands[vars->k]->files, env);
			}
		}
	}
}

Command	**extract(Token **tokens, t_env *env)
{
	t_ext	vars;

	vars.i = 0;
	vars.k = 0;
	vars.num = calculate_cmds(tokens);
	vars.commands = malloc(sizeof(Command *) * (vars.num + 1));
	init_stuff(&vars);
	general_loop_ext(&vars, tokens, env);
	vars.commands[vars.num] = 0;
	return (vars.commands);
}
