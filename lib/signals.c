/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 22:20:14 by olahrizi          #+#    #+#             */
/*   Updated: 2023/06/25 10:37:04 by idelfag          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	(void) sig;
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
