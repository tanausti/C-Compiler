#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "lexer.h"
#include "parser.h"

#define DEFAULT_NODE_CAPACITY 20


int main(int argc, char* argv[]){

	FILE *cF = fopen(argv[1], "r");

	int lc[2] = {1, 1};

	TokenStackNode* head = tokenStack(cF, lc);

	ASTNode ast = application(&head);

	printAST(ast);

	return 0;



}


void printAST(ASTNode ast){


	switch(ast.childAST){
		case LIT:{
				 printLit(ast.value.primary);}
			 break;
		case KEY:{
				 printf("KEYWORD\n");}
			 break;
		case BIN:{
				 printAST(*ast.value.binaryAST.left);
				 printf("%c\n", ast.value.binaryAST.operator);
				 printAST(*ast.value.binaryAST.right);}
			 break;
		case VEC:{
				 const int size = ast.value.vecAST.vecAST.size;

				 for(int i = 0; i < size; i++){

					 printAST(**(ast.value.vecAST.children + i * sizeof(ASTNode*)));

				 }}
			 break;

	}

}


void printLit(Primary primary){


	switch(primary.primaryType){
		case LITERAL:{
				     const int x = primary.primary.literal.literalVal.val;
				     printf("%d\n", x);}
			     break;
		case IDENTIFIER:{
					printf("IDENTIFIER\n");}
				break;
		case FUNC_CALL:{
				       printf("FUNC_CALL\n");}
			       break;

	}


}



ASTNode ast_node_vec_children(){


	ASTNode astNode;
	astNode.childAST = VEC;
	astNode.value.vecAST.size = 0;
	astNode.value.vecASt.capacity = DEFAULT_NODE_CAPACITY;
	astNode.value.vecAST.children = malloc(sizeof(ASTNode*));
	return astNode;


}


ASTNode ast_node_bin_children(){

	ASTNode astNode;
	astNode.childAST = BIN;
	astNode.value.binaryAST = (BinaryAST){NULL, '\0', NULL};
	return astNode;


}


ASTNode ast_node_lit_children(){

	ASTNode astNode;
	astNode.childAST = LIT;
	astNode.value.primary.primary.literal.literalVal.val = -1;
	return astNode;
}



void add_right_child_vec(ASTNode* parent, ASTNode child){

	if(parent->childAST == VEC){

		ASTNode** vec = parent->value.vecAST.children;
		ASTNode* childPtr = malloc(sizeof(child));
		*childPtr = child;

		if(vec != NULL && parent->value.vecAST.size <= parent->value.vecAST.capacity){


			vec = realloc(vec, ++(parent->value.vecAST.size) * sizeof(ASTNode*));

			const int size = parent->value.vecAST.size;

			memcpy(vec + (size - 1) * sizeof(ASTNode*), &childPtr, sizeof(ASTNode*));


		}else{

			puts("not initialized properly");

		}
	}

}


TokenStackNode* tokenStack(File* cF, int* lc){


	TokenStackNode* curr = (TokenStackNode*)malloc(sizeof(TokenStackNode));

	curr->token = (Token*)malloc(sizeof(token));
	*(curr->token) = nextToken(cF, lc);
	curr->prev = NULL;

	TokenStackNode* head = curr;

	while(curr->token->type != TOK_EOF){

		TokenStackNode* tempPrev = curr;

		curr->next = (TokenStackNode*)malloc(sizeof(TokenStackNode));

		curr = curr->next;
		curr->token = (Token*)malloc(sizeof(Token));
		*(curr->token) = nextToken(cF, lc);

		curr->prev = tempPrev;


	}


	return head;




}



ASTNode application(TokenStackNode** curr){

	return functionList(curr);


}




ASTNode functionList(TokenStackNode** curr){

	ASTNode functionList = ast_node_vec_children();

	while(match(curr, 1, TOK_INT_TYPE) && match(curr, 1, TOK_IDENTIFIER) && match(curr, 1, TOK_LPARENTH) && match(curr, 1, TOK_RPARENTH) && match(curr, 1, TOK_LBRACE)){

		printf("current token: %s\n", (**curr).token->stringVal);
		ASTNode func = function(curr);
		add_right_child_vec(&functionList, func);

	}

	return functionList;

}


