#include "../minishell.h"

char *clean_command(char *string, t_env *env);

char **allocate_strings(Token **tokens, int *index, char *cmd, t_env *env)
{
	int i;
	char **res;
	int c = 0;

	i = *index;
	while (tokens[i] && tokens[i]->type == 's')
	{
		if (ft_strlen(tokens[i]->value) == 0)
		{
			i++;
			continue;
		}
		i++;
		c++;
	}
	res = malloc(sizeof(char *) * (c + 2));
	c = 0;
	i = *index;
	if (cmd && i - 2 >= 0 && tokens[i - 2]->type != 's')
	{
		res[c] = ft_strdup(cmd);
		c++;
	}
	while(tokens[i] && tokens[i]->type == 's')
	{
		if (ft_strlen(tokens[i]->value) == 0)
		{
			i++;
			continue;
		}
		res[c] = clean_command(tokens[i]->value, env);
		c++;
		i++;
	}
	*index = i;
	res[c] = 0;
	return (res);
}

void set_last_infile(files *file_list)
{
	files *node;
	files *res;
	res = NULL;
	node = file_list;
	while (node)
	{
		if (node->type == '<')
		{
			if (res)
				res->open = -1;
			res = node;
		}
		node = node->next;
	}
}

void set_last_outfile(files *file_list)
{
	files *node;
	files *res;
	res = NULL;
	node = file_list;
	while (node)
	{
		if (node->type == 'a' || node->type == '>')
		{
			if (res)
				res->open = -1;
			res = node;
		}
		node = node->next;
	}
}


files *allocate_files(Token **tokens, int *index, files *file_list, t_env *env)
{
	// guaranteed this token type is an 's'
	int i;
	files *current_files;
	files *start;
	files *new;
	char *temp;

	i = *index;
	current_files = file_list;
	new = malloc(sizeof(files));
	new->next = NULL;
	new->filename = NULL;
	new->h_content = NULL;
	new->del = NULL;
	new->open = 1;
	new->expand = 1;
	if (tokens[i]->type != 's')
	{
		write(2, "token after redirection is not a string\n", 41);		
		free(new);
		return (NULL);
	}
	if(tokens[i - 1]->type == 'h')
	{
		temp = tokens[i]->value;
		if (temp[0] == temp[ft_strlen(temp) - 1] && ft_strchr("\"'", temp[0]))
			new->expand = 0;
		tokens[i]->value = clean_command(temp, env);
		free(temp);
		new->del = ft_strdup(tokens[i]->value);
		new->type = 'h';
		new->fd = -1;
		i++;
	}
	else if (tokens[i - 1]->type == '>' || tokens[i - 1]->type == '<' || tokens[i - 1]->type == 'a')
	{
		temp = tokens[i]->value;
		tokens[i]->value = clean_command(temp, env);
		free(temp);
		new->filename = ft_strdup(tokens[i]->value);
		new->type = tokens[i - 1]->type;
		new->fd = -1;
		i++;
	}
	if (!current_files)
	{
		current_files = new;
		file_list = current_files;
	}
	else
	{
		start = current_files;
		while (current_files->next)
			current_files = current_files->next;
		current_files->next = new;
		file_list = start;
	}
	*index = i;
	set_last_infile(file_list);
	set_last_outfile(file_list);
	return (file_list);
}

char *get_userhome(t_env *env)
{
	char *str;
	char *temp;

	temp = get_env("HOME", env);
	str = ft_strdup("/Users/");
	if (!*temp)
	{
		free(temp);
		temp = get_env("USER", env);
		if (!*temp)
		{
			free(temp);
			temp = get_env("LOGNAME", env);
			if (!*temp)
			{
				free(str);
				return (temp);
			}
		}
		str = ft_strjoin(str, temp);
	}
	else
	{
		free(str);
		return (temp);
	}
	free(temp);
	return (str);
}

