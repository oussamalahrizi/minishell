/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 23:26:19 by idelfag           #+#    #+#             */
/*   Updated: 2023/06/23 08:51:32 by olahrizi         ###   ########.fr       */
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
void sortby_ascii(char **strings, int numStrings)
{
    int i;
    int j;
    char* temp;

    i = 0;
    while (i < numStrings - 1)
    {
        j = i + 1;
        while (j < numStrings)
        {
            if (ft_strcmp(strings[i], strings[j]) > 0)
            {
                temp = strings[i];
                strings[i] = strings[j];
                strings[j] = temp;
            }
            j++;
        }
        i++;
    }
}

void print_export(t_env *env)
{
    t_env *node;
    char **array;
    int i;
    int k;

    i = 0;
    node = env;
    while (node)
    {
        i++;
        node = node->next;
    }
    array = malloc(sizeof(char *) * (i + 1));
    node = env;
    i = 0;
    while(node)
    {
        array[i] = malloc(sizeof(char) * (ft_strlen(node->name) + 1));
        ft_strlcpy(array[i], node->name, (ft_strlen(node->name) + 1));
        node = node->next;
        i++;
    }
    array[i] = NULL;
    k = i;
    i = 0;
    sortby_ascii(array, k);
    node = env;
    while(array[i])
    {
        node = env;
        while(ft_strcmp(node->name, array[i]))
            node = node->next;
        if (!ft_strcmp(node->name, array[i]))
        {
            if (ft_strcmp(node->value, "") && node->flag)
                printf("declare -x %s=\"%s\"\n", node->name, node->value);
            else if (node->flag && !ft_strcmp(node->value, ""))
                printf("declare -x %s=\"\"\n", node->name);
            else if (!ft_strcmp(node->value, "") && !node->flag)
                printf("declare -x %s\n", node->name);
            node = env;
            i++;
            continue ;
        }
    }
    free_double_char(array);
}

int check_syntax_export(char *cmd)
{
    int i;

    i = 0;
    if(!ft_isalpha(cmd[i]) && cmd[i] != '_')
        return(0);
    i++;
    while(cmd[i])
    {
        if (ft_isalpha(cmd[i]) || ft_isdigit(cmd[i]) || cmd[i] == '_')
            i++;
        else
            return(0);
    }
    return(1);
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

void print_err_syntax(char *name)
{
    ft_putstr_fd("export: ", 2);
    ft_putchar_fd('`', 2);
    ft_putstr_fd(name, 2);
    ft_putchar_fd('\'', 2);
    ft_putendl_fd(" : not a valid identifier", 2);
    global.exit_status = 1;
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
    global.exit_status = 0;
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
                if (!check_syntax_export(new[0]))
                {
                    print_err_syntax(new[0]);
                    break ;
                }
                if (new[1] == 0)
                    joining(new[0], node, new[1], 0);
                else
                    joining(new[0], node, new[1], 1);
                break ;
            }
            if (!check_syntax_export(new[0]))
            {
                print_err_syntax(new[0]);
                break ;
            }
            else if (!ft_strcmp(new[0], node->name))
            {
                if (!get_flag(cmd[i]))
                    break ;
                tmp = node->value;
                node->value = ft_strdup(new[1]);
                node->flag = get_env_flag(cmd[i]);
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