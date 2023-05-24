#include "../minishell.h"

extern int exit_status;

void control_c()
{
	exit_status = 130;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_replace_line("", 0);
	rl_on_new_line();
}

void control_d()
{
	ioctl(STDIN_FILENO, TIOCSTI, "exit\n");
	rl_replace_line("", 0);
	exit(130);
}

void signal_handler()
{
    signal(SIGINT, control_c);
    signal(SIGQUIT, SIG_IGN);
}