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

	if (string[0] == '?')
		return (ft_itoa(exit_status));
	if (!ft_strlen(string))
		return(ft_strdup(""));
	temp = string;
	node = env;
	while (node)
	{
		if (!ft_strncmp(temp, node->name, ft_strlen(temp)))
		{
			variable = ft_strdup(node->value);
			return (variable);
		}
		node = node->next;
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

char *get_ifs(t_env *env)
{
	char	*variable;
	char	*temp;
	t_env *node;

	temp = "IFS";
	node = env;
	while (node)
	{
		if (!ft_strncmp(temp, node->name, ft_strlen(temp)))
		{
			variable = ft_strdup(node->value);
			return (variable);
		}
		node = node->next;
	}
	variable = NULL;
	return (variable);
}

int check_ifs(char *string, int index, t_env *env)
{
	char	*skip;
	char	*temp;
	int count;
	char **splited;
	char *ifs;

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
	count = 0;
	ifs = get_ifs(env);
	if (!ifs)
		splited = split_by_str(skip, " \t\n");
	else
		splited = split_by_str(skip, ifs);
	while (splited[count])
		count++;
	free(temp);
	free(skip);
	if (ifs)
		free(ifs);
	i = 0;
	while (splited[i])
	{
		free(splited[i]);
		i++;
	}
	free(splited);
	if (count > 1)
		return (-1);
	return (0);
}

int	handle_dollar_alone(char *string, int index, int token_index, t_exp *exp, int *new_index)
{
	char	*skip;
	char	*temp;
	char **split;
	Token **new_tokens = NULL;
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
	skip = get_env(skip, exp->env);
	int token_size = 0;
	while (exp->tokens[token_size])
		token_size++;
	int j = 0;
	split = ft_split(skip, ' ');
	while (split[j])
		j++;
	new_tokens = malloc(sizeof(Token *) * (token_size + j));
	int size = token_size + j - 1;
	new_tokens[size] = 0;
	j = 0;
	while (j < token_index + *new_index)
	{
		new_tokens[j] = new_token(exp->tokens[j]->type, exp->tokens[j]->value);
		j++;
	}
	int k = 0;
	while (split[k])
	{
		new_tokens[j] = new_token('s', split[k]);
		k++;
		j++;
	}
	token_index += *new_index + 1;
	if (token_index < token_size)
	{
		while (j < size)
		{
			new_tokens[j] = new_token(exp->tokens[token_index]->type, exp->tokens[token_index]->value);
			j++;
			token_index++;
		}
	}
	*new_index += k - 1;
	free(temp);
	free_double_char(split);
	free(skip);
	free_double(exp->tokens);
	exp->tokens = duplicate_tokens(new_tokens);
	free_double(new_tokens);
	return (i);
}


int	handle_dollar(char **new_token, char *string, int index, int token_index,int *new_index, t_exp *exp)
{
	char	*skip;
	char	*temp;
	char *old;

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
	skip = get_env(skip, exp->env);
	free(temp);
	*new_token = ft_strjoin(*new_token, skip);
	old = exp->tokens[token_index + *new_index]->value;
	exp->tokens[token_index + *new_index]->value = ft_strdup(*new_token);
	free(old);
	free(skip);
	return (i);
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
		while (*string)
		{
			new_token = ft_strdup("");
			i = 0;
			while (string[i] && string[i] != '$' && !is_quote(string[i]))
			{
				append_character(&new_token, string[i]);
				i++;
			}
			if (string[i] == '$')
			{
				if (string[i + 1] == 0)
				{
					append_character(&new_token, '$');
					break;
				}
				if (j - 1 >= 0 && (tokens[j - 1]->type == '>' || tokens[j - 1]->type == '<' 
					|| tokens[j - 1]->type == 'a') && check_ifs(string, i + 1, env) == -1)
				{
					free(new_token);
					return (-1);
				}
				i = handle_dollar_alone(string , i + 1, j, &vars, &new_index);
			}
			else if (string[i] == '\"')
			{
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
							i = handle_dollar(&new_token, string, i + 1, j,&new_index, &vars);
					}
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
			free(new_token);
		}
		j++;
	}
	free_double(*tokens_i);
	*tokens_i = duplicate_tokens(vars.tokens);
	free_double(vars.tokens);
	return (0);
}