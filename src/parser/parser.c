#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "token_stack.h"
#include "print_ast.h"
#include "parser.h"
#include "free_ast.h"
#include "ast.h"

#define DEFAULT_NODE_CAPACITY 10000


//pop tokens off the stack and use them to build the AST
ast_node_t build_ast(FILE* cF){

	pos_t lc = {1,1};

	token_stack_node_t* top_node = token_stack(cF, &lc);

	ast_node_t ast = parse_program_node(&top_node);

	return ast;


}


ast_node_t parse_program_node(token_stack_node_t** curr){

	ast_node_t program_node = init_program_node();
	program_node.as.program.function_list = parse_function_list(curr);


	return program_node;

}


function_list_t parse_function_list(token_stack_node_t** curr){

	function_list_t function_list = (function_list_t){init_vector_tree()};


	while(!match_token(curr, 1, TOK_EOF) && check_token(curr, TOK_INT_TYPE)){

		ast_node_t function_node = parse_function_node(curr);
		vec_tree_add_right_child(&function_list.vector_tree, function_node);

	}


	return function_list;

}


ast_node_t parse_function_node(token_stack_node_t** curr){

	ast_node_t function_node = init_function_node();

	function_prototype_t function_prototype;
	statement_list_t statement_list;


	function_prototype = parse_function_prototype(curr);

	consume_token(curr, TOK_LPARENTH, "expected token '('");
	consume_token(curr, TOK_RPARENTH, "expected token ')'");
	consume_token(curr, TOK_LBRACE, "expected token '{'");

	statement_list = parse_statement_list(curr);
	
	if(function_prototype.primitive_type == -1 || function_prototype.function_name == NULL)
	{
		function_node = init_error_node();
		
		token_stack_node_t* next_token_node = peek_token_node(curr);
		
		while(check_token(&next_token_node, TOK_LBRACE)){

			pop_token_node(curr);

		}

		free_statement_list(statement_list);

	}
	else
	{

		function_node.as.function = (function_t){function_prototype, statement_list};
	
	}

	

	return function_node;


}


function_prototype_t parse_function_prototype(token_stack_node_t** curr){

	primitive_type_t function_type;
	char* function_name;


	if(check_token(curr, TOK_INT_TYPE)){
		
		function_type = PRIMITIVE_INT;
	}
	else{
		function_type = -1;
	}

	consume_token(curr, TOK_INT_TYPE, "function header must include type.");


	if(check_token(curr, TOK_IDENTIFIER)){
		
		function_name = strdup((**curr).token->string);
	}
	else{
		
		function_name = NULL;
	}


	consume_token(curr, TOK_IDENTIFIER, "expected identifier.");


	function_prototype_t function_prototype = (function_prototype_t){function_type, function_name};

	return function_prototype;

}



statement_list_t parse_statement_list(token_stack_node_t** curr){

	statement_list_t statement_list = (statement_list_t){init_vector_tree()};

	while(!match_token(curr, 1, TOK_RBRACE) && !check_token(curr, TOK_EOF)){

		ast_node_t statement_node;

		if(match_token(curr, 1, TOK_IF)){

			statement_node = parse_if_statement_node(curr);

		}
		else{

			statement_node = parse_statement_node(curr);

		}


			vec_tree_add_right_child(&statement_list.vector_tree, statement_node);
		
	}

	return statement_list;



}


ast_node_t parse_if_statement_node(token_stack_node_t** curr){

	ast_node_t if_statement_node = init_if_statement_node();

	ast_node_t condition_node;
	statement_list_t then_statement_list;


	consume_token(curr, TOK_LPARENTH, "expected '(' before expression.");

	condition_node = parse_expression(curr);

	consume_token(curr, TOK_RPARENTH, "expected ')' before expression.");
	consume_token(curr, TOK_LBRACE, "expected '{' before if statement body.");

	then_statement_list = parse_statement_list(curr);

	//'}' is matched in parse_statement_list(), so don't consume here

	if_statement_node.as.if_statement = (if_statement_t){malloc(sizeof(condition_node)), then_statement_list};
	*(if_statement_node.as.if_statement.condition_node) = condition_node;
	return if_statement_node;
}



