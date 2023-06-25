/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 19:54:46 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 20:17:17 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	build_pwd(t_env *pwd_fail, t_env *env, int fd)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	(void) *env;
	if (!pwd && pwd_fail)
		pwd = ft_strdup(pwd_fail->value);
	if (pwd)
	{
		ft_putstr_fd(pwd, fd);
		write(1, "\n", fd);
		if (pwd_fail)
		{
			free(pwd_fail->value);
			pwd_fail->value = ft_strdup(pwd);
		}
		free(pwd);
		g_global.exit_status = 0;
	}
	else
		error_cmd("getcwd : failed to get current directory .\n", 2);
}
