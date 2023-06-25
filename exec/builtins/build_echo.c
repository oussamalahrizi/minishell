
#include "../../minishell.h"

extern t_global g_global;

int	check_n_syntax(char **cmd_args, int *flag)
{
	int	i;
	int	j;

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
			while (cmd_args[i][j] == 'n' && cmd_args[i])
				j++;
			if (!cmd_args[i][j])
				*flag = 0;
			else
				return (i);
		}
		i++;
	}
	return (i);
}

void	build_echo(char **cmd_args, int fd_out)
{
	int i;
	char *str;
	int flag;

	flag = 1;
	i = 1;
	if (count_args(cmd_args) == 1)
	{
		write(fd_out, "\n", 1);
		return ;
	}
	if (!ft_strncmp(cmd_args[i], "-n", 2))
		i = check_n_syntax(cmd_args, &flag);
	while (cmd_args[i])
	{
		str = ft_strdup(cmd_args[i]);
		ft_putstr_fd(str, fd_out);
		i++;
		if (cmd_args[i])
			write(fd_out, " ", 1);
		free(str);
	}
	if (flag)
		write(fd_out, "\n", 1);
	g_global.exit_status = 0;
}