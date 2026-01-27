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


typedef struct vector_tree{

	ast_node_t** children;

	int size;
	int capacity;

}vector_tree_t;


typedef enum primitive_type{

	PRIMITIVE_INT

}primitive_type_t;


typedef struct literal{


	primitive_type_t primitive_type;

	union{

		int integer;

	}as;

}literal_t;


typedef struct func_call{

	primitive_type_t primitive_type;

	char* callee;


}func_call_t;


typedef enum primary_type{

	PRIMARY_LITERAL,
	PRIMARY_FUNC_CALL


}primary_type_t;

typedef struct primary{

	primary_type_t type;

	union{

		literal_t literal;
		func_call_t func_call;

	}as;


}primary_t;


typedef struct function_list{

	vector_tree_t vector_tree;

}function_list_t;


typedef struct function_prototype{

	primitive_type_t primitive_type;

	char* function_name;

}function_prototype_t;


typedef struct statement_list{

	vector_tree_t vector_tree;

}statement_list_t;


typedef struct statement{

	vector_tree_t vector_tree;

}statement_t;


typedef struct function{

	function_prototype_t function_prototype;

	statement_list_t statement_list;

}function_t;


typedef enum keyword{

	KEYW_RETURN,
	KEYW_INT

}keyword_t;


typedef struct binary_expression{

	ast_node_t* left;
	char operator;
	ast_node_t* right;

	primitive_type_t primitive_type;

}binary_expression_t;



typedef enum ast_node_type{


	AST_FUNCTION_LIST,
	AST_FUNCTION,
	AST_STATEMENT_LIST,
	AST_STATEMENT,
	AST_BINARY_EXPRESSION,	
	AST_KEYWORD,
	AST_PRIMARY


}ast_node_type_t;


typedef struct ast_node{


	ast_node_type_t type;

	union{

		function_list_t function_list;

		function_t function;

		statement_list_t statement_list;

		statement_t statement;

		keyword_t keyword;

		binary_expression_t binary_expression;

		primary_t primary;

	}as;

}ast_node_t;




token_stack_node_t* token_stack(FILE* cF, int* lc);

void print_ast(ast_node_t ast);
void print_primary(primary_t primary);



ast_node_t init_keyword_node();
ast_node_t init_primary_node();
ast_node_t init_function_list_node();
ast_node_t init_function_node();
ast_node_t init_statement_list_node();
ast_node_t init_statement_node();
ast_node_t init_binary_expression_node();
vector_tree_t init_vector_tree();



void vec_node_add_right_child(vector_tree_t* parent, ast_node_t child);




ast_node_t application(token_stack_node_t** curr);
ast_node_t function_list(token_stack_node_t** curr);
ast_node_t function(token_stack_node_t** curr);
function_prototype_t function_prototype(token_stack_node_t** curr);
ast_node_t statement_list(token_stack_node_t** curr);
ast_node_t statement(token_stack_node_t** curr);
ast_node_t expression(token_stack_node_t** curr);
primitive_type_t get_primitive_type();
ast_node_t equality(token_stack_node_t** curr);
ast_node_t comparison(token_stack_node_t** curr);
ast_node_t term(token_stack_node_t** curr);
ast_node_t factor(token_stack_node_t** curr);
ast_node_t unary(token_stack_node_t** curr);
ast_node_t primary(token_stack_node_t** curr);

int str_to_int(char* str);

bool match_token(token_stack_node_t** curr, int n, ...);
bool check_token(token_stack_node_t* curr, token_type_t type);

token_stack_node_t* pop_token(token_stack_node_t** curr);
token_stack_node_t* peek_token(token_stack_node_t* curr);


#endif


