#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "lexer.h"

#define MAX_TOKEN_LENGTH 10


Token nextToken(FILE *cF, int* lc){

	int c = advanceChar(cF, lc);

	int line = lc[0];
	int column = lc[1];



	while(isspace(c)){

		c = advanceChar(cF, lc);

	}




	switch(c){

		case EOF:{
			char* string = strdup("EOF");
			return (Token){TOK_EOF, string, line, column};
			break;}

		case '+':{
			char* string = strdup("+");
			return (Token){TOK_PLUS, string, line, column};
			break;}

		case '(':{
			char* string = strdup("(");
			return (Token){TOK_LPARENTH, string, line, column};
			break;}

		case ')':{
			char* string = strdup(")");
			return (Token){TOK_RPARENTH, string, line, column};
			break;}

		case '{':{
			char* string = strdup("{");
			return (Token){TOK_LBRACE, string, line, column};
			break;}

		case '}':{
			char* string = strdup("}");
			return (Token){TOK_RBRACE, string, line, column};
			break;}

		case ';':{
			char* string = strdup(";");
			return (Token){TOK_SEMI, string, line, column};
			break;}
		default:
			break;

	}

	if(c >= '0' && c <= '9'){

		return createConstantToken(cF, c, lc);

	}

	if(isalpha(c) || c == '_'){

		return createKeywordOrIdentifierToken(cF, c, lc);

	}

	return (Token){TOK_UNKNOWN, "UNKNOWN", line, column};


}



int advanceChar(FILE *cF, int* lc){

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


int retreatChar(FILE *cF, int* lc, int* retreatData){

	int* line = lc
		int* column = lc + 1;

	char c = retreatData[0];
	char prevColumn = retreatData[1];


	int prevChar = ungetc(c, cF);

	if(c == '\n'){

		(*line)--;

	}

	*column = prevColumn;

	return prevChar;

}


Token createConstantToken(FILE *cF, char c, int* lc){

	char constant[MAX_TOKEN_LENGTH] = "";
	int prevColumn;

	do{

		prevColumn = lc[1];

		char additionStr[2]; = {c, '\0'};
		strcat(constant, additionStr);

		c = advanceChar(cF, lc);

	}
	while(c >= '0' && c <= '9');


	int retreatData[2] = {c, prevColumn};
	c = retreatChar(cF, lc, retreatData);


	int line = lc[0];
	int column = lc[1];

	char* stringval = strdup(constant);

	return (Token){TOK_INT_LITERAL, stringval, line, column};



}





Token createKeywordOrIdentifierToken(FILE *cF, char c, int* lc){

	char word[MAX_TOKEN_LENGTH] = "";

	int prevColumn;

	do{

		prevColumn = lc[1];

		char additionStr[2] = {c, '\0'};
		strcat(word, additionStr);

		c = advanceChar(cF, lc);

	}
	while(isalpha(c) || c == '_');

	int retreatData[2] = {c, prevColumn};
	c = retreatChar(cF, lc, retreatData);


	int line = lc[0];
	int column = lc[1];

	char* stringVal = strdup(word);


	if(strcmp(stringVal, "int") == 0){

		return (Token){TOK_INT_TYPE, stringVal, line, column};


	}
	else if(strcmp(stringVal, "return") == 0){


		return (Token){TOK_RETURN, stringVal, line, column};
		

	}
	else{


		return (Token){TOK_IDENTIFIER, stringVal, line, column};

	}


}
