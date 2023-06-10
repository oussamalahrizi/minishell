/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 23:26:19 by idelfag           #+#    #+#             */
/*   Updated: 2023/06/10 01:28:21 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../minishell.h"

int check_for_sign(char *cmd)
{
    int i;
    
    i = 0;
    while(cmd[i])
    {
        if (cmd[i] == '+')
        {
            if (cmd[i + 1] == '=')
                return(1);
        }
        i++;
    }
    return(0);
}

int get_flag(char *cmd)
{
    int i;
    int flag;

    i = 0;
    flag = 0;
    while(cmd[i])
    {
        if (cmd[i] == '=')
            flag = 1;
        i++;
    }
    return(flag);
}

void print_export(t_env *env)
{
    t_env *node;

    node = env;
    while(node)
    {
        if (ft_strcmp(node->value, "") && node->flag)
            printf("declare x : %s=\"%s\"\n", node->name, node->value);
        else if (node->flag && !ft_strcmp(node->value, ""))
            printf("declare x : %s=\"\"\n", node->name);
        else if (!ft_strcmp(node->value, "") && !node->flag)
            printf("declare x : %s\n", node->name);
        node = node->next;
    }
}

char **get_vars_join(char *str)
{
	char **res;
	int len = 0;
	int flag = 0;
    char *tmp;

	res = malloc(sizeof(char *) * 3);
	while (str[len])
	{
		if (str[len] == '+' && str[len + 1] == '=')
		{
			flag = 1;
			break ;
		}
		len++;
	}
	res[0] = ft_substr(str, 0, len);
	if (flag)
	{
		res[1] = ft_strdup(&str[len + 2]);
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


void joining(char *name, t_env *env, char *value, int flag)
{
    t_env *node;
    t_env *last;

    node = env;
    while(node)
    {
        if(!ft_strcmp(name, node->name))
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

void build_export(char **cmd ,t_env *env)
{
    int i;
    char **new;
    t_env *node;
    t_env *last;
    char *tmp;

    if (count_args(cmd) == 1)
    {
        print_export(env);
        return ;
    }
    i = 1;
    while(cmd[i])
    {
        new = get_name_value(cmd[i]);
        node = env;
        while (node)
        {
            if (check_for_sign(cmd[i]))
            {
                tmp = new[0];
                new[0] = ft_strtrim(tmp, "+");
                free(tmp);
                if (new[1] == 0)
                    joining(new[0], node, new[1], 0);
                else
                    joining(new[0], node, new[1], 1);
                break ;
            }
            else if (!ft_strcmp(new[0], node->name))
            {
                tmp = node->value;
                node->value = ft_strdup(new[1]);
                free(tmp);
                break ;
            }
            node = node->next;
        }
        if (!node)
        {
            last = malloc(sizeof(t_env));
            last->name = ft_strdup(new[0]);
            last->value = ft_strdup(new[1]);
            last->flag = get_env_flag(cmd[i]);
            last->next = NULL;
            env_add_back(&env, last);
        }
        free_double_char(new);
        i++;
    }
}

// int main(int ac, char **av, char **env)
// {
//     char **cmd = malloc(sizeof(char *) * ac);
//     t_env *ev;
//     int i = 1;
//     int j = 0;
    
//     ev = copy_env(env);
//     while(i <= ac)
//     {
//         cmd[j] = av[i];
//         i++;
//         j++;
//     }
//     build_export(cmd, ev);
//     build_env(ev, 1);
//     printf("\n\n");
//     print_export(ev);
// }