#include "../minishell.h"

char *clean_command(char *string);

char **allocate_strings(Token **tokens, int *index)
{
	int i;
	char **res;
	int c = 0;

	i = *index;
	while (tokens[i] && tokens[i]->type == 's')
	{
		i++;
		c++;
	}
	res = malloc(sizeof(char *) * (c + 1));
	i = *index;
	c = 0;
	while(tokens[i] && tokens[i]->type == 's')
	{
		res[c] = clean_command(tokens[i]->value);
		c++;
		i++;
	}
	*index = i;
	res[c] = 0;
	return (res);
}

files *allocate_files(Token **tokens, int *index, files *file_list)
{
	// guaranteed this token type is an 's'
	int i;
	files *current_files;
	files *start;
	files *new;

	i = *index;
	current_files = file_list;
	new = malloc(sizeof(files));
	new->next = NULL;
	new->filename = NULL;
	new->del = NULL;
	if (tokens[i]->type != 's')
	{
		write(2, "token after redirection is not a string\n", 41);		
		free(new);
		return (NULL);
	}
	if(tokens[i - 1]->type == 'h')
	{
		new->del = ft_strdup(tokens[i]->value);
		new->type = 'h';
		new->fd = -1;
		i++;
	}
	else if (tokens[i - 1]->type == '>' || tokens[i - 1]->type == '<' || tokens[i - 1]->type == 'a')
	{
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
	return (file_list);
}

char *clean_command(char *string)
{
	int i = 0;
	char *res = ft_strdup("");
	char quote;
	while (string[i])
	{
		if (ft_strchr("'\"", string[i]))
		{
			quote = string[i];
			i++;
			while (string[i] && string[i] != quote)
			{
				append_character(&res, string[i]);
				i++;
			}
			i++;
		}
		else
		{
			append_character(&res, string[i]);
			i++;
		}
	}
	return (res);
}

Command **extract(Token **tokens)
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
				char *temp = clean_command(tokens[i]->value);
				commands[k]->cmd = ft_strdup(tokens[i]->value);
				free(temp);
			}
			else
				commands[k]->cmd_args = allocate_strings(tokens, &i);
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
					commands[k]->files = allocate_files(tokens, &i, commands[k]->files);
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
			node = node->next;
			h++;
		}
		printf("going next command\n");
		j++;
	}
#endif
	return commands;
}