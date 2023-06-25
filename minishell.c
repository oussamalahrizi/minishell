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

void free_tokens(Token **tokens)
{
	int i;

	i = 0;
	while (tokens[i])
	{
		free(tokens[i]->value);
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

int get_last_pipe(char *input) {
    int i;

	i = ft_strlen(input) - 1;

    while (i > 0 && is_space(input[i]))
        i--;
	if (i == 0)
		return (0);
    if (input[i] == '|')
	{
        i--;
		while (i > 0 && is_space(input[i]))
            i--;
        if ((input[i] == '<' || input[i] == '>') || i == 0)
            return 0;
		return (1);
    }
    return 0;
}

int	main(int ac, char **av, char **env)
{
	Token	**tokens;
	Command **commands;
	struct termios term;
	struct termios original;
	char	*input;
	int size = 0;
	t_vars vars;
	((void)ac, (void)av);
	t_env *node;
	node = copy_env(env);
	increase_shell_lvl(node);
	vars.env = node;
	signal_handler();
	char *new_input = NULL;
	while (1)
	{
		tcgetattr(0, &term);
		tcgetattr(0, &original);
		term.c_lflag &= ~(ECHOCTL);
		tcsetattr(0, TCSANOW, &term);
		g_global.readline = 1;
		input = readline("minishell$ ");
		g_global.readline = 0;
		tcsetattr(0, TCSANOW, &original);
		if (!input)
		{
			write(2, "exit\n", 6);
			exit(g_global.exit_status);
		}
		else if (!ft_strcmp("", input))
		{
			free(input);
			continue;
		}
		int go = 0;
		while (get_last_pipe(input))
		{
			new_input = readline("> ");
			if (!new_input)
			{
				(go = 1, error_cmd("syntax error : unexpected end of file\n", 258));
				break;
			}
			(input = ft_strjoin(input, " "), input = ft_strjoin(input, new_input));
			free(new_input);
		}
		add_history(input);
		if (go)
		{
			free(input);
			continue;
		}
		size = word_count(input);
		if (size == -1)
		{
			write(2, "syntax error\n", 14);
			free(input);
			g_global.exit_status = 258;
			continue;
		}
		tokens = (Token **)malloc(sizeof(Token *) * (size + 1));
		if (!tokens)
		{
			perror("allocation failed : ");
			return (1);
		}
		tokenize(input, tokens);
		expander(&tokens, vars.env);
		commands = extract(tokens, vars.env);
		vars.commands = commands;
		exec(&vars);
		free(input);
		free_tokens(tokens);
		free_cmds(commands);
	}
	return (0);
}