ast_node_t parse_statement_node(token_stack_node_t** curr){

	ast_node_t statement_node = init_statement_node();
	statement_node.as.statement = (statement_t){init_vector_tree()};
	

	//added to vector tree inside function based on a condition in the function, because there may not be a keyword
	parse_keyword_node(curr, &statement_node);

	ast_node_t expression_node = parse_expression(curr);
	vec_tree_add_right_child(&(statement_node.as.statement.vector_tree), expression_node);

	token_t prev_token = *((*curr)->token);

	consume_token(curr, TOK_SEMI, "expected ';' at end of statement.");

	if(prev_token.type != TOK_SEMI){
		synchronize(curr);
	}	

	return statement_node;

}


ast_node_t parse_keyword_node(token_stack_node_t** curr, ast_node_t* statement_node){

	ast_node_t keyword_node = init_keyword_node();


	keyword_t keyword = -1;

	token_type_t token_type = (**curr).token->type;

	//no if, handled in separate function
	if(match_token(curr, 2, TOK_RETURN, TOK_INT_TYPE)){

		switch(token_type){

			case TOK_RETURN:
				keyword = KEYW_RETURN;
				break;
			case TOK_INT_TYPE:
				keyword = KEYW_INT;
				break;
			default:
				break;

		}

		keyword_node.as.keyword = keyword;

		vec_tree_add_right_child(&(statement_node->as.statement.vector_tree), keyword_node);

	}

	return keyword_node;

}


ast_node_t parse_expression(token_stack_node_t** curr){

	return parse_equality(curr);
}


ast_node_t parse_equality(token_stack_node_t** curr){

	return parse_comparison(curr);

}


ast_node_t parse_comparison(token_stack_node_t** curr){

	return parse_term(curr);

}


ast_node_t parse_term(token_stack_node_t** curr){

	ast_node_t left = parse_factor(curr);

	parse_binary_expression_node(curr, &left);

	return left;

}


void parse_binary_expression_node(token_stack_node_t** curr, ast_node_t* left){

	char operator = ((**curr).token->string)[0];

	while(match_token(curr, 2, TOK_PLUS, TOK_EQ)){

		ast_node_t right;

		token_stack_node_t* one_ahead = peek_token_node(curr);
		token_stack_node_t* two_ahead = peek_token_node(&one_ahead);
		token_stack_node_t* three_ahead = peek_token_node(&two_ahead);

		if(check_token(&one_ahead, TOK_SEMI) || check_token(&two_ahead, TOK_RPARENTH) && check_token(&three_ahead, TOK_RBRACE)){

			right = parse_factor(curr);

		}
		else{

			right = parse_term(curr);

		}

		ast_node_t* right_ptr = malloc(sizeof(ast_node_t));
		ast_node_t* left_ptr = malloc(sizeof(ast_node_t));
		*left_ptr = *left;
		*right_ptr = right;

		ast_node_t new_left;
		new_left = init_binary_expression_node();
		new_left.as.binary_expression = (binary_expression_t){left_ptr, operator, right_ptr, PRIMITIVE_INT};
		*left = new_left;

	}

}


ast_node_t parse_factor(token_stack_node_t** curr){

	return parse_unary(curr);

}


ast_node_t parse_unary(token_stack_node_t** curr){

	return parse_primary_node(curr);

}


ast_node_t parse_primary_node(token_stack_node_t** curr){

	ast_node_t primary_node = init_primary_node();
	primary_t primary;


	char* token_string = (**curr).token->string;

	char* name = malloc(strlen(token_string) + 1);
	strcpy(name, token_string);

	if(match_token(curr, 1, TOK_IDENTIFIER)){

		if(match_token(curr, 1, TOK_LPARENTH)){

			primary = parse_primary_func_call(name);
			consume_token(curr, TOK_RPARENTH, "expected ')' to close expression.");
		}
		else{

			primary = parse_primary_variable(name);
		}

	}
	else if(match_token(curr, 1, TOK_INT_LITERAL)){

		primary = parse_primary_literal(name);
		free(name);
	}
	else{
	
		primary_node = init_error_node();
		consume_token(curr, TOK_INT_LITERAL, "expected primary to close expression.");
		free(name);
		
		return primary_node;

	}

	primary_node.as.primary = primary;

	return primary_node;

}



primary_t parse_primary_func_call(char* identifier_name){

	primary_t primary;

	primary_type_t primary_type = PRIMARY_FUNC_CALL;
	func_call_t func_call = (func_call_t){PRIMITIVE_INT, identifier_name};
	
	primary = (primary_t){primary_type, .as.func_call = func_call};

	return primary;

}

primary_t parse_primary_variable(char* identifier_name){

	primary_t primary;

	primary_type_t primary_type = PRIMARY_VARIABLE;

	variable_t variable = (variable_t){PRIMITIVE_INT, identifier_name};

	primary = (primary_t){primary_type, .as.variable = variable};

	return primary;

}


