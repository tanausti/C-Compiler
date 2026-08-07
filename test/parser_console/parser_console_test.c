#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "../../src/parser/parser.h"
#include "../../src/parser/ast.h"
#include "../../src/parser/free_ast.h"
#include "parser_console_test.h"
#include "../test_harness.h"



int main(int argc, char* argv[]){

	int num_tests = get_num_tests(argc, argv);

	FILE* (*get_expected_out)(int) = &parse_error_get_expected_out;
	FILE* (*get_actual_out)(int) = &parse_error_get_actual_out;

	test_all_inputs(get_expected_out, get_actual_out, num_tests);

}



FILE* parse_error_get_expected_out(int test_num){

	char expected_out_filename[256];
	snprintf(expected_out_filename, sizeof(expected_out_filename), "test/parser_console/expected_out/expected_out%d.txt", test_num);
	FILE* expected_out = fopen(expected_out_filename, "r");
	return expected_out;

}



FILE* parse_error_get_actual_out(int test_num){

	char actual_out_filename[256];
	snprintf(actual_out_filename, sizeof(actual_out_filename), "test/parser_console/actual_out/actual_out%d.txt", test_num);
	FILE* actual_out = freopen(actual_out_filename, "w+", stderr);


	char input_filename[256];
	snprintf(input_filename, sizeof(input_filename), "test/parser_console/input/input%d.c", test_num);
	FILE* input = fopen(input_filename, "r");


	console_output(input);
	fflush(stderr);
	freopen("/dev/tty", "w", stderr);

	actual_out = fopen(actual_out_filename, "r");


	fclose(input);

	return actual_out;



}


void console_output(FILE* input){


	ast_node_t head = build_ast(input);
	free_ast(head);


}
