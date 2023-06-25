/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 22:07:18 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/24 22:14:40 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	append_character(char **string, char c)
{
	char	*new;
	int		i;

	if (!*string)
		return ;
	if (!c)
		return ;
	new = malloc(sizeof(char) * (ft_strlen(*string) + 2));
	if (!new)
		return ;
	i = 0;
	while (string[0][i])
	{
		new[i] = string[0][i];
		i++;
	}
	new[i++] = c;
	new[i] = 0;
	free(*string);
	*string = new;
}

int	is_space(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

int	is_quote(char c)
{
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}
