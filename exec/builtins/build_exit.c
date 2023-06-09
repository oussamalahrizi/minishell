/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 18:55:27 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 11:22:54 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	check_valid(char **str)
{
	int	i;

	i = 0;
	while (str[1][i] && str[1][i] == 32 && str[1][i] == '\t')
		i++;
	if (str[1][i] == '-' || str[1][i] == '+')
		i++;
	while (str[1][i])
	{
		if (!ft_isdigit(str[1][i]))
			return (0);
		i++;
	}
	return (1);
}

int	check_args(char **str)
{
	if (count_args(str) > 2)
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (0);
	}
	return (1);
}

int	get_exitstatus(int nbr)
{
	if (nbr > 255)
	{
		while (nbr > 255)
			nbr -= 256;
		ft_putstr_fd("exit\n", 1);
		return (nbr);
	}
	if (nbr < 0)
	{
		if (nbr < -255)
		{
			while (nbr < -255)
				nbr += 256;
		}
		nbr += 256;
		ft_putstr_fd("exit\n", 2);
		return (nbr);
	}
	ft_putstr_fd("exit\n", 2);
	return (nbr);
}

void	build_exit(char **str)
{
	int	nbr;

	if (count_args(str) == 1)
	{
		ft_putstr_fd("exit\n", 1);
		g_global.exit_status = 0;
		exit(0);
	}
	nbr = ft_atoi(str[1]);
	if (!check_valid(str))
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("exit: numeric argument required\n", 2);
		g_global.exit_status = 255;
		exit(255);
	}
	if (!check_args(str))
	{
		g_global.exit_status = 1;
		return ;
	}
	g_global.exit_status = get_exitstatus(nbr);
	exit(g_global.exit_status);
}
