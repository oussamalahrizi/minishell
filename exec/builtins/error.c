/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 19:57:58 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/04 20:02:46 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

extern int exit_status;

void error_cmd(char *str, int code)
{
	write(2, str, ft_strlen(str));
	exit_status = code;
}