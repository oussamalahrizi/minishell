/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 19:54:46 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 11:47:10 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	build_pwd(char *pwd_fail, t_env *env, int fd)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd && pwd_fail)
		pwd = ft_strdup(pwd_fail);
	if (pwd)
	{
		ft_putstr_fd(pwd, fd);
		write(1, "\n", fd);
		free(get_pwd_env(env)->value);
		get_pwd_env(env)->value = ft_strdup(pwd);
		free(pwd);
		g_global.exit_status = 0;
	}
	else
		error_cmd("getcwd : failed to get current directory .\n", 2);
}
