#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/parser/parser.h"
#include "../../src/parser/ast.h"
#include "../../src/parser/free_ast.h"
#include "../../src/ir/ir.h"
#include "ir_test.h"
#include "../test_harness.h"





int main(int argc, char* argv[]){

	int num_tests = get_num_tests(argc, argv);
	FILE* (*get_expected_out)(int) = &ir_get_expected_out;
	FILE* (*get_actual_out)(int) = &ir_get_actual_out;

	test_all_inputs(get_expected_out, get_actual_out, num_tests);

	return 0;

}



FILE* ir_get_expected_out(int test_num){

	char expected_out_filename[256];
	snprintf(expected_out_filename, sizeof(expected_out_filename), "test/ir/expected_out/expected_out%d.ll", test_num);
	FILE* expected_out = fopen(expected_out_filename, "r");
	return expected_out;

}



FILE* ir_get_actual_out(int test_num){

	char input_filename[256];
	snprintf(input_filename, sizeof(input_filename), "test/ir/input/input%d.c", test_num);
	FILE* input = fopen(input_filename, "r");
	
	//these tests only take valid ASTs (no error nodes)
	ast_node_t ast = build_ast(input);
	fclose(input);

	char actual_out_filename[256];
	snprintf(actual_out_filename, sizeof(actual_out_filename), "test/ir/actual_out/actual_out%d.ll", test_num); 
	generate_llvm_to_file(actual_out_filename, ast);

	free_ast(ast);
	FILE* generated_llvm_file = fopen(actual_out_filename, "r");
	return generated_llvm_file;

}



