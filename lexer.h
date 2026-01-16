#ifndef LEXER_H
#define LEXER_H


typedef enum{


	TOK_EOF,

	TOK_INT_TYPE,
	TOK_RETURN,

	TOK_INT_LITERAL,

	TOK_PLUS,

	TOK_IDENTIFIER,

	TOK_LPARENTH,
	TOK_RPARENTH,
	TOK_LBRACE,
	TOK_RBRACE,
	TOK_SEMI,

	TOK_UNKNOWN

}TokenType;


typedef struct{


	TokenType type;
	char* stringVal;
	int line;
	int column;

}Token;



Token nextToken(FILE* cFm int* lc);
void skipWhiteSpace(FILE *cF, int* c, int* lc);
int advanceChar(FILE *cF, int* lc);
Token createConstantToken(FILE* cF, char c, int* lc);
Token createKeywordOrIdentifierToken(FILE* cF, char c, int* lc);



#endif
