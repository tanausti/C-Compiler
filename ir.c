#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "ir.h"



int main(int argc, char* argv[]){

	FILE *cF = fopen(argv[1], "r");

	int lc[2] = {1, 1};

	token_stack_node_t* head = token_stack(cF, lc);

	ast_node_t ast = application(&head);

	

	LLVMContextRef context = LLVMContextCreate();
	LLVMBuilderRef builder = LLVMCreateBuilderInContext(context);
	LLVMModuleRef module = LLVMModuleCreateWithNameInContext("expression", context);


	LLVMTypeRef int_type = LLVMInt32TypeInContext(context);

	ast_to_llvm(ast, context, module, builder, int_type);

	return 0;



}


void ast_to_llvm(ast_node_t ast, LLVMContextRef context, LLVMModuleRef module, LLVMBuilderRef builder, LLVMTypeRef int_type){


	const vector_tree_t vector_tree = ast.as.function_list.vector_tree;
	const int size = vector_tree.size;
	for(int i = 0; i < size; i++){

	LLVMVerifyModule(module, LLVMAbortProcessAction, NULL);
		function_to_llvm(vector_tree.children[i]->as.function, context, module, builder, int_type);
	}


}




void function_to_llvm(function_t function, LLVMContextRef context, LLVMModuleRef module, LLVMBuilderRef builder, LLVMTypeRef int_type){


	const function_prototype_t function_prototype = function.function_prototype;

	LLVMTypeRef func_type;

	switch(function_prototype.primitive_type){
		
		case PRIMITIVE_INT:
			func_type = LLVMFunctionType(int_type, NULL, 0, 0);
			break;
		default:
			func_type = LLVMFunctionType(int_type, NULL, 0, 0);
			break;

	}

	const char* function_name = function_prototype.function_name;
	
	LLVMValueRef func = LLVMAddFunction(module, function_name, func_type);


	LLVMBasicBlockRef entry = LLVMAppendBasicBlockInContext(context, func, "entry");
	LLVMPositionBuilderAtEnd(builder, entry);
	const vector_tree_t vector_tree = function.statement_list.vector_tree;
	const int size = vector_tree.size;

	for(int i = 0; i < size; i++){

		statement_to_llvm(vector_tree.children[i]->as.statement, module, builder, int_type, entry);

	}
		



}



//TODO add case of void function
void statement_to_llvm(statement_t statement, LLVMModuleRef module, LLVMBuilderRef builder, LLVMTypeRef int_type, LLVMBasicBlockRef entry){

/*
	const vector_tree_t vector_tree = statement.vector_tree;

	if(vector_tree.children[0]->type == AST_KEYWORD){

		keyword_t keyword = vector_tree.children[0]->as.keyword;

		switch(keyword){
			case KEYW_RETURN:
				return_statement_to_llvm(statement, module, builder, int_type, entry);
				break;
		}



	}
	else{
*/

				return_statement_to_llvm(statement, module, builder, int_type, entry);
		//binary_expression_to_llvm(statement.vector_tree.children[1]->as.binary_expression, module, builder, int_type, entry);


	//}




}



void return_statement_to_llvm(statement_t statement, LLVMModuleRef module, LLVMBuilderRef builder, LLVMTypeRef int_type, LLVMBasicBlockRef entry){



	binary_expression_t binary_expression = statement.vector_tree.children[1]->as.binary_expression;
	
	LLVMValueRef sum = binary_expression_to_llvm(binary_expression, module, builder, int_type, entry);


	LLVMBuildRet(builder, sum);
	
	char* ir = LLVMPrintModuleToString(module);

	printf("%s\n", ir);





}


//TODO
//add check for result type for binary expression
LLVMValueRef binary_expression_to_llvm(binary_expression_t binary_expression, LLVMModuleRef module, LLVMBuilderRef builder, LLVMTypeRef int_type, LLVMBasicBlockRef entry){


	LLVMValueRef lhs = LLVMConstInt(int_type, binary_expression.left->as.primary.as.literal.as.integer, 0);	


	LLVMValueRef rhs = LLVMConstInt(int_type, binary_expression.right->as.primary.as.literal.as.integer, 0);	

	LLVMValueRef sum = LLVMBuildAdd(builder, lhs, rhs, "sum");

	return sum;


}
