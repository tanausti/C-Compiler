#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <ctype.h>
#include "lexer.h"


typedef struct ASTNode ASTNode;

typedef struct TokenStackNode{

	struct TokenStackNode* prev;
	struct TokenStackNode* next;
	Token* token;

}TokenStackNode;



typedef struct{

	struct ASTNode* left;
	char operator;
	struct ASTNode* right;

}BinaryAST;

typedef struct{

	ASTNode** children;
	int size;
	int capacity;

}VecAST;

enum ChildAST{

	VEC,
	BIN,
	LIT,
	KEY

};


typedef struct{



	union{


		int val;

	}literalVal;


}Literal;




typedef struct{






}Identifier;




typedef struct{



}FuncCall;







enum PrimaryType{


	LITERAL,
	IDENTIFIER,
	FUNC_CALL

};




typedef struct Primary{

	enum PrimaryType primaryType;

	union{

		Literal literal;
		Identifier identifier;
		FuncCall funcCall;
	}primary;


}Primary;

enum Keyword{

	KEYW_RETURN,
	KEYW_INT
};


typedef struct ASTNode{


	enum ChildAST childAST;

	union{

		enum Keyword keyword;

		Primary primary;

		VecAST vecAST;

		BinaryAST binaryAST;



	}value;

}ASTNode;












TokenStackNode* tokenStack(FILE* cF, int* lc);

void printAST(ASTNode ast);
void printLit(Primary primary);
void printTerm(ASTNode** ast);

ASTNode ast_node_vec_children();

ASTNode ast_node_bin_children();

ASTNode ast_node_lit_child();

void add_right_child_vec(ASTNode* parent, ASTNode child);

ASTNode application(TokenStackNode** curr);
ASTNode functionList(TokenStackNode** curr);
ASTNode function(TokenStackNode** curr);
ASTNode statementList(TokenStackNode** curr);
ASTNode statement(TokenStackNode** curr);
ASTNode expression(TokenStackNode** curr);
ASTNode equality(TokenStackNode** curr);
ASTNode comparison(TokenStackNode** curr);
ASTNode term(TokenStackNode** curr);
ASTNode factor(TokenStackNode** curr);
ASTNode unary(TokenStackNode** curr);
ASTNode primary(TokenStackNode** curr);

int strToInt(char* str);

bool match(TokenStackNode** curr, int n, ...);

bool check(TokenStackNode* curr, TokenType type);

void advance(TokenStackNode** curr);

TokenStackNode* peek(TokenStackNode* curr);

#endif


