
# include "../../minishell.h"

int check_if_valid(char *cmd)
{
    int i;

    i = 0;
    while (cmd[i] && cmd[i] != '=');
        i++;
    if (!cmd[i])
       return(-1);
    if (cmd[i] == '=' && !cmd[i + 1])
        return(-1);
    return(0);
}

int export(char **cmd ,t_env *env)
{
    int i;
    char **new;
    t_env *node;
    t_env *last;
    char *trimed;
    char *tmp;

    i = 1;
    while(cmd[i])
    {
        trimed = ft_strtrim(cmd[i],"'\"");
        if (check_if_valid(trimed) < 0)
        {
            free(trimed);
            return(-1);
        }
        new = get_name_value(trimed);
        node = env;
        while (node)
        {
            if (!ft_strcmp(new[0], node->name))
            {
                tmp = node->value;
                node->value = new[1];
                free(tmp);
            }
        node = node->next;
        }
        if (!node)
        {
            last = malloc(sizeof(t_env));
            last->name = new[0];
            last->value = new[1];
            last->next = NULL;
            env_add_back(&env, last);
        }
        free(trimed);
        i++;
    }
    return(0);
}