#include "minishell.h"

int main(int ac, char **av, char **env)
{
	((void)ac,(void)av);
	char **tokens;
	char *input;

	signal_handler();
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			return (2);
		add_history(input);
		tokens = (char **) malloc(sizeof(char *) * (word_count(input) + 1));
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
			printf("%s\n", tokens[i]);
			i++;
		}
		free(input);
		free_double(tokens);
	}
	return (0);
}
