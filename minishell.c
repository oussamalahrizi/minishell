/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idelfag <idelfag@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 12:14:59 by idelfag           #+#    #+#             */
/*   Updated: 2023/06/25 12:15:11 by idelfag          ###   ########.fr       */
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
		return (1);
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
		return (1);
	}
	return (0);
}

int	init_main_four(t_mini *mini, t_command **commands)
{
	mini->tokens = (t_token **)malloc(sizeof(t_token *) * (mini->size + 1));
	if (!mini->tokens)
	{
		perror("allocation failed : ");
		return (1);
	}
	tokenize(mini->input, mini->tokens);
	expander(&mini->tokens, mini->vars.env);
	commands = extract(mini->tokens, mini->vars.env);
	mini->vars.commands = commands;
	exec(&mini->vars);
	free(mini->input);
	free_tokens(mini->tokens);
	free_cmds(commands);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	t_mini			mini;
	t_command		**commands;

	((void)ac, (void)av);
	commands = NULL;
	init_main_one(&mini, env);
	while (1)
	{
		if (init_main_two(&mini))
			continue ;
		if (init_main_three(&mini))
			continue ;
		mini.size = word_count(mini.input);
		if (mini.size == -1)
		{
			write(2, "syntax error\n", 14);
			free(mini.input);
			g_global.exit_status = 258;
			continue ;
		}
		if (init_main_four(&mini, commands))
			return (1);
	}
	return (0);
}
