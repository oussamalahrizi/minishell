#include "../minishell.h"

extern int		exit_status;

typedef struct {
	t_env *env;
	Token **tokens;
}	t_exp;

char	*get_env(char *string, t_env *env)
{
	char	*variable;
	char	*temp;
	t_env *node;

	if (*string == '?')
		return (ft_itoa(global.exit_status));
	if (*string == '$')
		return (ft_strdup("$"));
	if (!ft_strlen(string))
		return(ft_strdup(""));
	temp = string;
	node = env;
	while (node)
	{
		if (!ft_strcmp(temp, node->name))
		{
			variable = ft_strdup(node->value);
			return (variable);
		}
		node = node->next;
	}
	variable = ft_strdup("");
	return (variable);
}

void	free_double(Token **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]->value);
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

void handle_dollar_alone(char *string, int *index, int token_index, t_exp *exp, int *new_index, char **new_token_value)
{
	char	*skip = NULL;
	char **split;
	Token **new_tokens = NULL;
	int i, len;
	i = *index + 1;
	int start_index = i;
	len = 0;
	if (string[i] == '$' || !string[i])
	{
		skip = get_env("$", exp->env);
		if (string[i] == '$')
			*index = i + 1;
		else
			*index = i;
	}
	else
	{
		while (string[i] && !is_space(string[i]) && !is_quote(string[i]) && string[i] != '$'
			&& (ft_isalnum(string[i]) || string[i] == '?' || string[i] == '_'))
		{
			len++;
			if (i == *index + 1 && ft_isdigit(string[i]))
			{
				i++;
				break;
			}
			if (string[i] == '?')
			{
				i++;
				break;
			}
			i++;
		}
		skip = ft_substr(string, start_index, len);
		*index = i;
		char *temp;
		temp = skip;
		skip = get_env(skip, exp->env);
		free(temp);
	}
	int token_size = 0;
	while (exp->tokens[token_size])
		token_size++;
	int j = 0;
	split = ft_split(skip, ' ');
	while (split[j])
		j++;
	int split_size = j;
	if (!*skip)
		j = 1;
	new_tokens = malloc(sizeof(Token *) * (token_size + j));
	int size = token_size + j - 1;
	j = 0;
	while (j < token_index + *new_index)
	{
		new_tokens[j] = new_token(exp->tokens[j]->type, exp->tokens[j]->value);
		j++;
	}
	int k = 0;
	if (!*skip)
	{
		*new_token_value = ft_strjoin(*new_token_value, skip);
		new_tokens[j] = new_token('s', *new_token_value);
		j++;
	}
	else
	{
		if (split_size == 1)
		{
			k = 0;
			*new_token_value = ft_strjoin(*new_token_value, split[k]);
			new_tokens[j] = new_token('s', *new_token_value);
			k++;
			j++;
		}
		else if (split_size > 1)
		{
			k = 0;
			*new_token_value = ft_strjoin(*new_token_value, split[k]);
			new_tokens[j] = new_token('s', *new_token_value);
			j++;
			k = 1;
			while (k < split_size)
			{
				new_tokens[j] = new_token('s', split[k]);
				k++;
				j++;
			}
			free(*new_token_value);
			*new_token_value = ft_strdup(split[k - 1]);	
		}
	}
	token_index += *new_index + 1;
	while (j < size)
	{
		new_tokens[j] = new_token(exp->tokens[token_index]->type, exp->tokens[token_index]->value);
		j++;
		token_index++;
	}
	new_tokens[j] = 0;
	if (*skip)
		*new_index += k - 1;
	j = 0;
	free_double_char(split);
	free(skip);
	free_double(exp->tokens);
	exp->tokens = duplicate_tokens(new_tokens);
	free_double(new_tokens);
}


void	handle_dollar(char **new_token, char *string, int *index, int token_index,int *new_index, t_exp *exp)
{
	char	*skip = NULL;
	char *old;
	int i, len;
	i = *index + 1;
	int start_index = i;
	len = 0;
	if (string[i] == '$')
	{
		skip = get_env("$", exp->env);
		if (string[i] == '$')
			*index = i + 1;
		else
			*index = i;
	}
	else
	{
		while (string[i] && !is_space(string[i]) && !is_quote(string[i])
			&& string[i] != '$' && (ft_isalnum(string[i]) || string[i] == '?' || string[i] == '_'))
		{
			len++;
			if (i == *index + 1 && ft_isdigit(string[i]))
			{
				i++;
				break;
			}
			if (string[i] == '?')
			{
				i++;
				break;
			}
			i++;
		}
		skip = ft_substr(string, start_index, len);
		*index = i;
		char *temp;
		temp = skip;
		skip = get_env(skip, exp->env);
		free(temp);
	}
	*new_token = ft_strjoin(*new_token, skip);
	free(skip);
	old = exp->tokens[token_index + *new_index]->value;
	exp->tokens[token_index + *new_index]->value = ft_strdup(*new_token);
	free(old);
}

int	expander(Token ***tokens_i, t_env *env)
{
	int i;
	int j = 0;
	char *string;
	char *new_token;
	t_exp vars;
	vars.env = env;
	Token **tokens = *tokens_i;
	vars.tokens = duplicate_tokens(tokens);
	int new_index = 0;
	while (tokens[j])
	{
		if (tokens[j]->type != 's')
		{
			j++;
			continue;
		}
		else if (j - 1 >= 0 && tokens[j - 1]->type == 'h' && tokens[j]->type == 's')
		{
			j++;
			continue;
		}
		string = tokens[j]->value;
		new_token = ft_strdup("");
		while (*string)
		{
			i = 0;
			while (string[i] && string[i] != '$' && !is_quote(string[i]))
			{
				append_character(&new_token, string[i]);
				i++;
			}
			if (string[i] == '$')
			{
				if (j - 1 >= 0 && (tokens[j - 1]->type == '>' || tokens[j - 1]->type == '<' 
					|| tokens[j - 1]->type == 'a'))
						break;
				handle_dollar_alone(string , &i , j, &vars, &new_index, &new_token);
			}
			else if (string[i] == '\"')
			{
				append_character(&new_token, string[i]);
				i += 1;
				while (string[i] && string[i] != '\"')
				{
					if (string[i] == '$')
					{
						if (string[i + 1] == '"' || is_space(string[i + 1]))
						{
							append_character(&new_token, '$');
							i++;
						}
						else
						{
							handle_dollar(&new_token, string, &i, j,&new_index, &vars);
						}
					}
					else
					{
						append_character(&new_token, string[i]);
						i++;
					}
				}
				append_character(&new_token, string[i]);
				i++;
			}
			else if (string[i] == '\'')
			{
				append_character(&new_token, string[i]);
				i++;
				while (string[i] && string[i] != '\'')
				{	
					append_character(&new_token, string[i]);
					i++;
				}
				append_character(&new_token, string[i]);
				i++;
			}
			if (!string[i] && ft_strcmp(vars.tokens[j + new_index]->value, new_token))
			{
				char *temp = vars.tokens[j + new_index]->value;
				vars.tokens[j + new_index]->value = ft_strdup(new_token);
				free(temp);
				break;
			}
			string += i;
		}
		free(new_token);
		j++;
	}
	free_double(*tokens_i);
	*tokens_i = duplicate_tokens(vars.tokens);
	free_double(vars.tokens);
	return (0);
}