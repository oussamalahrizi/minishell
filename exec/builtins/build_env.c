/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 00:56:19 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/13 13:50:50 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../minishell.h"

extern int exit_status;

void build_env(t_env *env)
{
    while(env)
    {
        if (env->flag)
        {
            ft_putstr_fd(env->name, 1);
            ft_putchar_fd('=', 1);
            ft_putstr_fd(env->value, 1);
            ft_putchar_fd('\n', 1);
        }
        env = env->next;
    }
    exit_status = 0;
}