ASTNode function(TokenStackNode** curr){

	return statementList(curr);


}



ASTNode statementList(TokenStackNode** curr){

	ASTNode statementList = ast_node_vec_children();
	ASTNode stement;

	if(!match(curr, 1, TOK_RBRACE)){


		stement = statement(curr);
		add_right_child_vec(&statementList, stement);

		while(match(curr, 1, TOK_SEMI) && !match(curr, 1, TOK_RBRACE)){

			stement = statement(curr);

			add_right_child_vec(&statementList, stement);

		}




	}

	return statementList;



}



ASTNode statement(TokenStackNode** curr){

	ASTNode statement = ast_node_vec_children();

	while(match(curr, 2, TOK_RETURN, TOK_INT_TYPE)){

		ASTNode keywordNode;
		keywordNode.childAST = KEY;

		switch(((*curr)->prev->token->type)){

			case TOK_RETURN:
				keywordNode.value.keyword = KEYW_RETURN;
				break;
			case TOK_INT_TYPE:
				keywordNode.value.keyword = KEYW_INT;
				break;

		}

		add_right_child_vec(&statement, keywordNode);



	}

	ASTNode expr = expression(curr);

	add_right_child_vec(&statement, expr);


	return statement;


}




ASTNode expression(TokenStackNode** curr){


	return equality(curr);



}


ASTNode equality(TokenStackNode** curr){


	return comparison(curr);



}


ASTNode comparison(TokenStackNode** curr){


	return term(curr);



}


ASTNode term(TokenStackNode** curr){


	ASTNode left = factor(curr);

	left.childAST = LIT;

	while(match(curr, 1, TOK_PLUS)){

		left.childAST = LIT;
		char operator = ((*curr)->prev->token->stringVal)[0];

		ASTNode right = factor(curr);
		right.childAST = LIT;

		ASTNode* rightPtr = malloc(sizeof(right));
		ASTNode* leftPtr = malloc(sizeof(left));
		*leftPtr = left;
		*rightPtr = right;

		ASTNode newLeft;
		newLeft.value.binaryAST = (BinaryAST){leftPtr, operator, rightPtr};
		newLeft.childAST = BIN;
		left = newLeft;


	}

	return left;



}





ASTNode factor(TokenStackNode** curr){

	return unary(curr);


}


ASTNode unary(TokenStackNode** curr){

	return primary(curr);


}


ASTNode primary(TokenStackNode** curr){

	ASTNode ast;

	if((*curr)->token->type == TOK_IDENTIFIER){

		if((*curr)->token->type == TOK_LPARENTH && peek(peek(*curr))->token->type == TOK_RPARENTH){

			ast.value.primary.primaryType = FUNC_CALL;
			ast.value.primary.primaryType = FUNC_CALL;
			ast.value.primary.primary.funcCall = (FuncCall){};
			advance(curr);
			advance(curr);

		}
		else{

			ast.value.primary.primaryType = IDENTIFIER;
			ast.value.primary.identifier = (Identifier){};

		}

	}
	else{

		ast.value.primary.primaryType = LITERAL;
		char* primaryStr = (*curr)->token->stringVal;
		int primaryVal = strToInt(primaryStr);

		ast.value.primary.primary.literal.literalVal.val = primaryVal;
	}

	advance(curr);

	return ast;

}




int strToInt(char* str){


	char* end;
	errno = 0;
	long v = strtol(str, &end, 10);
	int literalValue = (int)v;

	char firstDigit = str[0];

	if(literalValue == 0 && firstDigit != '0'){

		fprintf(stderr, "unaccounted token: %c\n", firstDigit);

	}

	return literalValue;


}



bool match(TokenStackNode** curr, int n, ...){


	va_list args;
	va_start(args, n);


	for(int i = 0; i < n; i++){


		TokenType tokenType = va_arg(args, TokenType);

		if(check(*curr, tokenType)){

			advance(curr);
			va_end(args);
			return true;



		}





	}



	va_end(args);

	return false;


}




bool check(TokenStackNode* curr, TokenType type){

	Token currToken = *(curr->token);

	return (currToken.type == type);

}


void advance(TokenStackNode** curr){


	*curr = peek(*curr);

}



TokenStackNode* peek(TokenStackNode* curr){

	return curr->next;


}
