/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 10:21:28 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 11:16:30 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**append_string(char **array, char *str)
{
	char	**new;
	int		i;

	i = 0;
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

static void	case_dollar_exit(char **new_value, char *original, int *index,
		t_env *env)
{
	char	*skip;
	char	*temp;
	int		i;

	i = *index;
	skip = ft_substr(original, i, 1);
	temp = skip;
	skip = get_env(skip, env);
	free(temp);
	*new_value = ft_strjoin(*new_value, skip);
	free(skip);
	i++;
	*index = i;
}

static void	expand_helper(int *index, char *original, char **new_value,
		t_env *env)
{
	int		i;
	int		start_index;
	int		len;
	char	*skip;
	char	*temp;

	len = 0;
	i = *index;
	start_index = i;
	while (original[i] && original[i] != '$' && !is_space(original[i])
		&& !is_quote(original[i]) && (ft_isalnum(original[i])
			|| original[i] == '_'))
	{
		i++;
		len++;
	}
	skip = ft_substr(original, start_index, len);
	temp = skip;
	skip = get_env(skip, env);
	free(temp);
	*new_value = ft_strjoin(*new_value, skip);
	free(skip);
	*index = i;
}

static void	here_doc_loop(char *original, char **new_value, t_env *env)
{
	int	i;

	while (*original)
	{
		i = 0;
		while (original[i] && original[i] != '$')
		{
			append_character(new_value, original[i]);
			i++;
		}
		if (!original[i])
			break ;
		i++;
		if (original[i] == '?' || original[i] == '$')
			case_dollar_exit(new_value, original, &i, env);
		else if (ft_isdigit(original[i]))
		{
			append_character(new_value, original[i]);
			i++;
		}
		else
			expand_helper(&i, original, new_value, env);
		original += i;
	}
}

void	expand_value(char **string, t_env *env)
{
	int		i;
	char	*new_value;
	char	*original;

	i = 0;
	new_value = ft_strdup("");
	original = *string;
	here_doc_loop(original, &new_value, env);
	free(*string);
	*string = new_value;
}
