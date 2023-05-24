#include "minishell.h"

extern int	exit_status;

int	main(int ac, char **av, char **env)
{
	char	**tokens;
	char	*input;
	int		i;
	int size = 0;

	((void)ac, (void)av);
	signal_handler();

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			exit(exit_status);
		}
		// add to history
		add_history(input);
		size = word_count(input);
		if (size == -1)
		{
			write(2, "syntax error\n", 14);
			free(input);
			continue;
		}
		tokens = (char **)malloc(sizeof(char *) * (size + 1));
		if (!tokens)
		{
			perror("allocation failed : ");
			return (1);
		}
		tokenize(input, tokens);
		expander(tokens, env);
		i = 0;
		while (tokens[i] != NULL)
		{
			printf("%s\n", tokens[i]);
			i++;
		}
		free(input);
		free_double(tokens);
	}
	return (0);
}
