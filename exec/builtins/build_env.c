/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 00:56:19 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/20 02:13:35 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../minishell.h"

void build_env(t_env *env)
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
            ft_putstr_fd(env->name, 1);
            ft_putchar_fd('=', 1);
            ft_putstr_fd(env->value, 1);
            ft_putchar_fd('\n', 1);
        }
        env = env->next;
    }
}