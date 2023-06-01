#include "../minishell.h"

extern int exit_status;

int error(char *str)
{
	write(2, str, ft_strlen(str));
	exit_status = 1;
	return (1);
}

int check_del(char *del, char c)
{
	int i = 0;
	while (del[i])
	{
		if (del[i] == c)
			return (1);
		i++;
	}
	return (0);
}

size_t	get_total_words(const char *str, char *del)
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

char	**free_stuff(char **strings)
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

size_t	get_len(char const *s, char *del)
{
	size_t	i;

	i = 0;
	while (s[i] && !check_del(del, s[i]))
	{
		i++;
	}
	return (i);
}

char **split_by_str(char *s, char *del)
{
	char	**strings;
	size_t	index;
	int j;

	if (!s)
		return (NULL);
	strings = (char **) malloc(sizeof(char *) * (get_total_words(s, del) + 1));
	if (!strings)
		return (NULL);
	index = 0;
	j = 0;
	while (*s)
	{
		j = 0;
		while (*s && del[j] && del[j] == *s)
		{
			s++;
			j++;
		}
		strings[index++] = ft_substr(s, 0, get_len(s, del));
		if (!strings[index - 1])
			return (free_stuff(strings));
		s += get_len(s, del);
	}
	strings[index] = 0;
	j = 0;
	return (strings);
}