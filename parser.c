#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "lexer.h"
#include "parser.h"

#define DEFAULT_NODE_CAPACITY 20





int main(int argc, char* argv[]){

	FILE *cF = fopen(argv[1], "r");

	int lc[2] = {1, 1};

	token_stack_node_t* head = token_stack(cF, lc);

	ast_node_t ast = application(&head);

	print_ast(ast);

	return 0;



}


void print_ast(ast_node_t ast){


	switch(ast.type){
		case AST_PRIMARY:{
				 puts("primary");}
			 break;
		case AST_KEYWORD:{
				 printf("KEYWORD\n");}
			 break;
		case AST_BINARY:{
				 print_ast(*ast.as.binary_tree.left);
				 printf("%c\n", ast.as.binary_tree.operator);
				 print_ast(*ast.as.binary_tree.right);}
			 break;
		case AST_VECTOR:{
				 const int size = ast.as.vector_tree.size;

				 for(int i = 0; i < size; i++){

					 print_ast(**(ast.as.vector_tree.children + i * sizeof(ast_node_t*)));

				 }}
			 break;
		default:
			 puts("no ast node type enum assigned!");
			 break;

	}

}


void print_primary(primary_t primary){


	switch(primary.type){
		case PRIMARY_LITERAL:{
				     const int x = primary.as.literal.as.integer;
				     printf("%d\n", x);}
			     break;
		case PRIMARY_IDENTIFIER:{
					printf("IDENTIFIER\n");}
				break;
		case PRIMARY_FUNC_CALL:{
				       printf("FUNC_CALL\n");}
			       break;
		default:
			       puts("no primary enum type assigned!");
			       break;

	}


}




ast_node_t ast_primary_node(){

	ast_node_t ast_node;
	ast_node.type = AST_PRIMARY;
	ast_node.as.primary.as.literal.as.integer = -1;
	return ast_node;
}


ast_node_t ast_keyword_node(){

	ast_node_t ast_node;
	ast_node.type = AST_KEYWORD;
	return ast_node;

}


ast_node_t ast_vector_tree_node(){


	ast_node_t ast_node;
	ast_node.type = AST_VECTOR;
	ast_node.as.vector_tree.size = 0;
	ast_node.as.vector_tree.capacity = DEFAULT_NODE_CAPACITY;
	ast_node.as.vector_tree.children = malloc(sizeof(ast_node_t*));
	return ast_node;


}


ast_node_t ast_binary_tree_node(){

	ast_node_t ast_node;
	ast_node.type = AST_BINARY;
	return ast_node;


}



void vec_node_add_right_child(ast_node_t* parent, ast_node_t child){

	if(parent->type == AST_VECTOR){

		ast_node_t** vec = parent->as.vector_tree.children;
		ast_node_t* childPtr = malloc(sizeof(child));
		*childPtr = child;

		if(vec != NULL && parent->as.vector_tree.size <= parent->as.vector_tree.capacity){


			vec = realloc(vec, ++(parent->as.vector_tree.size) * sizeof(ast_node_t*));

			const int size = parent->as.vector_tree.size;

			memcpy(vec + (size - 1) * sizeof(ast_node_t*), &childPtr, sizeof(ast_node_t*));


		}else{

			puts("not initialized properly");

		}
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

	ast_node_t function_list = ast_vector_tree_node();

	while(match_token(curr, 1, TOK_INT_TYPE) && 
			match_token(curr, 1, TOK_IDENTIFIER) && 
			match_token(curr, 1, TOK_LPARENTH) && 
			match_token(curr, 1, TOK_RPARENTH) && match_token(curr, 1, TOK_LBRACE)){

		ast_node_t func = function(curr);
		vec_node_add_right_child(&function_list, func);
		printf("current token: %s\n", (**curr).token->string);

	}

	return function_list;

}


ast_node_t function(token_stack_node_t** curr){

	return statement_list(curr);


}



ast_node_t statement_list(token_stack_node_t** curr){

	ast_node_t statement_list = ast_vector_tree_node();

	if(!match_token(curr, 1, TOK_RBRACE)){

		ast_node_t statement_var = statement(curr);
		vec_node_add_right_child(&statement_list, statement_var);

		while(match_token(curr, 1, TOK_SEMI) && !match_token(curr, 1, TOK_RBRACE)){

			statement_var = statement(curr);

			vec_node_add_right_child(&statement_list, statement_var);

		}




	}

	return statement_list;



}



ast_node_t statement(token_stack_node_t** curr){

	ast_node_t statement = ast_vector_tree_node();

	while(match_token(curr, 2, TOK_RETURN, TOK_INT_TYPE)){

		ast_node_t keyword_node;
		keyword_node.type = AST_KEYWORD;

		switch(((*curr)->prev->token->type)){

			case TOK_RETURN:
				keyword_node.as.keyword = KEYW_RETURN;
				break;
			case TOK_INT_TYPE:
				keyword_node.as.keyword = KEYW_INT;
				break;

		}

		vec_node_add_right_child(&statement, keyword_node);



	}

	ast_node_t expression_var = expression(curr);

	vec_node_add_right_child(&statement, expression_var);


	return statement;


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
		new_left.as.binary_tree = (binary_tree_t){leftPtr, operator, rightPtr};
		new_left.type = AST_BINARY;
		left = new_left;


	}

	return left;



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

			printf("the token responsible: %s\n", (**curr).token->string);
			ast.as.primary.type = PRIMARY_FUNC_CALL;
			ast.as.primary.as.func_call = (func_call_t){};
			pop_token(curr);
			pop_token(curr);

		}
		else{
			printf("identifier: %s\n", (**curr).token->string);
			ast.as.primary.type = PRIMARY_IDENTIFIER;
			ast.as.primary.as.identifier = (identifier_t){};

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


void pop_token(token_stack_node_t** curr){

	*curr = peek_token(*curr);

}



token_stack_node_t* peek_token(token_stack_node_t* curr){

	return curr->next;


}
