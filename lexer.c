#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "lexer.h"

#define MAX_TOKEN_LENGTH 10


token_t next_token(FILE *cF, int* lc){

	int c = advance_char(cF, lc);

	int line = lc[0];
	int column = lc[1];



	while(isspace(c)){

		c = advance_char(cF, lc);

	}




	switch(c){

		case EOF:
			{
				char* string = strdup("EOF");
				return (token_t){TOK_EOF, string, line, column};
				break;
			}
		case '+':
			{
				char* string = strdup("+");
				return (token_t){TOK_PLUS, string, line, column};
				break;
			}

		case '(':
			{
				char* string = strdup("(");
				return (token_t){TOK_LPARENTH, string, line, column};
				break;
			}

		case ')':
			{
				char* string = strdup(")");
				return (token_t){TOK_RPARENTH, string, line, column};
				break;
			}

		case '{':
			{
				char* string = strdup("{");
				return (token_t){TOK_LBRACE, string, line, column};
				break;
			}

		case '}':
			{
				char* string = strdup("}");
				return (token_t){TOK_RBRACE, string, line, column};
				break;
			}

		case ';':
			{
				char* string = strdup(";");
				return (token_t){TOK_SEMI, string, line, column};
				break;
			}

	}

	if(c >= '0' && c <= '9'){

		return create_constant_token(cF, c, lc);

	}

	if(isalpha(c) || c == '_'){

		return create_keyword_or_identifier_token(cF, c, lc);

	}

	return (token_t){TOK_UNKNOWN, "UNKNOWN", line, column};


}



int advance_char(FILE *cF, int* lc){

	int* line = lc;
	int* column = lc + 1;

	int c = fgetc(cF);

	if(c == '\n'){

		(*line)++;
		*column = 1;

	}
	if(c == '\t'){

		(*column) += 8;

	}

	return c;

}


int retreat_char(FILE *cF, int* lc, int* retreat_data){

	int* line = lc;
	int* column = lc + 1;

	char c = retreat_data[0];
	char prev_column = retreat_data[1];


	int prev_char = ungetc(c, cF);

	if(c == '\n'){

		(*line)--;

	}

	*column = prev_column;

	return prev_char;

}


token_t create_constant_token(FILE *cF, char c, int* lc){

	char constant[MAX_TOKEN_LENGTH] = "";
	int prev_column;

	do{

		prev_column = lc[1];

		char addition_str[2] = {c, '\0'};
		strcat(constant, addition_str);

		c = advance_char(cF, lc);

	}
	while(c >= '0' && c <= '9');


	int retreat_data[2] = {c, prev_column};
	c = retreat_char(cF, lc, retreat_data);


	int line = lc[0];
	int column = lc[1];

	char* string = strdup(constant);

	return (token_t){TOK_INT_LITERAL, string, line, column};



}





token_t create_keyword_or_identifier_token(FILE *cF, char c, int* lc){

	char word[MAX_TOKEN_LENGTH] = "";

	int prev_column;

	do{

		prev_column = lc[1];

		char addition_str[2] = {c, '\0'};
		strcat(word, addition_str);

		c = advance_char(cF, lc);

	}
	while(isalpha(c) || c == '_');

	int retreat_data[2] = {c, prev_column};
	c = retreat_char(cF, lc, retreat_data);


	int line = lc[0];
	int column = lc[1];

	char* string = strdup(word);


	if(strcmp(string, "int") == 0){

		return (token_t){TOK_INT_TYPE, string, line, column};


	}
	else if(strcmp(string, "return") == 0){


		return (token_t){TOK_RETURN, string, line, column};


	}
	else{


		return (token_t){TOK_IDENTIFIER, string, line, column};

	}


}