char *expand_tilde(char *string, t_env *env)
{
	char *str = NULL;
	if (string[0] == '~' && (string[1] == '/' || !string[1]))
	{
		str = get_userhome(env);
		if (!*str)
		{
			error_cmd("cd: HOME/USER/LOGNAME not set\n", 1);
			free(str);
			return (ft_strdup(""));
		}
		str = ft_strjoin(str, &string[1]);
	}
	else if (string[0] == '~' && string[1] != '/')
	{
		str = ft_strdup("/Users/");
		int i = 1;
		while (string[i] && string[i] != '/')
			i++;
		char *temp = ft_substr(&string[1], 0, i);
		str = ft_strjoin(str, temp);
		int j = 0;
		if (string[i])
		{
			i++;
			int save = i;
			while (string[i])
			{
				j++;
				i++;
			}
			free(temp);
			temp = ft_substr(&string[save], 0, j);
			str = ft_strjoin(str, temp);
		}
		free(temp);
	}
	else
	{
		str = ft_strdup("");
		str = ft_strjoin(str, string);
	}
	free(string);
	return (str);
}

char *clean_command(char *string, t_env *env)
{
	int i = 0;
	char *res = ft_strdup("");
	char quote;

	char *temp = ft_strdup(string);
	temp = expand_tilde(temp, env);

	while (temp[i])
	{
		if (ft_strchr("'\"", temp[i]))
		{
			quote = temp[i];
			i++;
			while (temp[i] && temp[i] != quote)
			{
				append_character(&res, temp[i]);
				i++;
			}
			i++;
		}
		else
		{
			append_character(&res, temp[i]);
			i++;
		}
	}
	free(temp);
	return (res);
}

Command **extract(Token **tokens, t_env *env)
{
	int i = 0;
	Command **commands;
	int num = 1;
	int k = 0;

	while(tokens[i])
	{
		if (tokens[i]->type == '|')
			num++;
		i++;
	}
	commands = malloc(sizeof(Command*) * (num + 1));
	i = 0;
	commands[num] = NULL;
	while (i < num)
	{
		commands[i] = malloc(sizeof(Command));
		commands[i]->cmd = NULL;
		commands[i]->cmd_args = NULL;
		commands[i]->files = NULL;
		i++;
	}
	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->type == 's')
		{
			if (commands[k]->cmd == NULL)
			{
				char *temp = clean_command(tokens[i]->value, env);
				commands[k]->cmd = ft_strdup(temp);
				free(temp);
			}
			else
				commands[k]->cmd_args = allocate_strings(tokens, &i ,commands[k]->cmd, env);
		}
		else{
			if (tokens[i]->type == '|' || !tokens[i])
			{
				k++;
				if (tokens[i] && tokens[i]->type == '|')
					i++;
				continue;
			}
			else if (tokens[i]->type == '>' || tokens[i]->type == '<' || tokens[i]->type == 'h' || tokens[i]->type == 'a')
			{
				i++;
				if (!tokens[i])
				{
					error("syntax error\n");
					break;
				}
				if (tokens[i]->type == 's')
					commands[k]->files = allocate_files(tokens, &i, commands[k]->files, env);
			}
		}
	}
	commands[num] = 0;
#if 0
	int j = 0;
	while(commands[j])
	{
		printf("command : %s$\n", commands[j]->cmd);
		int h = 0;
		if (commands[j]->cmd_args)
		{
			while(commands[j]->cmd_args[h])
			{
				printf("command args: %s\n", commands[j]->cmd_args[h]);
				h++;
			}
		}
		files *node = commands[j]->files;
		printf("---------------------\n");
		printf("files : \n");
		h = 0;
		while (node)
		{
			printf("file number : %d\n", h + 1);
			printf("type : %c\n", node->type);
			printf("del if exists : %s\n", node->del);
			printf("filename if exists : %s\n", node->filename);
			printf("open ? : %d\n", node->open);
			node = node->next;
			h++;
		}
		printf("going next command\n");
		j++;
	}
#endif
	return commands;
}