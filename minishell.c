#include "minishell.h"

extern int	exit_status;

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

int	main(int ac, char **av, char **env)
{
	Token	**tokens;
	char	*input;
	int size = 0;

	((void)ac, (void)av, (void)env);
	signal_handler();

	while (1)
	{
		input = readline("minishell> ");
		char *test;
		test = ft_strtrim(input, " ");
		if (!input || !strncmp(test, "exit", ft_strlen("exit")))
		{
			free(test);
			printf("exit\n");
			exit(exit_status);
		}
		free(test);
		// add to history
		add_history(input);
		size = word_count(input);
		if (size == -1)
		{
			write(2, "syntax error\n", 14);
			free(input);
			continue;
		}
		tokens = (Token **)malloc(sizeof(Token *) * (size + 1));
		if (!tokens)
		{
			perror("allocation failed : ");
			return (1);
		}
		tokenize(input, tokens);
		expander(tokens, env);
		int i = 0;
		while (tokens[i] != NULL)
		{
			printf("token with type : %c and value of : %s\n", tokens[i]->type, tokens[i]->value);
			i++;
		}
		// extract(tokens);
		free(input);
		free_tokens(tokens);
	}
	return (0);
}
