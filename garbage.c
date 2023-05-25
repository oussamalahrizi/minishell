#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "minishell.h"

typedef struct Token {
    char type;
    char* value;
} Token1;

Token1* new_token(char type, char* value) {
    Token1* token = (Token1*)malloc(sizeof(Token1));
    token->type = type;
    token->value = strdup(value);
    return token;
}


void build_tokens(char* input, Token1** tokens, int* token_count) {
    char* delimiters = "|><";
	int i;

	i = 0;
	while(input[i])
	{
		if (isspace(input[i]))
		{
			i++;
			continue;
		}
		if (input[i] == '\'' || input[i] == '\"')
		{
			char quote = input[i];
			char *string = ft_strdup("");
			append_character(&string, input[i]);
			i++;
			while (input[i] && input[i] != quote)
			{
				append_character(&string, input[i]);
				i++;
			}
			if (!input[i])
			{
				printf("syntax error unmatched quote %c\n", quote);
				exit(1);
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
					if (!input[i])
					{
						printf("syntax error unmatched quote %c\n", quote);
						exit(1);
					}
					append_character(&string, input[i]);
					i++;
					continue;
				}
				append_character(&string, input[i]);
				i++;
			}
			tokens[*token_count] = new_token('s', string);
			(*token_count)++;
			free(string);
			continue;
		}
		if (ft_strchr(delimiters, input[i]))
		{
			char *string = ft_strdup("");
			append_character(&string, input[i]);
			tokens[*token_count] = new_token(input[i], string);
			i++;
			(*token_count)++;
			free(string);
			continue;
		}
		char *string =  ft_strdup("");
		char quote;
		while(input[i] && !ft_strchr(delimiters, input[i]) && !isspace(input[i]))
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
				if (!input[i])
				{
					printf("syntax error unmatched quote %c\n", quote);
					exit(1);
				}
				append_character(&string, input[i]);
				i++;
				continue;
			}
			append_character(&string, input[i]);
			i++;
		}
		tokens[*token_count] = new_token('s', string);
		(*token_count)++;
		free(string);
	}
}

void print_tokens(Token1** tokens, int token_count) {
    for (int i = 0; i < token_count; i++) {
        printf("Token type: %c, Value: %s\n", tokens[i]->type, tokens[i]->value);
    }
}

int main() {
    char input[] = "echo haha\"Hello, world!\"''$USER|grep 'world'>output.txt";
    Token1* tokens[100];
    int token_count = 0;
    
    build_tokens(input, tokens, &token_count);
    print_tokens(tokens, token_count);
    return 0;
}
