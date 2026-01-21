#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <ctype.h>
#include "lexer.h"


typedef struct token_stack_node{

	token_t* token;
	struct token_stack_node* prev;
	struct token_stack_node* next;

}token_stack_node_t;


typedef struct ast_node ast_node_t;


typedef enum ast_node_type{

	AST_VECTOR,
	AST_BINARY,
	AST_PRIMARY,
	AST_KEYWORD

}ast_node_type_t;


typedef enum keyword{

	KEYW_RETURN,
	KEYW_INT

}keyword_t;



typedef enum primary_type{

	PRIMARY_LITERAL,
	PRIMARY_IDENTIFIER,
	PRIMARY_FUNC_CALL

}primary_type_t;


typedef struct literal{

	union{

		int integer;

	}as;

}literal_t;


typedef struct identifier{

}identifier_t;


typedef struct func_call{

}func_call_t;


typedef struct primary{

	primary_type_t type;

	union{

		literal_t literal;
		identifier_t identifier;
		func_call_t func_call;

	}as;


}primary_t;



typedef struct vector_tree{

	ast_node_t** children;
	int size;
	int capacity;

}vector_tree_t;


typedef struct binary_tree{

	ast_node_t* left;
	char operator;
	ast_node_t* right;

}binary_tree_t;



typedef struct ast_node{


	ast_node_type_t type;

	union{

		keyword_t keyword;

		primary_t primary;

		vector_tree_t vector_tree;

		binary_tree_t binary_tree;

	}as;

}ast_node_t;




token_stack_node_t* token_stack(FILE* cF, int* lc);

void print_ast(ast_node_t ast);
void print_primary(primary_t primary);


ast_node_t ast_keyword_node();
ast_node_t ast_vector_tree_node();
ast_node_t ast_binary_tree_node();
ast_node_t ast_primary_node();

void vec_node_add_right_child(ast_node_t* parent, ast_node_t child);

ast_node_t application(token_stack_node_t** curr);
ast_node_t function_list(token_stack_node_t** curr);
ast_node_t function(token_stack_node_t** curr);
ast_node_t statement_list(token_stack_node_t** curr);
ast_node_t statement(token_stack_node_t** curr);
ast_node_t expression(token_stack_node_t** curr);
ast_node_t equality(token_stack_node_t** curr);
ast_node_t comparison(token_stack_node_t** curr);
ast_node_t term(token_stack_node_t** curr);
ast_node_t factor(token_stack_node_t** curr);
ast_node_t unary(token_stack_node_t** curr);
ast_node_t primary(token_stack_node_t** curr);

int str_to_int(char* str);

bool match_token(token_stack_node_t** curr, int n, ...);
bool check_token(token_stack_node_t* curr, token_type_t type);

void pop_token(token_stack_node_t** curr);
token_stack_node_t* peek_token(token_stack_node_t* curr);


#endif


