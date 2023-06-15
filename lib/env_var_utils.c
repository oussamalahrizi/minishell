#include "../minishell.h"

int count_env(char **env)
{
	int len = 0;
	while (env[len])
		len++;
	return (len);
}

int get_env_flag(char *cmd)
{
	int i = 0;

	while (cmd[i])
	{
		if (cmd[i] == '=')
			return(1);
		i++;
	}
	return(0);
}

char **get_name_value(char *str)
{
	char **res;
	int len = 0;
	int flag = 0;
	char *tmp;

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
	{
		res[1] = ft_strdup(&str[len + 1]);
		tmp = res[1];
		res[1] = ft_strtrim(tmp, "'\"");
		free(tmp);
		res[2] = NULL;
	}
	else
	{
		res[1] = ft_strdup("");
		res[2] = NULL;
	}
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
		node->flag = get_env_flag(env[i]);
		free_double_char(temp);
		node->next = NULL;
		env_add_back(&new_env, node);
		i++;
	}
	return (new_env);
}

void increase_shell_lvl(t_env *env)
{
	t_env *node;
	char *temp;
	node = env;
	int n;

	while (node)
	{
		if (!ft_strncmp("SHLVL", node->name, 6))
		{
			temp = node->value;
			n = ft_atoi(node->value);
			node->value = ft_itoa(n + 1);
			free(temp);
		}
		node = node->next;
	}
}