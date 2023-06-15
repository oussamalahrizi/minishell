/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 19:57:58 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/13 11:10:37 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

extern int exit_status;

void error_cmd(char *str, int code)
{
	write(2, str, ft_strlen(str));
	exit_status = code;
}
