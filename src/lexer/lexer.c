#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "lexer.h"

#define MAX_CONSTANT_LENGTH 31
#define MAX_IDENTIFIER_LENGTH 127




token_t next_token(FILE *cF, pos_t* lc){

	int c = -1;

	do{

		c = advance_char(cF, lc);

	}
	while(isspace(c));



	int line = lc->line;
	int column = lc->column;


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

		case '=':{

				char* string = strdup("=");
				return (token_t){TOK_EQ, string, line, column};
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



	char* err_string = malloc(2);
	err_string[0] = c;
	err_string[1] = '\0';
	
	fprintf(stderr, "Error (%d, %d): Invalid token: %s\n", line, column, err_string);
	return (token_t){TOK_ERROR, err_string, line, column};


}






int advance_char(FILE *cF, pos_t* lc){


	int c = fgetc(cF);

	if(c == '\n' && look_ahead(cF) != EOF){

		lc->line++;
		lc->column = 1;


	}
	else if(c == '\t'){

		lc->column += 8;
	
	}
	else{

		lc->column++;
	}

	return c;

}






token_t create_constant_token(FILE *cF, char c, pos_t* lc){

	char constant[MAX_CONSTANT_LENGTH + 1] = "";
	int start_column = lc->column;

	bool exceeded = false;
	int constant_length = 0;

	//read until end of constant string, append to constant
	while(look_ahead(cF) >= '0' && look_ahead(cF) <= '9'){
	
		char addition_str[2] = {c, '\0'};

		if(constant_length < MAX_CONSTANT_LENGTH){

			strcat(constant, addition_str);

			c = advance_char(cF, lc);
			constant_length++;

		}
		else{
			exceeded = true;
			advance_char(cF, lc);
		}




	}


	char addition_str[2] = {c, '\0'};
	strcat(constant, addition_str);

	if(exceeded){

		fprintf(stderr, "Error (%d, %d): Constant exceeded maximum length. \n", lc->line, start_column);
		return (token_t){TOK_ERROR, strdup(constant), lc->line, start_column};
	}



	int line = lc->line;

	char* string = strdup(constant);


	return (token_t){TOK_INT_LITERAL, string, line, start_column};



}





token_t create_keyword_or_identifier_token(FILE *cF, char c, pos_t* lc){

	char word[MAX_IDENTIFIER_LENGTH + 1] = "";

	int start_column = lc->column;

	bool exceeded = false;
	int length = 0;

	//read until end of word, append to word
	while(isalpha(look_ahead(cF)) || look_ahead(cF) == '_' || isdigit(look_ahead(cF))){

		char addition_str[2] = {c, '\0'};

		if(length < MAX_IDENTIFIER_LENGTH){

			strcat(word, addition_str);

			c = advance_char(cF, lc);
			length++;

		}
		else{
			exceeded = true;
			advance_char(cF, lc);
		}


	}


	char addition_str[2] = {c, '\0'};
	strcat(word, addition_str);

	if(exceeded){

		fprintf(stderr, "Error (%d, %d): Identifier exceeded maximum length. \n", lc->line, start_column);
		return (token_t){TOK_ERROR, strdup(word), lc->line, start_column};

	}



	char* string = strdup(word);

	if(strcmp(word, "if") == 0){
		
		return (token_t){TOK_IF, string, lc->line, start_column};

	}
	else if(strcmp(string, "int") == 0){
	
		return (token_t){TOK_INT_TYPE, string, lc->line, start_column};

	}
	else if(strcmp(string, "return") == 0){

		return (token_t){TOK_RETURN, string, lc->line, start_column};
	}
	else{

		return (token_t){TOK_IDENTIFIER, string, lc->line, start_column};
	}


}




int look_ahead(FILE* cF){

	int c = fgetc(cF);

	ungetc(c, cF);

	return c;

}

