#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../src/lexer/lexer.h"
#include "lexer_tokens_test.h"
#include "../test_harness.h"



int main(int argc, char* argv[]){

	int num_tests = get_num_tests(argc, argv);

	FILE* (*get_expected_out)(int) = &lexer_get_expected_out;
	FILE* (*get_actual_out)(int) = &lexer_get_actual_out;

	test_all_inputs(get_expected_out, get_actual_out, num_tests);


}



FILE* lexer_get_expected_out(int test_num){

	char expected_out_filename[256];
	snprintf(expected_out_filename, sizeof(expected_out_filename), "test/lexer_tokens/expected_out/expected_out%d.txt", test_num);
	FILE* expected_out = fopen(expected_out_filename, "r");
	return expected_out;



}



FILE* lexer_get_actual_out(int test_num){

	char actual_out_filename[256];
	snprintf(actual_out_filename, sizeof(actual_out_filename), "test/lexer_tokens/actual_out/actual_out%d.txt", test_num);
	FILE* actual_out = fopen(actual_out_filename, "w+");

	char input_filename[256];
	snprintf(input_filename, sizeof(input_filename), "test/lexer_tokens/input/input%d.c", test_num);
	FILE* input = fopen(input_filename, "r");

	print_tokens(input, actual_out);
	fclose(input);

	return actual_out;



}





void print_tokens(FILE* input, FILE* out){


	int token_num = 0;
	pos_t lc = {1, 1};

	token_t current_token;
	current_token.type = TOK_ERROR;

	while(current_token.type != TOK_EOF){


		current_token = next_token(input, &lc);

		char enum_str[256];
		assign_enum_str(current_token, enum_str);
		char* tok_str = current_token.string;
		int line = current_token.line;
		int column = current_token.column;

		fprintf(out, "%s, %s, %d, %d\n", enum_str, tok_str, line, column);

		free(tok_str);


	}




}


void assign_enum_str(token_t current_token, char* enum_str){
	

	switch(current_token.type){

		case(TOK_EOF):
			{
				strcpy(enum_str, "TOK_EOF");			
				break;
			}
		case(TOK_INT_TYPE):
			{
				strcpy(enum_str, "TOK_INT_TYPE");			
				break;
			}
		case(TOK_RETURN):
			{
				strcpy(enum_str, "TOK_RETURN");			
				break;
			}
		case(TOK_IF):
			{
				strcpy(enum_str, "TOK_IF");			
				break;
			}

		case(TOK_INT_LITERAL):
			{
				strcpy(enum_str, "TOK_INT_LITERAL");			
				break;
			}
		case(TOK_PLUS):
			{
				strcpy(enum_str, "TOK_PLUS");			
				break;
			}
		case(TOK_EQ):
			{
				strcpy(enum_str, "TOK_EQ");			
				break;
			}
		case(TOK_IDENTIFIER):
			{
				strcpy(enum_str, "TOK_IDENTIFIER");			
				break;
			}
		case(TOK_LPARENTH):
			{
				strcpy(enum_str, "TOK_LPARENTH");			
				break;
			}
		case(TOK_RPARENTH):
			{
				strcpy(enum_str, "TOK_RPARENTH");			
				break;
			}
		case(TOK_LBRACE):
			{
				strcpy(enum_str, "TOK_LBRACE");			
				break;
			}
		case(TOK_RBRACE):
			{
				strcpy(enum_str, "TOK_RBRACE");			
				break;
			}
		case(TOK_SEMI):
			{
				strcpy(enum_str, "TOK_SEMI");			
				break;
			}
		case(TOK_ERROR):
			{
				strcpy(enum_str, "TOK_ERROR");			
				break;
			}
		default:
			{
				strcpy(enum_str, "TOK_UNKNOWN");			
				break;
			}

	}


}
