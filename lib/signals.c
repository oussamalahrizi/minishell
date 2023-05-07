#include "../minishell.h"

void control_c()
{
    ioctl(STDIN_FILENO, TIOCSTI, "\n");
    rl_replace_line("", 0);
	rl_on_new_line();
	// rl_replace_line("", 0);
}


void signal_handler()
{
    signal(SIGINT, control_c);
}