/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ifs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 11:21:49 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/24 22:14:36 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_ifs(char *string)
{
	int		count;
	char	**splited;

	count = 0;
	if (!*string)
		return (-1);
	splited = split_by_str(string, " \t\n");
	while (splited[count])
		count++;
	free_double_char(splited);
	if (count > 1)
		return (-1);
	return (0);
}
