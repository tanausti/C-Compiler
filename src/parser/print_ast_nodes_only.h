#ifndef PRINT_AST_H
#define PRINT_AST_H

#include <stdio.h>
#include "ast.h"


void print_ast(FILE* out, ast_node_t ast);
void print_ast_child(FILE* out, ast_node_t ast, int indent_level);

void indent(FILE* out, int indent_level);

void print_program_node(FILE* out, program_t program, int indent_level);
void print_function_list(FILE* out, function_list_t function_list, int indent_level);
void print_function_node(FILE* out, function_t function, int indent_level);
void print_statement_list(FILE* out, statement_list_t statement_list, int indent_level);
void print_if_statement_node(FILE* out, if_statement_t if_statement, int indent_level);
void print_statement_node(FILE* out, statement_t statement, int indent_level);
void print_binary_expression_node(FILE* out, binary_expression_t binary_expression, int indent_level);
void print_keyword_node(FILE* out, keyword_t keyword, int indent_level);
void print_primary_node(FILE* out, primary_t primary, int indent_level);

#endif
