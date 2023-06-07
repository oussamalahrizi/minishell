#include "../minishell.h"

int is_space(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f');
}

int	word_count(char *input)
{
	int	k;
	int	i;
	char quote;
	char *delimiters;

	k = 0;
	i = 0;
	delimiters = ft_strdup("|><");
	while (input[i])
	{
		if(is_space(input[i]))
		{
			i++;
			continue;
		}
		if (input[i] == '\'' || input[i] == '\"')
		{
			quote = input[i];
			i++;
			while (input[i] && input[i] != quote)
			{
				i++;
			}
			if (!input[i])
			{
				free(delimiters);
				return (-1);
			}
			i++;
			while(input[i] && !ft_strchr(delimiters, input[i]))
			{
				if (ft_strchr("'\"", input[i]))
				{
					quote = input[i];
					i++;
					while(input[i] && input[i] != quote)
						i++;
					if (!input[i])
					{
						free(delimiters);
						return (-1);
					}
					i++;
					continue;
				}
				i++;
			}
			k++;
			continue;
		}
		/*
			delimiters section
		*/
		if (ft_strchr(delimiters, input[i])) 
		{
			if ((input[i] == '|' && k == 0))
			{
				free(delimiters);
				return (-1);
			}
			if(input[i] == '|')
			{
				int temp = i + 1;
				while (input[temp] && is_space(input[temp]))
					temp++;
				if (!input[temp] || input[temp] ==  '|')
				{
					free(delimiters);
					return (-1);
				}
			}
			if (input[i] == '>')
			{
				int start = 0;
				int temp = i + 1;
				while (input[temp] && is_space(input[temp]))
				{
					temp++;
					start++;
				}
				if (input[temp] == '|' && start != 0)
				{
					free(delimiters);
					return (-1);
				}
			}
			if(input[i] == '>' || input[i] == '<')
			{
				char c = input[i];
				char l = c == '>' ? '<' : '>';
				int temp = i + 1;
				if(input[temp] == c)
					temp++;
				while (input[temp] && is_space(input[temp]))
					temp++;
				if (!input[temp] || input[temp] == c || input[temp] == l)
				{
					free(delimiters);
					return (-1);
				}
			}
			if (input[i] == '<')
			{
				int temp = i + 1;
				while (input[temp] && is_space(input[temp]))
					temp++;
				if (input[temp] == '|')
				{
					free(delimiters);
					return (-1);
				}
			}
			k++;
			i++;
			continue;
		}
		while(input[i] && !ft_strchr(delimiters, input[i]) && !is_space(input[i]))
		{
			if (ft_strchr("'\"", input[i]))
			{
				quote = input[i];
				i++;
				while(input[i] && input[i] != quote)
					i++;
				if (!input[i])
					return (-1);
				i++;
				continue;
			}
			i++;
		}
		k++;
	}
	free(delimiters);
	return (k);
}

Token* new_token(char type, char* value)
{
    Token* token = (Token*)malloc(sizeof(Token));
    token->type = type;
    token->value = ft_strdup(value);
    return token;
}

void	tokenize(char *input, Token **tokens)
{
	int i;
	char quote;
	char *string;
	char *delimiters;
	int k;

	delimiters = ft_strdup("|><");
	i = 0;
	k = 0;
	while (input[i])
	{
		if(is_space(input[i]))
		{
			i++;
			continue;
		}
		if (input[i] == '\'' || input[i] == '\"')
		{
			quote = input[i];
			string = ft_strdup("");
			append_character(&string, input[i]);
			i++;
			while (input[i] && input[i] != quote)
			{
				append_character(&string, input[i]);
				i++;
			}
			append_character(&string, input[i]);
			i++;
			while(input[i] && !ft_strchr(delimiters, input[i]))
			{
				if (ft_strchr("'\"", input[i]))
				{
					quote = input[i];
					append_character(&string, input[i]);
					i++;
					while(input[i] && input[i] != quote)
					{
						append_character(&string, input[i]);
						i++;
					}
					append_character(&string, input[i]);
					i++;
					continue;
				}
				append_character(&string, input[i]);
				i++;
			}
			// ha wahd
			tokens[k++] = new_token('s', string);
			free(string);
			continue;
		}
		if (ft_strchr(delimiters, input[i]))
		{
			char type;
			string = ft_strdup("");
			if (input[i] == '>' && input[i + 1] == '|')
			{
				append_character(&string, input[i]);
				type = input[i];
				tokens[k++] = new_token(type, string);
				i += 2;
				free(string);
				continue;
			}
			append_character(&string, input[i]);
			type = input[i];
			if (input[i + 1] == input[i] && input[i] == '<')
			{
				append_character(&string, input[i]);
				i++;
				type = 'h';
			}
			else if (input[i + 1] == input[i] && input[i] == '>')
			{
				append_character(&string, input[i]);
				i++;
				type = 'a';
			}
			tokens[k++] = new_token(type, string);
			i++;
			free(string);
			continue;
		}
		string =  ft_strdup("");
		while(input[i] && !ft_strchr(delimiters, input[i]) && !is_space(input[i]))
		{
			if (ft_strchr("'\"", input[i]))
			{
				quote = input[i];
				append_character(&string, input[i]);
				i++;
				while(input[i] && input[i] != quote)
				{
					append_character(&string, input[i]);
					i++;
				}
				append_character(&string, input[i]);
				i++;
				continue;
			}
			append_character(&string, input[i]);
			i++;
		}
		tokens[k++] = new_token('s', string);
		free(string);
	}
	free(delimiters);
	tokens[k] = NULL;
}