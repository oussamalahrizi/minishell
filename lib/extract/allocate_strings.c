/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocate_strings.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 17:58:18 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 12:09:16 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	init_stuff(int *index, Token **tokens, char ***res)
{
	int	i;
	int	c;

	i = *index;
	c = 0;
	while (tokens[i] && tokens[i]->type == 's')
	{
		if (ft_strlen(tokens[i]->value) == 0)
		{
			i++;
			continue ;
		}
		i++;
		c++;
	}
	*res = malloc(sizeof(char *) * (c + 1));
	*index = i;
}

char	**allocate_strings(Token **tokens, int *index, t_env *env)
{
	int		i;
	char	**res;
	int		c;

	i = *index;
	init_stuff(&i, tokens, &res);
	c = 0;
	i = *index;
	while (tokens[i] && tokens[i]->type == 's')
	{
		if (ft_strlen(tokens[i]->value) == 0)
		{
			i++;
			continue ;
		}
		res[c] = clean_command(tokens[i]->value, env);
		c++;
		i++;
	}
	*index = i;
	res[c] = 0;
	return (res);
}
