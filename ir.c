#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include "parser.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "lexer.h"


//all expressions have values and are assigned to registers


//TODO
//add check for result type for binary expression
LLVMValueRef expr_to_llvm(binary_expression_t binary_expression, LLVMModuleRef module, LLVMBuilderRef builder, LLVMTypeRef int_type, LLVMBasicBlockRef entry){

	LLVMValueRef lhs;

	if(binary_expression.left->type != AST_PRIMARY && binary_expression.left->type == AST_BINARY_EXPRESSION){
		
		lhs = expr_to_llvm(binary_expression.left->as.binary_expression, module, builder, int_type, entry);

	}
	else{

		primary_t primary = binary_expression.left->as.primary;


		if(primary.type == PRIMARY_LITERAL && primary.as.literal.primitive_type == PRIMITIVE_INT){
			lhs = LLVMConstInt(int_type, primary.as.literal.as.integer, 0);
		}
		if(primary.type == PRIMARY_FUNC_CALL){
			lhs = LLVMGetNamedFunction(module, "func");
		}

	}

	LLVMValueRef rhs = LLVMConstInt(int_type, binary_expression.right->as.primary.as.literal.as.integer, 0);

	LLVMPositionBuilderAtEnd(builder, entry);
	LLVMValueRef sum = LLVMBuildAdd(builder, lhs, rhs, "sum");
	
	return sum;


}
int main(void){


	LLVMContextRef context = LLVMContextCreate();

	LLVMBuilderRef builder = LLVMCreateBuilderInContext(context);
	LLVMModuleRef module = LLVMModuleCreateWithNameInContext("expression", context);

	LLVMTypeRef func_type = LLVMFunctionType(LLVMVoidTypeInContext(context), NULL, 0, 0);

	LLVMValueRef func = LLVMAddFunction(module, "dummy", func_type);

	LLVMBasicBlockRef entry = LLVMAppendBasicBlockInContext(context, func, "entry");

	LLVMTypeRef int_type = LLVMInt32TypeInContext(context);

	ast_node_t two_literal_node;


	two_literal_node.type = AST_PRIMARY;
	two_literal_node.as.primary.type = PRIMARY_LITERAL;
	two_literal_node.as.primary.as.literal.primitive_type = PRIMITIVE_INT;
	two_literal_node.as.primary.as.literal.as.integer = 2;	

	ast_node_t two_literal_node_clone = two_literal_node;

	binary_expression_t expr = (binary_expression_t){&two_literal_node, '+', &two_literal_node_clone};

	LLVMValueRef addition_register = expr_to_llvm(expr, module, builder, int_type, entry);

	LLVMBuildRet(builder, addition_register);

	LLVMDumpModule(module);



}




