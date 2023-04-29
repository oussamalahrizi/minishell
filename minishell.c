#include "minishell.h"

int skip_spaces(char *str, int index)
{
	while (str[index] && (str[index] == ' ' || str[index] == '\t' || str[index] == '\r'))
		index++;
	return index;
}

int is_space(char c)
{
	if (c == ' ' || c == '\n' || c == '\t' || c == '\r')
		return (1);
	return (0);
}

void tokenize(char *input, char **tokens)
{
	int i = 0;
	int len = 0;
	char *start;
	int k = 0;

	while (input[i])
	{
		i = skip_spaces(input, i);

		if (!input[i])
			break;
		if (input[i] == '\'')
		{
			start = &input[i];
			len = 1;
			i++;
			while (input[i] && input[i] != '\'')
			{
				len++;
				i++;
			}
			while (input[i + 1] && !is_space(input[i + 1]))
			{
				len++;
				i++;
			}
			tokens[k++] = ft_substr(input, start - input, len + 1);
			i += 2;
		}
		if (input[i] == '\"')
		{
			start = &input[i];
			len = 1;
			i++;
			while (input[i] && input[i] != '\"')
			{
				len++;
				i++;
			}
			tokens[k++] = ft_substr(input, start - input, len + 1);
			i++;
		}
		else
		{
			len = 0;
			start = &input[i];
			while (input[i] && !is_space(input[i]))
			{
				len++;
				i++;
			}
			tokens[k++] = ft_substr(input, start - input, len);
		}
	}
	tokens[k] = NULL;
}

char *get_env(char *string, char **env)
{
	char *variable;
	char *temp;
	if (string[0] == '{' && string[ft_strlen(string) - 1] == '}')
	{
		temp = ft_substr(string, 1, ft_strlen(string) - 2);
	}
	else
		temp = string;
	while (*env)
	{
		if (!ft_strncmp(temp, *env, ft_strlen(temp)))
		{
			variable = ft_strdup(*env + ft_strlen(temp) + 1);
			return variable;
		}
		env++;
	}
	variable = ft_strdup("");
	return variable;
}

void expander(char **tokens, char **env)
{
	int i = 0;
	char *temp;
	while (tokens[i])
	{
		if (tokens[i][0] == '$')
		{
			temp = tokens[i];
			tokens[i] = get_env(&tokens[i][1], env);
			free(temp);
		}
		i++;
	}
}

int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;

	char *input;
	char **tokens = malloc(sizeof(char *) * 1000);
	while (!SIGQUIT)
	{
		input = readline("minishell> ");
		if (!input)
			return (2);
		add_history(input);
		tokenize(input, tokens);
		expander(tokens, env);
		int i = 0;
		printf("{");
		while (tokens[i])
		{
			printf("%s, ", tokens[i]);
			i++;
		}
		printf("NULL}\n");
	}
	system("leaks minishell");
	return (0);
}
