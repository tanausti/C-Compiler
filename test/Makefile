CC ?= gcc

CFLAGS = -Wall -Wextra
DEBUG_FLAGS = -g
LLVM_FLAGS = $(shell llvm-config --cflags --ldflags --libs core)

BUILD_DIR = ../build

IR_SOURCES = \
	ir/ir_test.c \
	test_harness.c \
	../src/ir/ir.c \
	../src/parser/parser.c \
	../src/parser/token_stack.c \
	../src/parser/free_ast.c \
	../src/lexer/lexer.c

LEXER_CONSOLE_SOURCES = \
	lexer_console/lexer_console_test.c \
	test_harness.c \
	../src/lexer/lexer.c

LEXER_TOKENS_SOURCES = \
	lexer_tokens/lexer_tokens_test.c \
	test_harness.c \
	../src/lexer/lexer.c

PARSER_AST_SOURCES = \
	parser_ast/parser_ast_test.c \
	test_harness.c \
	../src/parser/parser.c \
	../src/parser/token_stack.c \
	../src/parser/free_ast.c \
	../src/parser/print_ast.c \
	../src/lexer/lexer.c

PARSER_CONSOLE_SOURCES = \
	parser_console/parser_console_test.c \
	test_harness.c \
	../src/parser/parser.c \
	../src/parser/token_stack.c \
	../src/parser/free_ast.c \
	../src/parser/print_ast.c \
	../src/lexer/lexer.c

TESTS = \
	$(BUILD_DIR)/ir_test/ir_test \
	$(BUILD_DIR)/lexer_console_test/lexer_console_test \
	$(BUILD_DIR)/lexer_tokens_test/lexer_tokens_test \
	$(BUILD_DIR)/parser_ast_test/parser_ast_test \
	$(BUILD_DIR)/parser_console_test/parser_console_test

.PHONY: all ir lexer_console lexer_tokens parser_ast parser_console debug clang clang-debug clean

all: $(TESTS)

ir: $(BUILD_DIR)/ir_test/ir_test

lexer_console: $(BUILD_DIR)/lexer_console_test/lexer_console_test

lexer_tokens: $(BUILD_DIR)/lexer_tokens_test/lexer_tokens_test

parser_ast: $(BUILD_DIR)/parser_ast_test/parser_ast_test

parser_console: $(BUILD_DIR)/parser_console_test/parser_console_test

$(BUILD_DIR)/ir/ir_test: $(IR_SOURCES)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(IR_SOURCES) $(LLVM_FLAGS) -o $@

$(BUILD_DIR)/lexer_console/lexer_console_test: $(LEXER_CONSOLE_SOURCES)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LEXER_CONSOLE_SOURCES) -o $@

$(BUILD_DIR)/lexer_tokens/lexer_tokens_test: $(LEXER_TOKENS_SOURCES)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LEXER_TOKENS_SOURCES) -o $@

$(BUILD_DIR)/parser_ast/parser_ast_test: $(PARSER_AST_SOURCES)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(PARSER_AST_SOURCES) -o $@

$(BUILD_DIR)/parser_console/parser_console_test: $(PARSER_CONSOLE_SOURCES)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(PARSER_CONSOLE_SOURCES) -o $@