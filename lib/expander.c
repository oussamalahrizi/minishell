#include "../minishell.h"

int		exit_status;

char	*get_env(char *string, char **env)
{
	char	*variable;
	char	*temp;

	if (string[0] == '?')
		return (ft_itoa(exit_status));
	temp = string;
	while (*env)
	{
		if (!ft_strncmp(temp, *env, ft_strlen(temp)))
		{
			variable = ft_strdup(*env + ft_strlen(temp) + 1);
			return (variable);
		}
		env++;
	}
	variable = ft_strdup("");
	return (variable);
}

void	print_split(char **str)
{
	int	i;

	i = 0;
	printf("-------------------\n");
	while (str[i])
	{
		printf("%s\n", str[i]);
		i++;
	}
}

void	free_double(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

int	is_quote(char c)
{
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}

int	handle_dollar(char **new_token, char *string, int index, char **env)
{
	char	*skip;
	char	*temp;

	int i, len;
	i = index;
	len = 0;
	while (string[i] && !is_space(string[i]) && !is_quote(string[i])
		&& (ft_isalnum(string[i]) || string[i] == '?'))
	{
		i++;
		len++;
	}
	skip = ft_substr(string, index, len);
	temp = skip;
	skip = get_env(skip, env);
	free(temp);
	*new_token = ft_strjoin(*new_token, skip);
	free(skip);
	return (i);
}

void	expander(char **tokens, char **env)
{
	int i;
	char *temp;
	int j = 0;
	char *string;
	char *new_token;
	while (tokens[j])
	{
		new_token = ft_strdup("");
		string = tokens[j];
		//loop thru each string and find dollar sign before single or double quotes
		while (*string)
		{
			i = 0;
			while (string[i] && string[i] != '$' && !is_quote(string[i]))
			{
				append_character(&new_token, string[i]);
				i++;
			}
			if (string[i] == '$')
				i = handle_dollar(&new_token, string, i + 1, env);
			else if (string[i] == '\"')
			{
				i += 1;
				while (string[i] && string[i] != '\"')
				{
					if (string[i] == '$')
						i = handle_dollar(&new_token, string, i + 1, env);
					else
					{
						append_character(&new_token, string[i]);
						i++;
					}
				}
				i++;
			}
			else if (string[i] == '\'')
			{
				i++;
				while (string[i] && string[i] != '\'')
				{
					append_character(&new_token, string[i]);
					i++;
				}
				i++;
			}
			string += i;
		}
		temp = tokens[j];
		tokens[j] = new_token;
		free(temp);
		j++;
	}
}