/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 11:27:00 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 16:05:47 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

extern t_global	g_global;

static int	case_input(t_op_files *vars, t_env *env)
{
	if (check_ambiguous(vars, env) == -1)
		return (-1);
	vars->try = open(vars->node->filename, O_RDONLY);
	if (vars->try == -1)
	{
		close_files(vars->new_fds, vars->count);
		error_message_files(vars->node->filename);
		vars->failure = -1;
		return (-1);
	}
	else
	{
		vars->new_fds[vars->count++] = vars->try;
		if (vars->node->open == -1)
		{
			if (close(vars->try))
				error_cmd("close failed\n", 1);
		}
		else
			vars->node->fd = vars->try;
	}
	return (0);
}

static int	case_output(t_op_files *vars, t_env *env)
{
	if (check_ambiguous(vars, env) == -1)
		return (-1);
	vars->try = open(vars->node->filename, O_TRUNC | O_CREAT | O_RDWR, 0644);
	if (vars->try == -1)
	{
		close_files(vars->new_fds, vars->count);
		error_message_files(vars->node->filename);
		vars->failure = -1;
		return (-1);
	}
	else
	{
		vars->new_fds[vars->count++] = vars->try;
		if (vars->node->open == -1)
		{
			if (close(vars->try))
				error_cmd("close failed\n", 1);
		}
		else
			vars->node->fd = vars->try;
	}
	return (0);
}

static int	case_append(t_op_files *vars, t_env *env)
{
	if (check_ambiguous(vars, env) == -1)
		return (-1);
	vars->try = open(vars->node->filename, O_CREAT | O_APPEND | O_RDWR, 0644);
	if (vars->try == -1)
	{
		close_files(vars->new_fds, vars->count);
		error_message_files(vars->node->filename);
		vars->failure = -1;
		return (-1);
	}
	else
	{
		vars->new_fds[vars->count++] = vars->try;
		if (vars->node->open == -1)
		{
			if (close(vars->try))
				error_cmd("close failed\n", 1);
		}
		else
			vars->node->fd = vars->try;
	}
	return (0);
}

static void	init_stuff(t_op_files *vars, t_files *file_list)
{
	vars->i = 0;
	vars->count = 0;
	vars->failure = 0;
	vars->count = count_files(file_list);
	vars->new_fds = malloc(sizeof(int) * vars->count);
	vars->i = 0;
	vars->count = 0;
	vars->node = file_list;
}

int	open_files(t_files *file_list, t_env *env)
{
	t_op_files	vars;

	init_stuff(&vars, file_list);
	while (vars.node)
	{
		if (vars.node->type == '<' && case_input(&vars, env) == -1)
			break ;
		else if (vars.node->type == '>' && case_output(&vars, env) == -1)
			break ;
		else if (vars.node->type == 'a' && case_append(&vars, env) == -1)
			break ;
		vars.node = vars.node->next;
	}
	free(vars.new_fds);
	return (vars.failure);
}
