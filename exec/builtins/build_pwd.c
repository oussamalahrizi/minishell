/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 19:54:46 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/07 18:30:37 by olahrizi         ###   ########.fr       */
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
		write(1, pwd, ft_strlen(pwd));
		write(1, "\n", 1);
		free(pwd);
		exit_status = 0;
	}
}