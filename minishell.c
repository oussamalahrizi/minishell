/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahrizi <olahrizi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 12:14:59 by idelfag           #+#    #+#             */
/*   Updated: 2023/06/25 18:04:24 by olahrizi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_global	g_global;

void	init_main_one(t_mini *mini, t_env *node, char **env)
{
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

int	init_main_three(t_mini *mini, int go)
{
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

int	init_main_four(t_mini *mini, t_command **commands)
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
	commands = extract(mini->tokens, mini->vars.env);
	mini->vars.commands = commands;
	exec(&mini->vars);
	free(mini->input);
	free_tokens(mini->tokens);
	free_cmds(mini->vars.commands);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	t_mini			mini;
	t_env			*node;
	t_command		**commands;
	int				go;

	((void)ac, (void)av);
	node = NULL;
	commands = NULL;
	init_main_one(&mini, node, env);
	while (1)
	{
		if (init_main_two(&mini) == -1)
			continue ;
		go = 0;
		if (init_main_three(&mini, go) == -1)
			continue ;
		mini.size = word_count(mini.input);
		if (init_main_four(&mini, commands) == -1)
			continue ;
	}
	return (0);
}
