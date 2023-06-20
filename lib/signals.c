#include "../minishell.h"

extern int exit_status;
extern int in_cmd;

void control_c()
{
	printf("\n");
	printf("in cmd : %d\n", in_cmd);
	if (in_cmd)
		return ;
	exit_status = 1;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void signal_handler()
{
	// struct sigaction sa;
    // sa.sa_handler = control_c;
    // sigemptyset(&sa.sa_mask);
    // sa.sa_flags = SA_RESTART;
    // sigaction(SIGINT, &sa, NULL);
    signal(SIGINT, control_c);
    signal(SIGQUIT, SIG_IGN);
}