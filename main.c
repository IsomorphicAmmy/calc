#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum TokenType
{
	NUMBER,
	PLUS_OPERATOR,
	MULT_OPERATOR
};

struct Token
{
	enum TokenType type;
	int value;
};

struct TokenArray
{
	int count;
	struct Token *arr_ptr;
};

struct Node
{
	struct Token node_token;
	struct Node *left;
	struct Node *right;
};

void AddToken(struct TokenArray *input_arr, struct Token input_token)
{
	if (input_arr->count == 0)
	{
		input_arr->arr_ptr = malloc(sizeof(struct Token));
		input_arr->arr_ptr[0] = input_token;
	}
	else
	{
		input_arr->arr_ptr = realloc(input_arr->arr_ptr, sizeof(struct Token) * (input_arr->count + 1));
		input_arr->arr_ptr[input_arr->count] = input_token;
	}
	input_arr->count++;
}

struct TokenArray ParseTokens(char *input_string)
{
	struct TokenArray result = {0, NULL};
	for (int i = 0; input_string[i] != '\0'; i++)
	{
		if (input_string[i] >= '0' && input_string[i] <= '9')
		{
			int d = 0;
			int n = 0;
			int buffer[32];

			while (input_string[i] >= '0' && input_string[i] <= '9')
			{
				buffer[d] = input_string[i] - '0';
				i++;
				d++;
			}

			for (int j = 0; d > 0; j++)
			{
				n += buffer[j]*pow(10, d - 1);
				d--;
			}

			struct Token token = {NUMBER, n};
			AddToken(&result, token);
			i--;
		}
		else if (input_string[i] == '+')
		{
			struct Token token = {PLUS_OPERATOR, 0};
			AddToken(&result, token);
		}
		else if (input_string[i] == '*')
		{
			struct Token token = {MULT_OPERATOR, 0};
			AddToken(&result, token);
		}
		else if (input_string[i] == ' ')
		{
			continue;
		}
	}
	return result;
}

struct Node ParseNode(struct TokenArray input_array)
{
	struct Node result;
	if (input_array.count == 1)
	{
		result.node_token = input_array.arr_ptr[0];
	}
	else 
	{
		int i = 0;
		while(i + 1 < input_array.count)
		{
			if (input_array.arr_ptr[i].type == PLUS_OPERATOR)
			{
				result.node_token = input_array.arr_ptr[i];
	
				struct TokenArray left_buffer = {0, NULL};
				struct TokenArray right_buffer = {0, NULL};
	
				for (int j = 0; j < i; j++)
				{
					AddToken(&left_buffer, input_array.arr_ptr[j]);
				}
				for (int j = 1; j < input_array.count - i; j++)
				{
					AddToken(&right_buffer, input_array.arr_ptr[i + j]);
				}
	
				struct Node left_node = ParseNode(left_buffer);
				struct Node right_node = ParseNode(right_buffer);
				result.left = malloc(sizeof(struct Node));
				*result.left = left_node;
				result.right = malloc(sizeof(struct Node));
				*result.right = right_node;
				break;	
			}
			else if (i + 3 > input_array.count)
			{
				result.node_token = input_array.arr_ptr[i];
	
				struct TokenArray left_buffer = {0, NULL};
				struct TokenArray right_buffer = {0, NULL};
	
				for (int j = 0; j < i; j++)
				{
					AddToken(&left_buffer, input_array.arr_ptr[j]);
				}
	
				struct Node left_node = ParseNode(left_buffer);
				struct Node right_node = {input_array.arr_ptr[i + 1], NULL, NULL};
				result.left = malloc(sizeof(struct Node));
				*result.left = left_node;
				result.right = malloc(sizeof(struct Node));
				*result.right = right_node;

			}
			i++;
		}
	}
	return result;
}

int NodeEvaluate(struct Node input_node)
{
	if (input_node.node_token.type != NUMBER)
	{
		if (input_node.left->node_token.type != NUMBER)
		{
			input_node.left->node_token.value = NodeEvaluate(*input_node.left);
		}
		if (input_node.right->node_token.type != NUMBER)
		{
			input_node.right->node_token.value = NodeEvaluate(*input_node.right);
		}

		if (input_node.node_token.type == PLUS_OPERATOR)
		{
			return input_node.left->node_token.value + input_node.right->node_token.value;
		}
		else if (input_node.node_token.type == MULT_OPERATOR)
		{
			return input_node.left->node_token.value * input_node.right->node_token.value;
		}
	}
	else
	{
		return input_node.node_token.value;
	}
}

int main(void)
{
	char input_buffer[128];
	fgets(input_buffer, 64, stdin);
	struct TokenArray token_array = ParseTokens(input_buffer);

	struct Node node = ParseNode(token_array);
	
	int result = NodeEvaluate(node);
	printf("%d\n", result);

	return 0;
}
