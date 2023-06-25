/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 02:22:21 by idelfag           #+#    #+#             */
/*   Updated: 2023/06/25 21:22:48 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	check_n_syntax(char **cmd_args, int *flag)
{
	int	i;
	int	j;

	j = 0;
	i = 1;
	while (cmd_args[i])
	{
		j = 0;
		if (!ft_strcmp(cmd_args[i], "-n"))
			*flag = 0;
		if (ft_strcmp(cmd_args[i], "-n"))
		{
			j++;
			while (cmd_args[i][j] == 'n' && cmd_args[i])
				j++;
			if (!cmd_args[i][j])
				*flag = 0;
			else
				return (i);
		}
		i++;
	}
	return (i);
}

void	build_echo(char **cmd_args, int fd)
{
	int		i;
	char	*str;
	int		flag;

	flag = 1;
	i = 1;
	if (count_args(cmd_args) == 1)
	{
		write(fd, "\n", 1);
		return ;
	}
	if (!ft_strncmp(cmd_args[i], "-n", 2))
		i = check_n_syntax(cmd_args, &flag);
	while (cmd_args[i])
	{
		str = ft_strdup(cmd_args[i]);
		ft_putstr_fd(str, fd);
		i++;
		if (cmd_args[i])
			write(fd, " ", 1);
		free(str);
	}
	if (flag)
		write(fd, "\n", 1);
	g_global.exit_status = 0;
}
