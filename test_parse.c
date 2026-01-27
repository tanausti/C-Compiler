#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "lexer.h"
#include "parser.h"

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

		case AST_FUNCTION_LIST:
			{
				const int size = ast.as.function_list.vector_tree.size;
				for(int i = 0; i < size; i++){
					print_ast(*ast.as.function_list.vector_tree.children[i]);
				}
				break;
			}
		case AST_FUNCTION:
			{

				function_prototype_t function_prototype = ast.as.function.function_prototype;

				printf("Function type: %d\n, function name: %s\n", function_prototype.primitive_type, function_prototype.function_name);


				const vector_tree_t vector_tree = ast.as.function.statement_list.vector_tree;
				const int size = vector_tree.size;
				for(int i = 0; i < size; i++){
					print_ast(*vector_tree.children[i]);
				}
				break;
			}
		case AST_STATEMENT_LIST:
			{
	
