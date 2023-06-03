#include "../minishell.h"

int count_env(char **env)
{
	int len = 0;
	while (env[len])
		len++;
	return (len);
}

char **get_name_value(char *str)
{
	char **res;
	int len = 0;
	res = malloc(sizeof(char *) * 3);
	while (str[len] && str[len] != '=')
		len++;
	res[0] = ft_substr(str, 0, len);
	str += len + 1;
	len = 0;
	while (str[len])
		len++;
	res[1] = ft_substr(str, 0, len);
	res[2] = 0;
	return (res);
}

void	env_add_back(t_env **node, t_env *data)
{
	t_env *temp;
	if(*node)
	{
		temp = *node;
		while (temp->next)
			temp = temp->next;
		temp->next = data;
	}
	else
		*node = data;
}

t_env *copy_env(char **env)
{
	t_env *new_env;
	t_env *node;
	char **temp;
	int i = 0;

	new_env = NULL;
	while (env[i])
	{
		node = malloc(sizeof(t_env));
		temp = get_name_value(env[i]);
		node->name = ft_strdup(temp[0]);
		node->value = ft_strdup(temp[1]);
		free_double_char(temp);
		node->next = NULL;
		env_add_back(&new_env, node);
		free(node);
		i++;
	}
	return (new_env);
}