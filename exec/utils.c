/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 11:27:00 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/22 09:42:30 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int exit_status;

char *expand_file(char *string, t_env *env)
{
	char *skip;
	char *temp;
	char *rest = ft_strdup("");
	int c;
	if (!string[0])
		return (rest);
	int i = 1;
	while (string[i] && string[i] != '$')
		i++;
	c = i;
	if (string[i] == '$')
		c = i - 1;
	skip = ft_substr(string, 1, c);
	temp = skip;
	skip = get_env(skip, env);
	free(temp);
	if (string[i] == '$')
		rest = expand_file(string + i, env);
	skip = ft_strjoin(skip, rest);
	free(rest);
	return (skip);
}

void close_files(int *files, int size)
{
	int i = 0;
	while (i < size - 1)
	{
		close(files[size]);
		i++;
	}
}

int open_files(files *file_list, t_env *env)
{
	int i = 0;
	files *node;
	int count = 0;
	int *new_fds;
	int try;
	int failure = 0;
	char *expanded;
	node = file_list;
	while (node)
	{
		count++;
		node = node->next;
	}
	new_fds = malloc(sizeof(int) * count);
	i = 0;
	count = 0;
	node = file_list;
	while (node)
	{
		if (node->type == '<')
		{
			if (node->filename[0] == '$')
			{
				expanded = expand_file(node->filename, env);
				if (check_ifs(expanded) == -1 || *expanded == 0)
				{
					error_cmd("ambiguous redirect\n", 1);
					free(expanded);
					failure = -1;
					break;
				}
				else
				{
					free(node->filename);
					node->filename = expanded;
				}
			}
			try = open(node->filename, O_RDONLY);
			if (try == -1)
			{
				close_files(new_fds, count);
				write(2, "minishell: ", 12);
				ft_putstr_fd(node->filename, 2);
				write(2, ": ", 3);
				perror("");
				global.exit_status = 1;
				failure = -1;
				break;
			}
			else
			{
				new_fds[count++] = try;
				if (node->open == -1)
				{
					if (close(try))
						error_cmd("close failed\n", 1);
				}
				else
					node->fd = try;
			}
		}
		else if (node->type == '>')
		{
			if (node->filename[0] == '$')
			{
				expanded = expand_file(node->filename, env);
				if (check_ifs(expanded) == -1 || *expanded == 0)
				{
					error_cmd("ambiguous redirect\n", 1);
					free(expanded);
					failure = -1;
					break;
				}
				else
				{
					free(node->filename);
					node->filename = expanded;
				}
			}
			try = open(node->filename, O_TRUNC | O_CREAT | O_RDWR, 0644);
			if (try == -1)
			{
				close_files(new_fds, count);
				write(2, "minishell: ", 12);
				ft_putstr_fd(node->filename, 2);
				write(2, ": ", 3);
				perror("");
				global.exit_status = 1;
				failure = -1;
				break;
			}
			else
			{
				new_fds[count++] = try;
				if (node->open == -1)
				{
					if (close(try))
						error_cmd("close failed\n", 1);
				}
				else
					node->fd = try;
			}
		}
		else if (node->type == 'a')
		{
			if (node->filename[0] == '$')
			{
				expanded = expand_file(node->filename, env);
				if (check_ifs(expanded) == -1 || *expanded == 0)
				{
					free(expanded);
					failure = -1;
					break;
				}
				else
				{
					free(node->filename);
					node->filename = expanded;
				}
			}
			try = open(node->filename, O_CREAT | O_APPEND | O_RDWR, 0644);
			if (try == -1)
			{
				close_files(new_fds, count);
				write(2, "minishell: ", 12);
				ft_putstr_fd(node->filename, 2);
				write(2, ": ", 3);
				perror("");
				global.exit_status = 1;
				failure = -1;
				break;
			}
			else
			{
				new_fds[count++] = try;
				if (node->open == -1)
				{
					if (close(try))
						error_cmd("close failed\n", 1);
				}
				else
					node->fd = try;
			}
		}
		node = node->next;
	}
	free(new_fds);
	return (failure);
}
