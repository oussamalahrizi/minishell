#include "../minishell.h"


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

int word_count(char *input)
{
    int k;
    int i;

    k = 0;
    i = 0;
    while (input[i])
    {
        i = skip_spaces(input, i);
		if (!input[i])
			break;
		if (input[i] == '\'')
		{
			i++;
			while (input[i] && input[i] != '\'')
				i++;
			i++;
			while (input[i] && !is_space(input[i]))
				i++;
			k++;
			i++;
		}
		else if (input[i] == '\"')
		{
			i++;
			while (input[i] && input[i] != '\"')
				i++;
			k++;
			i++;
		}
		else
		{
			while (input[i] && !is_space(input[i]))
				i++;
			k++;
		}
    }
    return k;
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
			i++;
			while (input[i] && !is_space(input[i]))
			{
				len++;
				i++;
			}
			tokens[k++] = ft_substr(input, start - input, len + 1);
			i++;
		}
		else if (input[i] == '\"')
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
				if (input[i] == '\"')
				{
					i++;
					len++;
					while(input[i] && input[i] != '\"')
					{
						i++;
						len++;
					}
					i++;
					len++;
				}
				else
				{
					len++;
					i++;
				}
			}
			tokens[k++] = ft_substr(input, start - input, len);
		}
	}
	tokens[k] = NULL;
}