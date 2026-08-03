#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "ast.h"
#include "token_stack.h"

ast_node_t build_ast(FILE* cF);

ast_node_t parse_program_node(token_stack_node_t** curr);
function_list_t parse_function_list(token_stack_node_t** curr);
ast_node_t parse_function_node(token_stack_node_t** curr);
function_prototype_t parse_function_prototype(token_stack_node_t** curr);
statement_list_t parse_statement_list(token_stack_node_t** curr);
ast_node_t parse_if_statement_node(token_stack_node_t** curr);
ast_node_t parse_statement_node(token_stack_node_t** curr);
ast_node_t parse_keyword_node(token_stack_node_t** curr, ast_node_t* statement_node);
ast_node_t parse_expression(token_stack_node_t** curr);
ast_node_t parse_equality(token_stack_node_t** curr);
ast_node_t parse_comparison(token_stack_node_t** curr);
ast_node_t parse_term(token_stack_node_t** curr);
void parse_binary_expression_node(token_stack_node_t** curr, ast_node_t* left);
ast_node_t parse_factor(token_stack_node_t** curr);
ast_node_t parse_unary(token_stack_node_t** curr);
ast_node_t parse_primary_node(token_stack_node_t** curr);
primary_t parse_primary_func_call(char* identifier_name);
primary_t parse_primary_variable(char* identifier_name);
primary_t parse_primary_literal(char* token_string);

int str_to_int(char* str);

bool check_token(token_stack_node_t** curr, token_type_t type);
bool match_token(token_stack_node_t** curr, int n, ...);
void synchronize(token_stack_node_t** curr);
void consume_token(token_stack_node_t** curr, token_type_t type, char* error_message);


#endif
