/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_helpers2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 17:55:17 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/24 20:27:33 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*first_case(char **str, t_env *env, char *string)
{
	*str = get_userhome(env);
	if (!*str[0])
	{
		error_cmd("minishell : HOME/USER/LOGNAME not set\n", 1);
		free(*str);
		return (ft_strdup(""));
	}
	*str = ft_strjoin(*str, &string[1]);
	return (*str);
}

static	void	second_case(char **str, char *string)
{
	char	*temp;
	int		i;
	int		j;
	int		save;

	i = 1;
	*str = ft_strdup("/Users/");
	while (string[i] && string[i] != '/')
		i++;
	temp = ft_substr(&string[1], 0, i);
	*str = ft_strjoin(*str, temp);
	j = 0;
	if (string[i])
	{
		i++;
		save = i;
		while (string[i++])
			j++;
		free(temp);
		temp = ft_substr(&string[save], 0, j);
		*str = ft_strjoin(*str, temp);
	}
	free(temp);
}

char	*expand_tilde(char *string, t_env *env)
{
	char	*str;

	if (string[0] == '~' && (string[1] == '/' || !string[1]))
		str = first_case(&str, env, string);
	else if (string[0] == '~' && string[1] != '/')
		second_case(&str, string);
	else
	{
		str = ft_strdup("");
		str = ft_strjoin(str, string);
	}
	free(string);
	return (str);
}
