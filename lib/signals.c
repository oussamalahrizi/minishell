/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 22:20:14 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 10:51:13 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern t_global	g_global;

static void	control_c(int sig)
{
	if (sig == SIGINT)
	{
		if (g_global.readline == 1)
		{
			g_global.exit_status = 1;
			printf("\n");
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
		}
		else if (g_global.child)
			printf("\n");
	}
}

void	sig_quit(int sig)
{
	(void)sig;
	if (g_global.heredoc)
		return ;
	else if (g_global.child)
		printf("Quit: 3\n");
	rl_redisplay();
}

void	signal_handler(void)
{
	signal(SIGINT, control_c);
	signal(SIGQUIT, sig_quit);
}
