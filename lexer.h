#ifndef LEXER_H
#define LEXER_H


typedef enum token_type{


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

}token_type_t;


typedef struct token{


	token_type_t type;
	char* string;
	int line;
	int column;

}token_t;



token_t next_token(FILE* cF, int* lc);
int advance_char(FILE *cF, int* lc);
int retreat_char(FILE *cF, int* lc, int* retreat_data);
token_t create_constant_token(FILE* cF, char c, int* lc);
token_t create_keyword_or_identifier_token(FILE* cF, char c, int* lc);



#endif
