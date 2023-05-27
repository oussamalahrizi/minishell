#include "../minishell.h"

typedef struct
{
	int file;
	char type;
	struct files *next;
} files;

typedef struct
{
	char *cmd;
	char **cmd_args;
	files *infiles;
	files *outfiles;
}	Command;


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
		res[c] = ft_strdup(tokens[i]->value);
		c++;
		i++;
	}
	*index = i;
	res[c] = 0;
	return (res);
}

files *allocate_files(Token **tokens, int *index, files **file_list)
{
	// guaranteed this token type is an 's'
	int i;

	i = *index;
	if (tokens[i]->type != 's')
	return NULL;
}

void extract(Token **tokens)
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
		commands[i]->infiles = NULL;
		commands[i]->outfiles = NULL;
		i++;
	}
	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->type == 's')
		{
			if (commands[k]->cmd == NULL)
			{
				commands[k]->cmd = ft_strdup(tokens[i]->value);
				i++;
			}	
			else
				commands[k]->cmd_args = allocate_strings(tokens, &i);
		}
		else{
			k++;
			if (tokens[i]->type == '|')
			{
				i++;
				continue;
			}
			else if (tokens[i]->type == '>')
			{
				i++;
				if (!tokens[i])
				{
					error("syntax error\n");
					break;
				}
				if (tokens[i]->type == 's')
				{
					commands[k]->infiles = allocate_files(tokens, &i, &commands[k]->infiles);
				}
			}
			else break;
		}
	}
	for(int j = 0; j < k; j++)
	{
		printf("command : %s\n", commands[j]->cmd);
		int h = 0;
		while(commands[j]->cmd_args[h])
		{
			printf("command args: %s\n", commands[j]->cmd_args[h]);
			h++;
		}
	}
}