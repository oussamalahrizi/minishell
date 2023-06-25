/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 19:54:46 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 11:40:17 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

extern t_global g_global;

void build_pwd(char *pwd_fail, t_env *env, int fd_out)
{
	char *pwd;
	char *pwd_env;

	pwd = getcwd(NULL, 0);
	if (!pwd && pwd_fail)
		pwd = ft_strdup(pwd_fail);
	if (pwd)
	{
		ft_putstr_fd(pwd, fd_out);
		write(fd_out, "\n", 1);
		pwd_env = get_pwd_env(env)->value;
		free(get_pwd_env(env)->value);
		get_pwd_env(env)->value = ft_strdup(pwd);
		free(pwd);
		g_global.exit_status = 0;
	}
	else
		error_cmd("getcwd : failed to get current directory .\n", 1);
}