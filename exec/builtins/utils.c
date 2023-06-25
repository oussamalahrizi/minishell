/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 20:33:03 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 10:38:39 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	count_args(char **cmd_args)
{
	int	len;

	len = 0;
	while (cmd_args[len])
		len++;
	return (len);
}

int	get_flag(char *cmd)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (cmd[i])
	{
		if (cmd[i] == '=')
			flag = 1;
		i++;
	}
	return (flag);
}

int	check_for_sign(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '+')
		{
			if (cmd[i + 1] == '=')
				return (1);
		}
		i++;
	}
	return (0);
}

void	error_cmd(char *str, int code)
{
	write(2, str, ft_strlen(str));
	g_global.exit_status = code;
}
