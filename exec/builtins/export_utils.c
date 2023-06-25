/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 03:48:58 by idelfag           #+#    #+#             */
/*   Updated: 2023/06/25 10:35:33 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	check_syntax_export(char *cmd)
{
	int	i;

	i = 0;
	if (!ft_isalpha(cmd[i]) && cmd[i] != '_')
		return (0);
	i++;
	while (cmd[i])
	{
		if (ft_isalpha(cmd[i]) || ft_isdigit(cmd[i]) || cmd[i] == '_')
			i++;
		else
			return (0);
	}
	return (1);
}

void	sortby_ascii(char **strings, int numstrings)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < numstrings - 1)
	{
		j = i + 1;
		while (j < numstrings)
		{
			if (ft_strcmp(strings[i], strings[j]) > 0)
			{
				temp = strings[i];
				strings[i] = strings[j];
				strings[j] = temp;
			}
			j++;
		}
		i++;
	}
}

char	**get_env_tab(t_env *env, int *i)
{
	char	**array;
	t_env	*node;

	*i = 0;
	node = env;
	while (node)
	{
		(*i)++;
		node = node->next;
	}
	array = malloc(sizeof(char *) * (*i + 1));
	node = env;
	*i = 0;
	while (node)
	{
		array[*i] = malloc(sizeof(char) * (ft_strlen(node->name) + 1));
		ft_strlcpy(array[*i], node->name, (ft_strlen(node->name) + 1));
		node = node->next;
		(*i)++;
	}
	array[*i] = NULL;
	return (array);
}

void	print_util(t_env *env, char *array, int fd)
{
	t_env	*node;

	node = env;
	while (ft_strcmp(node->name, array))
		node = node->next;
	if (!ft_strcmp(node->name, array))
	{
		if (ft_strcmp(node->value, "") && node->flag)
			print_cases(node->name, node->value, fd, 1);
		else if (node->flag && !ft_strcmp(node->value, ""))
			print_cases(node->name, node->value, fd, 2);
		else if (!ft_strcmp(node->value, "") && !node->flag)
			print_cases(node->name, node->value, fd, 3);
	}
}

void	print_export(t_env *env, int fd)
{
	char	**array;
	int		i;

	array = get_env_tab(env, &i);
	sortby_ascii(array, i);
	i = 0;
	while (array[i])
	{
		print_util(env, array[i], fd);
		i++;
		continue ;
	}
	free_double_char(array);
}
