
# include "../../minishell.h"

void build_echo(char **cmd_args)
{
    int i;
    int j;
    int flag;

    i = 1;
    j = 0;
    flag = 1;
    if (ft_strncmp(cmd_args[i], "-n", 3) == 0)
        flag = 0;
    i++;
    while(cmd_args[i])
    {
        ft_putstr_fd(cmd_args[i], 1);
        i++;
        if (cmd_args[i])
            write(1," ",1);
    }
    if (flag)
        write(1,"\n",1);
}

