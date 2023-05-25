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

int main(void)
{
	text = "5555*(1+2)";
	build_tokens();
	tk_pos = 0;
	int res = 0;
	res = evaluate(expr());
	printf("res %d\n", res);
}