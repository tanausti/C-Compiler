#include "../../src/parser/parser.h"
#include "../../src/parser/ast.h"
#include "../../src/parser/print_ast.h"
#include "../../src/parser/free_ast.h"
#include "parser_ast_test.h"
#include "../test_harness.h"
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[]){

	int num_tests = get_num_tests(argc, argv);

	FILE* (*get_expected_out)(int) = &parser_get_expected_out;
	FILE* (*get_actual_out)(int) = &parser_get_actual_out;

	test_all_inputs(get_expected_out, get_actual_out, num_tests);


}


FILE* parser_get_expected_out(int test_num){

	char expected_out_filename[256];
	snprintf(expected_out_filename, sizeof(expected_out_filename), "test/parser_ast/expected_out/expected_out%d.txt", test_num);
	FILE* expected_out = fopen(expected_out_filename, "r");
	return expected_out;



}


FILE* parser_get_actual_out(int test_num){

	char actual_out_filename[256];
	snprintf(actual_out_filename, sizeof(actual_out_filename), "test/parser_ast/actual_out/actual_out%d.txt", test_num);
	FILE* actual_out = fopen(actual_out_filename, "w+");

	char input_filename[256];
	snprintf(input_filename, sizeof(input_filename), "test/parser_ast/input/input%d.c", test_num);
	FILE* input = fopen(input_filename, "r");

	ast_node_t ast = build_ast(input);
	print_ast(actual_out, ast);
	free_ast(ast);
	fclose(input);

	return actual_out;



}
