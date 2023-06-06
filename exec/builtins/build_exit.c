/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 18:55:27 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/06 22:36:30 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


extern int exit_status;

void skip_space(char *str, int *index)
{
	int i = 0;
	while (str[i] && is_space(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	*index = i;
}

int calculate_exit_value(int nbr, int *status)
{
	if (nbr < 0)
	{
		if (nbr < -255)
		{
			while (nbr < 255)
				nbr += 256;
		}
		else
			nbr += 256;
	}
	else if (nbr > 255)
	{
		while(nbr > 255)
			nbr -= 256;
	}
	nbr = status[nbr];
	return (nbr);
}

void numeric_argument()
{
	write(2, "exit\n", 6);
	error_cmd("exit : numeric argument required\n", 255);
	exit_status = 255;
	exit(255);
}

void too_many_arguments()
{
	write(2, "exit\n", 6);
	write(2, "exit : too many arguments\n", 26);
	exit_status = 1;
	exit(1);
}

void	build_exit(char **cmd_args)
{
	int *status;
	int nbr;

	status = malloc(sizeof(int) * 256);
	int i = 0;
	while (i < 256)
	{
		status[i] = i;
		i++;
	}
	int len = count_args(cmd_args);
	if (len == 1)
	{
		write(2, "exit\n", 6);
		exit_status = 0;
		exit(0);
	}
	else if (len == 2)
	{
		i = 0;
		skip_space(cmd_args[1], &i);
		if (!cmd_args[1][i])
			numeric_argument();
		while (cmd_args[1][i] && ft_isdigit(cmd_args[1][i]))
			i++;
		if (cmd_args[1][i])
			numeric_argument();
		else
		{
			nbr = ft_atoi(cmd_args[1]);
			nbr = calculate_exit_value(nbr, status);
				write(2, "exit\n", 6);
			exit_status = nbr;
			exit(nbr);
		}
	}
	else
	{
		i = 0;
		skip_space(cmd_args[1], &i);
		if (!cmd_args[1][i])
			numeric_argument();
		while (cmd_args[1][i] && ft_isdigit(cmd_args[1][i]))
			i++;
		if (cmd_args[1][i])
			numeric_argument();
		else
			too_many_arguments();
	}
}