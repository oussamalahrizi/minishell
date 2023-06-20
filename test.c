#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *text;
int txt_pos;

typedef struct Token
{
	char type;
	int value;
} Token;

typedef struct Node
{
	struct Node *left;
	struct Node *right;
	Token *token;
} Node;

Token *tokens[100];
int tk_pos;

Token *new_token(char type)
{
	Token *new = calloc(1, sizeof(Token));
	new->type = type;
	tokens[tk_pos++] = new;
	printf("new token ");
	if(new->type == 'n')
		printf("with type number ");
	if(new->type == 'e')
		printf("with type end\n");
	if(new->type && strchr("+-", new->type))
		printf("with type %c\n", new->type);

	return new;
}

Node *new_node(Token *token)
{
	Node *new = calloc(1, sizeof(Node));
	if(new->left == NULL)
		printf("node->left is null\n");
	new->token = token;
	return new;
}

void build_tokens(void)
{
	while(text && text[txt_pos])
	{
		if(isspace(text[txt_pos]))
		{
			txt_pos++;
			continue;
		}
		if(text[txt_pos] && strchr("+-*/()",text[txt_pos]))
		{
			new_token(text[txt_pos]);
			txt_pos++;
			continue;
		}
		if(isdigit(text[txt_pos]))
		{

			Token *token = new_token('n');
			int n = 0;
			while(isdigit(text[txt_pos]))
			{
				n = n * 10 + text[txt_pos] - '0';
				txt_pos++;
			}
			
			token->value = n;
			printf("has value %d\n", token->value);
			continue;
		}
	}
	new_token('e');
}

Node *expr();
Node *add_sub();
Node *mul_div();
Node *prime();

void skip(char type)
{
	if(tokens[tk_pos]->type != type)
	{
		printf("Error expected %c\n", type);
		exit(1);
	}
	tk_pos++;
}

Node *expr()
{
	return add_sub();
}

Node *add_sub()
{
	Node *left = mul_div();
	while(tokens[tk_pos] && strchr("+-", tokens[tk_pos]->type))
	{
		Node *node = new_node(tokens[tk_pos]);
		skip(tokens[tk_pos]->type);
		node->left = left;
		node->right = mul_div();
		left = node;
	}
	return left;
}

Node *mul_div()
{
	Node *left = prime();
	while(tokens[tk_pos] && strchr("*/", tokens[tk_pos]->type))
	{
		Node *node = new_node(tokens[tk_pos]);
		skip(tokens[tk_pos]->type);
		node->left = left;
		node->right = prime();
		left = node;
	}
	return left;
}

Node *prime()
{
	if(tokens[tk_pos]->type == 'n')
	{
		Node *node = new_node(tokens[tk_pos]);
		skip(tokens[tk_pos]->type);
		return node;
	}
	if(tokens[tk_pos]->type == '(')
	{
		skip('(');
		Node *node = expr();
		skip(')');
		return node;
	}
	return NULL;
}

int evaluate(Node *node)
{
	switch (node->token->type)
	{
	case 'n':
	{
		return node->token->value;
	}
	case '+':
	case '-':
	case '*':
	case '/':
	{
		int left = evaluate(node->left);
		int right = evaluate(node->right);
		if(node->token->type == '+')
			return left + right;
		if(node->token->type == '-')
			return left - right;
		if(node->token->type == '/')
			return left / right;
		if(node->token->type == '*')
			return left * right;
		break;
	}
	default:
		printf("Error");
		exit(1);
		break;
	}
	return -1;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <string.h>

// int main() {

// 	char *input;
// 	char **doc = malloc(sizeof(char *) * 1000);
// 	int i = 0;
// 	while (1)
// 	{
// 		input = readline("< ");
// 		if (!strcmp(input, "del"))
// 		{
// 			doc[i] = 0;
// 			break;
// 		}
// 		doc[i] = strdup(input);
// 		i++;
// 	}
//     pid_t child = fork();
//     if (child == 0) {
//         // Child process
//         char* args[] = {"cat", NULL};
//         execvp("cat", args);
//     } else {
//         // Parent process
//         // Close the write end of the pipe
//         close(STDIN_FILENO);
// 		dup2(STDIN_FILENO, child);

// 		i = 0;
// 		while (doc[i])
// 		{
// 			write(child, doc[i], strlen(doc[i]));
// 			i++;
// 		}
//         // Wait for the child process to finish
//         wait(NULL);
//     }

//     return 0;
// }


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t child = fork();
    if (child == 0) {
        // Child process

        // Close unnecessary file descriptors
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        // Duplicate stdin to fd 3
        dup2(STDIN_FILENO, 3);

        // Execute the cat command
        char* args[] = {"cat", NULL};
        execve("/bin/cat", args, NULL);
    } else {
        // Parent process

        // Write the heredoc content to the child's stdin
        const char* heredocContent = "This is the heredoc content\n";
        size_t contentSize = strlen(heredocContent);
        write(3, heredocContent, contentSize);

        // Wait for the child process to finish
        wait(NULL);
    }

    return 0;
}
