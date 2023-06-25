/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocate_files.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 17:58:20 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/24 20:18:31 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	init_stuff(t_files **current_files, t_files *file_list,
	t_files **new)
{
	*current_files = file_list;
	(*new) = malloc(sizeof(t_files));
	(*new)->next = NULL;
	(*new)->filename = NULL;
	(*new)->del = NULL;
	(*new)->open = 1;
	(*new)->expand = 1;
}

static void	here_doc_case(int *index, t_env *env, t_files *new,
	t_token **tokens)
{
	char	*temp;

	temp = tokens[*index]->value;
	if (ft_strchr(tokens[*index]->value, '\"')
		|| ft_strchr(tokens[*index]->value, '\''))
		new->expand = 0;
	tokens[*index]->value = clean_command(temp, env);
	free(temp);
	new->del = ft_strdup(tokens[*index]->value);
	new->type = 'h';
	new->fd = -1;
	++(*index);
}

static void	redirection_case(t_token **tokens, t_env *env, int *index,
	t_files *new)
{
	char	*temp;
	int		i;

	i = *index;
	temp = tokens[i]->value;
	tokens[i]->value = clean_command(temp, env);
	free(temp);
	new->filename = ft_strdup(tokens[i]->value);
	new->type = tokens[i - 1]->type;
	new->fd = -1;
	++(*index);
}

static void	last_part(t_files **current_files, t_files *new,
	t_files **file_list)
{
	t_files	*start;

	if (!*current_files)
	{
		*current_files = new;
		*file_list = *current_files;
	}
	else
	{
		start = *current_files;
		while ((*current_files)->next)
			*current_files = (*current_files)->next;
		(*current_files)->next = new;
		*file_list = start;
	}
}

t_files	*allocate_files(t_token **tokens, int *index, t_files *file_list,
		t_env *env)
{
	int		i;
	t_files	*current_files;
	t_files	*new;

	i = *index;
	init_stuff(&current_files, file_list, &new);
	if (tokens[i]->type != 's')
	{
		write(2, "token after redirection is not a string\n", 41);
		free(new);
		return (NULL);
	}
	if (tokens[i - 1]->type == 'h')
		here_doc_case(&i, env, new, tokens);
	else if (tokens[i - 1]->type == '>' || tokens[i - 1]->type == '<'
		|| tokens[i - 1]->type == 'a')
		redirection_case(tokens, env, &i, new);
	last_part(&current_files, new, &file_list);
	*index = i;
	set_last_infile(file_list);
	set_last_outfile(file_list);
	return (file_list);
}
