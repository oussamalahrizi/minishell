/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 12:14:59 by idelfag           #+#    #+#             */
/*   Updated: 2023/06/25 22:29:24 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_global	g_global;

void	init_main_one(t_mini *mini, char **env)
{
	t_env	*node;

	mini->size = 0;
	node = copy_env(env);
	increase_shell_lvl(node);
	mini->vars.env = node;
	signal_handler();
	mini->new_input = NULL;
}

int	init_main_two(t_mini *mini)
{
	tcgetattr(0, &mini->term);
	tcgetattr(0, &mini->original);
	mini->term.c_lflag &= ~(ECHOCTL);
	tcsetattr(0, TCSANOW, &mini->term);
	g_global.readline = 1;
	mini->input = readline("minishell$ ");
	g_global.readline = 0;
	tcsetattr(0, TCSANOW, &mini->original);
	if (!mini->input)
	{
		write(2, "exit\n", 6);
		exit(g_global.exit_status);
	}
	if (!ft_strcmp("", mini->input))
	{
		free(mini->input);
		return (-1);
	}
	return (0);
}

int	init_main_three(t_mini *mini)
{
	int	go;

	go = 0;
	while (get_last_pipe(mini->input))
	{
		mini->new_input = readline("> ");
		if (!mini->new_input)
		{
			error_cmd("syntax error : unexpected end of file\n", 258);
			go = 1;
			break ;
		}
		mini->input = ft_strjoin(mini->input, " ");
		mini->input = ft_strjoin(mini->input, mini->new_input);
		free(mini->new_input);
	}
	add_history(mini->input);
	if (go)
	{
		free(mini->input);
		return (-1);
	}
	return (0);
}

int	init_main_four(t_mini *mini)
{
	if (mini->size == -1)
	{
		write(2, "syntax error\n", 14);
		free(mini->input);
		g_global.exit_status = 258;
		return (-1);
	}
	mini->tokens = (t_token **)malloc(sizeof(t_token *) * (mini->size + 1));
	tokenize(mini->input, mini->tokens);
	expander(&mini->tokens, mini->vars.env);
	mini->vars.commands = extract(mini->tokens, mini->vars.env);
	exec(&mini->vars);
	free(mini->input);
	free_tokens(mini->tokens);
	free_cmds(mini->vars.commands);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	t_mini			mini;

	((void)ac, (void)av);
	init_main_one(&mini, env);
	while (1)
	{
		if (init_main_two(&mini) == -1)
			continue ;
		if (init_main_three(&mini) == -1)
			continue ;
		mini.size = word_count(mini.input);
		if (init_main_four(&mini) == -1)
			continue ;
	}
	return (0);
}
