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

void free_double_char(char **str)
{
	int i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void free_cmds(Command **commands)
{
	int i = 0;
	files *node;
	files *temp;
	while(commands[i])
	{
		if (commands[i]->cmd)
			free(commands[i]->cmd);
		if (commands[i]->cmd_args)
			free_double_char(commands[i]->cmd_args);
		if (commands[i]->files)
		{
			node = commands[i]->files;
			while (node)
			{
				temp = node;
				if (node->del)
					free(node->del);
				if (node->filename)
					free(node->filename);
				node = node->next;
				free(temp);
			}
		}
		free(commands[i]);
		i++;
	}
	free(commands);
}

int	main(int ac, char **av, char **env)
{
	Token	**tokens;
	Command **commands;
	struct termios term;
	struct termios original;
	char	*input;
	int size = 0;

	((void)ac, (void)av, (void)env);
	signal_handler();


	printf("getting env var into linked list\n");
	t_env *node;
	t_env *tmp;
	node = copy_env(env);
	increase_shell_lvl(node);
	tmp = node;
	while (tmp)
	{
		printf("%s=%s\n", tmp->name, tmp->value);
		tmp = tmp->next;
	}
	while (1)
	{
		tcgetattr(0, &term);
		tcgetattr(0, &original);
		term.c_lflag &= ~(ECHOCTL);
		tcsetattr(0, TCSANOW, &term);
		input = readline("minishell> ");
		tcsetattr(0, TCSANOW, &original);
		char *test;
		test = ft_strtrim(input, " ");
		if (!input || !ft_strncmp(test, "exit", ft_strlen("exit")))
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
		if (expander(tokens, env) == -1)
		{
			write(2, "abiguous redirection\n", 22);
			free(input);
			free_tokens(tokens);
			continue;
		}
  #if 1
      
		  int i = 0;
		  while (tokens[i] != NULL)
		  {
			  printf("token with type : %c and value of :%s|EOF|\n", tokens[i]->type, tokens[i]->value);
			  i++;
		  }
    #endif
		commands = extract(tokens); // look extract file to print cmds
		free(input);
		free_tokens(tokens);
		free_cmds(commands);
	}
	return (0);
}
