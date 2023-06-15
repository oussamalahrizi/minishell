/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 19:54:46 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/15 23:47:03 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

extern int exit_status;

void build_pwd()
{
	char *pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		error_cmd("cannot get pwd\n", 1);
	else
	{
		ft_putstr_fd(pwd, 1);
		write(1, "\n", 1);
		free(pwd);
		exit_status = 0;
	}
}