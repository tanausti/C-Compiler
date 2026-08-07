#include <string.h>
#include <stdio.h>
#include "ast.h"
#include "print_ast_nodes_only.h"




void print_ast(FILE* out, ast_node_t ast){

	int indent_level = 0;

	print_ast_child(out, ast, indent_level);


}


void print_ast_child(FILE* out, ast_node_t ast, int indent_level){



	switch(ast.type){

		case AST_PROGRAM:
			{
				print_program_node(out, ast.as.program, indent_level);
				break;
			}
		case AST_FUNCTION:
			{
				print_function_node(out, ast.as.function, indent_level);	
				break;
			}

		case AST_IF_STATEMENT:
			{
				print_if_statement_node(out, ast.as.if_statement, indent_level);	
				break;
			}
		case AST_STATEMENT:
			{
				print_statement_node(out, ast.as.statement, indent_level);
				break;
			}
		case AST_BINARY_EXPRESSION:
			{
				print_binary_expression_node(out, ast.as.binary_expression, indent_level);
				break;
			}
		case AST_KEYWORD:
			{
				print_keyword_node(out, ast.as.keyword, indent_level);
				break;
			}
		case AST_PRIMARY:
			{
				print_primary_node(out, ast.as.primary, indent_level);
				break;
			}
		case AST_ERROR:
			{
				fprintf(out, "-error_node\n");
				break;
			}
		default:
			{
				fprintf(out, "-invalid_node\n");
				break;
			}

	}



}



void indent(FILE* out, int indent_level){


	
	for(int i = 0; i < indent_level; i++){


		fprintf(out, "\t");


	}
	
	

}



void print_program_node(FILE* out, program_t program, int indent_level){

	indent(out, indent_level);
	fprintf(out, "-program\n");

	print_function_list(out, program.function_list, indent_level + 1);


}

void print_function_list(FILE* out, function_list_t function_list, int indent_level){


	for(int i = 0; i < function_list.vector_tree.size; i++){
		print_ast_child(out, *function_list.vector_tree.children[i], indent_level);
	}


}


void print_function_node(FILE* out, function_t function, int indent_level){

	indent(out, indent_level);
	fprintf(out, "-function\n");

	statement_list_t statement_list = function.statement_list;
	print_statement_list(out, statement_list, indent_level + 1);


}




void print_statement_list(FILE* out, statement_list_t statement_list, int indent_level){
	
	for(int i = 0; i < statement_list.vector_tree.size; i++){

		print_ast_child(out, *statement_list.vector_tree.children[i], indent_level + 1);

	}


}



void print_if_statement_node(FILE* out, if_statement_t if_statement, int indent_level){

	indent(out, indent_level);

	fprintf(out, "-if_statement\n");

	print_statement_list(out, if_statement.statement_list, indent_level + 1);


}



void print_statement_node(FILE* out, statement_t statement, int indent_level){

	indent(out, indent_level);

	fprintf(out, "-statement\n");

	for(int i = 0; i < statement.vector_tree.size; i++){
		print_ast_child(out, *statement.vector_tree.children[i], indent_level + 1);
	}



}



void print_keyword_node(FILE* out, keyword_t keyword, int indent_level){

	indent(out, indent_level);
	fprintf(out, "-keyword: ");


	switch(keyword){

		case KEYW_RETURN:
			fprintf(out, "return\n");
			break;
		case KEYW_INT:
			fprintf(out, "int\n");
			break;
		default:
			fprintf(out, "invalid keyword enum. Enum value: %d\n", keyword);
			break;

	}



}






void print_binary_expression_node(FILE* out, binary_expression_t binary_expression, int indent_level){

	indent(out, indent_level);
	fprintf(out, "-binary_expression\n");

	print_primitive_type(out, binary_expression.primitive_type, indent_level + 1);

	print_ast_child(out, *binary_expression.left, indent_level + 1);
	print_ast_child(out, *binary_expression.right, indent_level + 1);


}





void print_primary_node(FILE* out, primary_t primary, int indent_level){

	indent(out, indent_level);
	fprintf(out, "-primary\n");


}




