/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_two.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 02:38:18 by idelfag           #+#    #+#             */
/*   Updated: 2023/06/25 10:32:18 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

char	*trim_for_join(char const *s1, char const *set)
{
	size_t	i;

	if (!s1)
		return (0);
	if (!set)
		return ((char *)s1);
	i = ft_strlen(s1) - 1;
	while (s1[i] && ft_strrchr(set, s1[i]))
		i--;
	return (ft_substr(s1, 0, i + 1));
}

void	print_err_syntax(char *name)
{
	ft_putstr_fd("export: ", 2);
	ft_putchar_fd('`', 2);
	ft_putstr_fd(name, 2);
	ft_putchar_fd('\'', 2);
	ft_putendl_fd(" : not a valid identifier", 2);
	g_global.exit_status = 1;
}

void	joining(char *name, t_env *env, char *value, int flag)
{
	t_env	*node;
	t_env	*last;

	node = env;
	while (node)
	{
		if (!ft_strcmp(name, node->name))
		{
			node->value = ft_strjoin(node->value, value);
			break ;
		}
		node = node->next;
	}
	if (!node)
	{
		last = malloc(sizeof(t_env));
		last->name = ft_strdup(name);
		last->value = ft_strdup(value);
		last->flag = flag;
		last->next = NULL;
		env_add_back(&env, last);
	}
}

void	print_cases(char *name, char *value, int fd, int flag)
{
	if (flag == 1)
	{
		ft_putstr_fd("declare -x ", fd);
		ft_putstr_fd(name, fd);
		ft_putchar_fd('=', fd);
		ft_putchar_fd('\"', fd);
		ft_putstr_fd(value, fd);
		ft_putendl_fd("\"", fd);
	}
	if (flag == 2)
	{
		ft_putstr_fd("declare -x ", fd);
		ft_putstr_fd(name, fd);
		ft_putchar_fd('=', fd);
		ft_putchar_fd('\"', fd);
		ft_putendl_fd("\"", fd);
	}
	if (flag == 3)
	{
		ft_putstr_fd("declare -x ", fd);
		ft_putendl_fd(name, fd);
	}
}
