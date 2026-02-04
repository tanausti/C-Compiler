#ifndef IR_C
#define IR_C




void ast_to_llvm(ast_node_t ast, LLVMContextRef context, LLVMModuleRef module, LLVMBuilderRef builder, LLVMTypeRef int_type);

void function_to_llvm(function_t function, LLVMContextRef context, LLVMModuleRef module, LLVMBuilderRef builder, LLVMTypeRef int_type);

void statement_to_llvm(statement_t statement, LLVMModuleRef module, LLVMBuilderRef builder, LLVMTypeRef int_type, LLVMBasicBlockRef entry);

void return_statement_to_llvm(statement_t statement, LLVMModuleRef module, LLVMBuilderRef builder, LLVMTypeRef int_type, LLVMBasicBlockRef entry);

LLVMValueRef binary_expression_to_llvm(binary_expression_t binary_expression, LLVMModuleRef module, LLVMBuilderRef builder, LLVMTypeRef int_type, LLVMBasicBlockRef entry);





#endif
