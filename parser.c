#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "lexer.h"
#include "parser.h"

#define DEFAULT_NODE_CAPACITY 10000




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
ast_node_t init_function_list_node(){

	ast_node_t ast_node;
	ast_node.type = AST_FUNCTION_LIST;
	return ast_node;

}
ast_node_t init_function_node(){

	ast_node_t ast_node;
	ast_node.type = AST_FUNCTION;
	return ast_node;

}
ast_node_t init_statement_list_node(){

	ast_node_t ast_node;
	ast_node.type = AST_STATEMENT_LIST;
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



void vec_node_add_right_child(vector_tree_t* parent_tree, ast_node_t child){


	ast_node_t* childPtr = malloc(sizeof(child));
	*childPtr = child;

	if(parent_tree->children != NULL && parent_tree->size <= parent_tree->capacity){

		ast_node_t** vec = parent_tree->children;

		vec = realloc(vec, ++(parent_tree->size) * sizeof(ast_node_t*));

		parent_tree->children = vec;

		const int size = parent_tree->size;

		memcpy(vec + (size - 1), &childPtr, sizeof(ast_node_t*));


	}else{

		puts("not initialized properly");

	}


}


token_stack_node_t* token_stack(FILE* cF, int* lc){


	token_stack_node_t* curr = (token_stack_node_t*)malloc(sizeof(token_stack_node_t));

	curr->token = (token_t*)malloc(sizeof(token_t));
	*(curr->token) = next_token(cF, lc);
	curr->prev = NULL;

	token_stack_node_t* head = curr;

	while(curr->token->type != TOK_EOF){

		token_stack_node_t* temp_prev = curr;

		curr->next = (token_stack_node_t*)malloc(sizeof(token_stack_node_t));

		curr = curr->next;
		curr->token = (token_t*)malloc(sizeof(token_t));
		*(curr->token) = next_token(cF, lc);

		curr->prev = temp_prev;


	}



	return head;


}



ast_node_t application(token_stack_node_t** curr){

	return function_list(curr);


}




ast_node_t function_list(token_stack_node_t** curr){

	ast_node_t function_list_node = init_function_list_node();
	function_list_node.as.function_list.vector_tree = init_vector_tree();

	while(match_token(curr, 1, TOK_INT_TYPE) && 
			match_token(curr, 1, TOK_IDENTIFIER) && 
			match_token(curr, 1, TOK_LPARENTH) && 
			match_token(curr, 1, TOK_RPARENTH) && match_token(curr, 1, TOK_LBRACE)){

		ast_node_t function_node = function(curr);

		vec_node_add_right_child(&function_list_node.as.function_list.vector_tree, function_node);

	}

	return function_list_node;

}


ast_node_t function(token_stack_node_t** curr){


	ast_node_t function_node = init_function_node();

	function_node.as.function.function_prototype = function_prototype(curr);

	ast_node_t statement_list_node = statement_list(curr);

	function_node.as.function.statement_list = statement_list_node.as.statement_list;

	return function_node;


}


function_prototype_t function_prototype(token_stack_node_t** curr){


	token_stack_node_t* stack_node = (*curr);

	while(stack_node->token->type != TOK_LPARENTH){

		stack_node = stack_node->prev;

	}

	char* name = stack_node->prev->token->string;

	char* primitive_string = stack_node->prev->prev->token->string;

	primitive_type_t primitive_type;

	
	switch(primitive_string[0]){
		case 'i':
			primitive_type = PRIMITIVE_INT;
			break;
	}


	function_prototype_t function_prototype;

	function_prototype.primitive_type = primitive_type;
	function_prototype.function_name = (char*)strdup(name);

	return function_prototype;


}


ast_node_t statement_list(token_stack_node_t** curr){

	ast_node_t statement_list_node = init_statement_list_node();	
	statement_list_node.as.statement_list.vector_tree = init_vector_tree();

	if(!match_token(curr, 1, TOK_RBRACE)){

		ast_node_t statement_var = statement(curr);
		vec_node_add_right_child(&statement_list_node.as.statement_list.vector_tree, statement_var);

		while(match_token(curr, 1, TOK_SEMI) && !match_token(curr, 1, TOK_RBRACE)){

			statement_var = statement(curr);

			vec_node_add_right_child(&statement_list_node.as.statement_list.vector_tree, statement_var);

		}

	}

	return statement_list_node;



}



ast_node_t statement(token_stack_node_t** curr){

	ast_node_t statement_node = init_statement_node();
	statement_node.as.statement.vector_tree = init_vector_tree();

	while(match_token(curr, 2, TOK_RETURN, TOK_INT_TYPE)){

		ast_node_t keyword_node = init_keyword_node();

		switch(((*curr)->prev->token->type)){

			case TOK_RETURN:
				keyword_node.as.keyword = KEYW_RETURN;
				break;
			case TOK_INT_TYPE:
				keyword_node.as.keyword = KEYW_INT;
				break;

		}

		vec_node_add_right_child(&statement_node.as.statement.vector_tree, keyword_node);

	}

	ast_node_t expression_var = expression(curr);

	vec_node_add_right_child(&statement_node.as.statement.vector_tree, expression_var);

	return statement_node;

}




ast_node_t expression(token_stack_node_t** curr){

	return equality(curr);
}


ast_node_t equality(token_stack_node_t** curr){

	return comparison(curr);

}


ast_node_t comparison(token_stack_node_t** curr){

	return term(curr);

}


ast_node_t term(token_stack_node_t** curr){

	ast_node_t left = factor(curr);

	left.type = AST_PRIMARY;

	while(match_token(curr, 1, TOK_PLUS)){

		left.type = AST_PRIMARY;
		char operator = ((*curr)->prev->token->string)[0];

		ast_node_t right = factor(curr);
		right.type = AST_PRIMARY;

		ast_node_t* rightPtr = malloc(sizeof(right));
		ast_node_t* leftPtr = malloc(sizeof(left));
		*leftPtr = left;
		*rightPtr = right;

		ast_node_t new_left;

		primitive_type_t primitive_type = get_primitive_type();

		new_left.as.binary_expression = (binary_expression_t){leftPtr, operator, rightPtr, primitive_type};
		new_left.type = AST_BINARY_EXPRESSION;
		left = new_left;

	}

	return left;



}


primitive_type_t get_primitive_type(){

	return PRIMITIVE_INT;

}




ast_node_t factor(token_stack_node_t** curr){

	return unary(curr);


}


ast_node_t unary(token_stack_node_t** curr){

	return primary(curr);


}


ast_node_t primary(token_stack_node_t** curr){

	ast_node_t ast;

	if((*curr)->token->type == TOK_IDENTIFIER){

		if(peek_token(*curr)->token->type == TOK_LPARENTH && 
				(peek_token(peek_token(*curr))->token->type) == TOK_RPARENTH){

			ast.as.primary.type = PRIMARY_FUNC_CALL;
			ast.as.primary.as.func_call = (func_call_t){get_primitive_type(), (*curr)->token->string};
			pop_token(curr);
			pop_token(curr);


		}
		else{

			//var

		}

	}
	else{

		ast.as.primary.type = PRIMARY_LITERAL;
		char* primary_str = (*curr)->token->string;
		int value = str_to_int(primary_str);

		ast.as.primary.as.literal.as.integer = value;


	}

	pop_token(curr);	

	return ast;

}




int str_to_int(char* str){


	char* end;
	errno = 0;
	long v = strtol(str, &end, 10);
	int literal_value = (int)v;

	char first_digit = str[0];

	if(literal_value == 0 && first_digit != '0'){

		fprintf(stderr, "unaccounted token: %c\n", first_digit);

	}

	return literal_value;


}



bool match_token(token_stack_node_t** curr, int n, ...){


	va_list args;
	va_start(args, n);


	for(int i = 0; i < n; i++){


		token_type_t token_type = va_arg(args, token_type_t);

		if(check_token(*curr, token_type)){

			pop_token(curr);
			va_end(args);
			return true;



		}





	}



	va_end(args);

	return false;


}




bool check_token(token_stack_node_t* curr, token_type_t type){

	token_t curr_token = *(curr->token);

	return (curr_token.type == type);

}


token_stack_node_t* pop_token(token_stack_node_t** curr){

	token_stack_node_t* node = *curr;
	*curr = peek_token(*curr);

	return node;

}



token_stack_node_t* peek_token(token_stack_node_t* curr){

	return curr->next;


}
