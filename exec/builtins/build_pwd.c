/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 19:54:46 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/22 02:01:38 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

extern int exit_status;

void build_pwd(char *pwd_fail, t_env *env)
{
	char *pwd;
	char *pwd_env;

	pwd = getcwd(NULL, 0);
	if (!pwd && pwd_fail)
		pwd = ft_strdup(pwd_fail);
	if (pwd)
	{
		ft_putstr_fd(pwd, 1);
		write(1, "\n", 1);
		pwd_env = get_pwd_env(env)->value;
		get_pwd_env(env)->value = ft_strdup(pwd);
		free(pwd);
		global.exit_status = 0;
	}
	else
		error_cmd("getcwd : failed to get current directory .\n", 1);
}