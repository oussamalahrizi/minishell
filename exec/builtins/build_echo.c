
# include "../../minishell.h"

extern int exit_status;

int check_n_syntax(char **cmd_args, int *flag)
{
    int i;
    int j;

    j = 0;
    i = 1;
    while (cmd_args[i])
    {
        j = 0;
        if (!ft_strcmp(cmd_args[i], "-n"))
            *flag = 0;
        if (ft_strcmp(cmd_args[i], "-n"))
        {
            j++;
            while(cmd_args[i][j] == 'n' && cmd_args[i])
                j++;
            if (!cmd_args[i][j])
                *flag = 0;
            else
                return(i);
        }
        i++;
    }
    return(i);
}

void build_echo(char **cmd_args)
{
    int i;
    char *str;
    int flag;

    flag = 1;
    i = 1;
    // i = check_n_syntax(cmd_args, &flag);
    while(cmd_args[i])
    {
        str = ft_strtrim(cmd_args[i], "'\"");
        ft_putstr_fd(str, 1);
        i++;
        if (cmd_args[i])
            write(1," ",1);
        free(str);
    }
    if (flag)
        write(1,"\n",1);
    exit_status = 0;
}