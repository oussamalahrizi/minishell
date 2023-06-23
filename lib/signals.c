#include "../minishell.h"

extern int readl;

static void control_c(int sig)
{
	if (sig == SIGINT)
	{
		if (global.readline == 1)
		{
			global.exit_status = 1;
			printf("\n");
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
		}
		else if (global.child)
			printf("\n");
	}
}

void sig_quit()
{
	if (global.heredoc)
		return;
	else if (global.child)
		printf("Quit: 3\n");
	rl_redisplay();
}

void signal_handler()
{
    signal(SIGINT, control_c);
    signal(SIGQUIT, sig_quit);
}