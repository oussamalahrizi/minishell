/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 23:26:19 by idelfag           #+#    #+#             */
/*   Updated: 2023/06/25 10:33:10 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	export_if_n_exist(char *cmd, t_env *env, char **new)
{
	t_env	*last;

	last = malloc(sizeof(t_env));
	last->name = ft_strdup(new[0]);
	last->value = ft_strdup(new[1]);
	last->flag = get_env_flag(cmd);
	last->next = NULL;
	env_add_back(&env, last);
}

int	join_export(t_env *env, char *cmd, char **new)
{
	t_env	*node;
	char	*tmp;

	node = env;
	if (check_for_sign(cmd))
	{
		tmp = new[0];
		new[0] = trim_for_join(tmp, "+");
		free(tmp);
		if (!check_syntax_export(new[0]))
		{
			print_err_syntax(new[0]);
			return (1);
		}
		if (new[1] == 0)
			joining(new[0], node, new[1], 0);
		else
			joining(new[0], node, new[1], 1);
		return (1);
	}
	return (0);
}

int	export_if_exist(t_env *env, char *cmd, char **new)
{
	t_env	*node;
	char	*tmp;

	node = env;
	while (node)
	{
		if (!check_syntax_export(new[0]))
		{
			print_err_syntax(new[0]);
			return (1);
		}
		else if (!ft_strcmp(new[0], node->name))
		{
			if (!get_flag(cmd))
				return (1);
			tmp = node->value;
			node->value = ft_strdup(new[1]);
			node->flag = get_env_flag(cmd);
			free(tmp);
			return (1);
		}
		node = node->next;
	}
	return (0);
}

int	export_supp(char *cmd, t_env *env, char **new)
{
	if (join_export(env, cmd, new))
		return (1);
	if (export_if_exist(env, cmd, new))
		return (1);
	return (0);
}

void	build_export(char **cmd, t_env *env, int fd)
{
	int		i;
	char	**new;

	if (count_args(cmd) == 1)
	{
		print_export(env, fd);
		return ;
	}
	i = 1;
	g_global.exit_status = 0;
	while (cmd[i])
	{
		new = get_name_value(cmd[i]);
		if (!export_supp(cmd[i], env, new))
			export_if_n_exist(cmd[i], env, new);
		free_double_char(new);
		i++;
	}
}
