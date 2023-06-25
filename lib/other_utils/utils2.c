/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 22:15:09 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/24 22:16:49 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_del(char *del, char c)
{
	int	i;

	i = 0;
	while (del[i])
	{
		if (del[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static size_t	get_total_words(const char *str, char *del)
{
	size_t	_string_count;

	_string_count = 0;
	while (*str)
	{
		while (*str && check_del(del, *str))
			str++;
		if (*str)
			_string_count++;
		while (*str && !check_del(del, *str))
			str++;
	}
	return (_string_count);
}

static char	**free_stuff(char **strings)
{
	size_t	i;

	i = 0;
	while (strings[i])
	{
		free(strings[i]);
		strings[i] = NULL;
		i++;
	}
	free(strings);
	strings = NULL;
	return (strings);
}

static size_t	get_len(char const *s, char *del)
{
	size_t	i;

	i = 0;
	while (s[i] && !check_del(del, s[i]))
	{
		i++;
	}
	return (i);
}

char	**split_by_str(char *s, char *del)
{
	char	**strings;
	size_t	index;

	if (!s)
		return (NULL);
	strings = (char **)malloc(sizeof(char *) * (get_total_words(s, del) + 1));
	if (!strings)
		return (NULL);
	index = 0;
	while (*s)
	{
		if (check_del(del, *s))
			s++;
		else
		{
			strings[index++] = ft_substr(s, 0, get_len(s, del));
			if (!strings[index - 1])
				return (free_stuff(strings));
			s += get_len(s, del);
		}
	}
	strings[index] = 0;
	return (strings);
}
