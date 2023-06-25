/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 17:15:48 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 14:16:41 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	calculate_cmds(t_token **tokens)
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
		vars->commands[i] = malloc(sizeof(t_command));
		vars->commands[i]->cmd = NULL;
		vars->commands[i]->cmd_args = malloc(sizeof(char *));
		vars->commands[i]->cmd_args[0] = NULL;
		vars->commands[i]->files = NULL;
		i++;
	}
}

static void	command_case(t_ext *vars, t_token **tokens, t_env *env)
{
	char	*temp;

	if (vars->commands[vars->k]->cmd == NULL)
	{
		temp = clean_command(tokens[vars->i]->value, env);
		vars->commands[vars->k]->cmd = ft_strdup(temp);
		free(temp);
	}
	temp = clean_command(tokens[vars->i]->value, env);
	vars->commands[vars->k]->cmd_args
		= append_string(vars->commands[vars->k]->cmd_args, temp);
	free(temp);
	vars->i++;
}

static void	general_loop_ext(t_ext *vars, t_token **tokens, t_env *env)
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

t_command	**extract(t_token **tokens, t_env *env)
{
	t_ext	vars;

	vars.i = 0;
	vars.k = 0;
	vars.num = calculate_cmds(tokens);
	vars.commands = malloc(sizeof(t_command *) * (vars.num + 1));
	init_stuff(&vars);
	general_loop_ext(&vars, tokens, env);
	vars.commands[vars.num] = 0;
	return (vars.commands);
}
