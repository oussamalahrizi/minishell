/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 05:15:31 by idelfag           #+#    #+#             */
/*   Updated: 2023/06/13 17:30:45 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../minishell.h"

void build_pwd(void)
{
	char *pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		ft_putendl_fd("failed to get pwd", 2);
	else
	{
		ft_putendl_fd(pwd, 1);
		free(pwd);
	}
}
