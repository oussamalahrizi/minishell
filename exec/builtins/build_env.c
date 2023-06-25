/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 00:56:19 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 10:04:54 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../minishell.h"

void build_env(t_env *env, int fd_out)
{
    t_env *node;

    node = env;
    while(node)
    {
        if (!ft_strcmp("PATH", node->name))
            break;
        node = node->next;
    }
    if (!node)
    {
        error_cmd("env: No such file or directory\n", 127);
        return;
    }
    while(env)
    {
        if (env->flag)
        {
            ft_putstr_fd(env->name, fd_out);
            ft_putchar_fd('=', fd_out);
            ft_putstr_fd(env->value, fd_out);
            ft_putchar_fd('\n', fd_out);
        }
        env = env->next;
    }
}