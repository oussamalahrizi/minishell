/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 08:19:28 by idelfag           #+#    #+#             */
/*   Updated: 2023/06/21 08:19:29 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

char **append_string(char **array, char *str)
{
	char **new;

	int i = 0;
	while (array[i])
		i++;
	new = malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (array[i])
	{
		new[i] = ft_strdup(array[i]);
		i++;
	}
	new[i++] = ft_strdup(str);
	new[i] = 0;
	return (new);
}

char **here_doc(char *del, int index)
{
	int i;
	char *str;
	char *doc_index;
	char **array;
	char **temp;

	i = 0;
	array = malloc(sizeof(char *));
	*array = 0;
	while(1)
	{
		doc_index = ft_itoa(index);
		doc_index = ft_strjoin(doc_index, "_here_doc> ");
		str = readline(doc_index);
		free(doc_index);
		if (!str)
			return (array);
		if (!ft_strcmp(del, str))
		{
			free(str);
			break ;
		}
		str = ft_strjoin(str, "\n");
		temp = array;
		array = append_string(array, str);
		free_double_char(temp);
		i++;
		free(str);
	}
	return (array);
}

void write_pipe(Command **cmd)
{
	int i = 0;
	files *node;
	files *last_file;
	int j;

	while (cmd[i])
	{
		node = cmd[i]->files;
		while (node)
		{
			if (pipe(node->here_doc_fd) == -1)
			{
				error_cmd("pipe for here_doc failed.\n", 1);
				return;
			}
			last_file = get_last_infile(node);
			if (last_file != node)
			{
				close(node->here_doc_fd[1]);
				node = node->next;
				continue;
			}
			if (node->type == 'h')
			{
				node->fd = node->here_doc_fd[0];
				j = 0;
				while(node->h_content[j])
				{
					write(node->here_doc_fd[1], node->h_content[j], ft_strlen(node->h_content[j]));
					j++;
				}
				break;
			}
			node =  node->next;
		}
		i++;
	}
}

void open_heredocs(Command **cmd)
{
	int i;
	files *node;
	int index = 1;
	
	i = 0;
	while(cmd[i])
	{
		node = cmd[i]->files;
		while(node)
		{
			if (node->type == 'h')
			{
				node->h_content = here_doc(node->del, index);
				index++;
			}
			node = node->next;
		}
		i++;
	}
	write_pipe(cmd);
}