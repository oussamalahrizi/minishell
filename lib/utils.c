#include "../minishell.h"

extern int exit_status;

int error(char *str)
{
	write(2, str, ft_strlen(str));
	exit_status = 1;
	return (1);
}