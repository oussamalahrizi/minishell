#include "../minishell.h"

extern int exit_status;

void control_c()
{
	printf("\n");
	exit_status = 1;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void signal_handler()
{
    signal(SIGINT, control_c);
    signal(SIGQUIT, SIG_IGN);
}