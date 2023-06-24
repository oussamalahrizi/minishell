/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 22:15:17 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/24 22:19:13 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	flag_case(char **res, char *str, int len)
{
	char	*tmp;

	res[1] = ft_strdup(&str[len + 1]);
	tmp = res[1];
	res[1] = ft_strtrim(tmp, "'\"");
	free(tmp);
	res[2] = NULL;
}

char	**get_name_value(char *str)
{
	char	**res;
	int		len;
	int		flag;

	len = 0;
	flag = 0;
	res = malloc(sizeof(char *) * 3);
	while (str[len])
	{
		if (str[len] == '=' && ft_strcmp(&str[len], ""))
		{
			flag = 1;
			break ;
		}
		len++;
	}
	res[0] = ft_substr(str, 0, len);
	if (flag)
		flag_case(res, str, len);
	else
	{
		res[1] = ft_strdup("");
		res[2] = NULL;
	}
	return (res);
}

static void	convert_helper(t_env *env, char **list)
{
	t_env	*node;
	char	*name;
	char	*value;
	int		i;

	node = env;
	i = 0;
	while (node)
	{
		name = ft_strdup(node->name);
		value = ft_strdup(node->value);
		name = ft_strjoin(name, "=");
		list[i] = ft_strjoin(name, value);
		free(value);
		i++;
		node = node->next;
	}
	list[i] = 0;
}

char	**convert_env(t_env *env)
{
	char	**list;
	t_env	*node;
	int		i;

	node = env;
	i = 0;
	while (node)
	{
		i++;
		node = node->next;
	}
	list = malloc(sizeof(char *) * (i + 1));
	convert_helper(env, list);
	return (list);
}

t_env	*get_env_node(t_env *env, char *name)
{
	t_env	*node;

	node = env;
	while (node)
	{
		if (!ft_strcmp(node->name, name))
			return (node);
		node = node->next;
	}
	return (NULL);
}