primary_t parse_primary_literal(char* token_string){

	primary_t primary;

	primary_type_t primary_type = PRIMARY_LITERAL;

	literal_t literal;
	int value = str_to_int(token_string);
	literal = (literal_t){primary_type, value};

	primary = (primary_t){primary_type, .as.literal = literal};

	return primary;

}


bool check_token(token_stack_node_t** curr, token_type_t type){

	if(curr == NULL || *curr == NULL || (*curr)->token == NULL){

		fprintf(stderr, "Internal error: Attempt to dereference null token/token node in check_token. Was EOF popped off token stack early?\n");
		return false;
	}


	token_t* curr_token = (**curr).token;
	return (curr_token->type == type);

}


bool match_token(token_stack_node_t** curr, int n, ...){

	va_list args;
	va_start(args, n);


	for(int i = 0; i < n; i++){

		token_type_t token_type = va_arg(args, token_type_t);

		if(check_token(curr, token_type)){

			pop_token_node(curr);
			va_end(args);
			return true;

		}

	}


	va_end(args);

	return false;

}


void consume_token(token_stack_node_t** curr, token_type_t type, char* error_message){

	if(!check_token(curr, type)){

		fprintf(stderr, "Syntax error (%d, %d): %s\n", (**curr).token->line, (**curr).token->column, error_message);

	}
	else{

		pop_token_node(curr);

	}

}


void synchronize(token_stack_node_t** curr){

	token_t previous_token = *((**curr).token);
	
	if(!check_token(curr, TOK_EOF)){
		pop_token_node(curr);
	}
	

	while(!check_token(curr, TOK_EOF)){
	
		if(previous_token.type == TOK_SEMI){
			return;
		}


		switch(peek_token_node(curr)->token->type){

			case TOK_INT_TYPE:
				return;
			case TOK_RETURN:
				return;
		}

		pop_token_node(curr);

	}

	
	previous_token = *((**curr).token);


}


void vec_tree_add_right_child(vector_tree_t* parent_tree, ast_node_t child){


	ast_node_t* child_ptr = malloc(sizeof(child));
	*child_ptr = child;

	if(parent_tree->children != NULL && parent_tree->size <= parent_tree->capacity){

		ast_node_t** vec = parent_tree->children;

		vec = realloc(vec, ++(parent_tree->size) * sizeof(ast_node_t*));

		parent_tree->children = vec;

		int size = parent_tree->size;

		memcpy(vec + (size - 1), &child_ptr, sizeof(ast_node_t*));


	}else{

		fprintf(stderr, "Internal error: Vector tree not initialized properly!\n");

	}


}




int str_to_int(char* str){


	char* end;
	errno = 0;
	long v = strtol(str, &end, 10);
	int literal_value = (int)v;

	char first_digit = str[0];

	if(literal_value == 0 && first_digit != '0'){

		fprintf(stderr, "Internal error: Unaccounted token: %c\n", first_digit);

	}

	return literal_value;


}



ast_node_t init_error_node(){

	ast_node_t ast_node;
	ast_node.type = AST_ERROR;
	return ast_node;

}
ast_node_t init_program_node(){

	ast_node_t ast_node;
	ast_node.type = AST_PROGRAM;
	return ast_node;

}
ast_node_t init_keyword_node(){

	ast_node_t ast_node;
	ast_node.type = AST_KEYWORD;
	return ast_node;

}
ast_node_t init_primary_node(){

	ast_node_t ast_node;
	ast_node.type = AST_PRIMARY;
	return ast_node;
}
ast_node_t init_function_node(){

	ast_node_t ast_node;
	ast_node.type = AST_FUNCTION;
	return ast_node;

}
ast_node_t init_if_statement_node(){

	ast_node_t ast_node;
	ast_node.type = AST_IF_STATEMENT;
	return ast_node;

}
ast_node_t init_statement_node(){

	ast_node_t ast_node;
	ast_node.type = AST_STATEMENT;
	return ast_node;

}
ast_node_t init_binary_expression_node(){

	ast_node_t ast_node;
	ast_node.type = AST_BINARY_EXPRESSION;
	return ast_node;

}

vector_tree_t init_vector_tree(){

	vector_tree_t vector_tree;
	vector_tree.size = 0;
	vector_tree.capacity = DEFAULT_NODE_CAPACITY;
	vector_tree.children = malloc(sizeof(ast_node_t*));

	return vector_tree;

}




