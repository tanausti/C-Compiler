CC ?= gcc

CFLAGS = -Wall -Wextra
DEBUG_FLAGS = -g

LLVM_FLAGS = $(shell llvm-config --cflags --ldflags --libs core)

SOURCES = \
    ir_test.c \
    ../test_harness.c \
    ../../src/ir/ir.c \
    ../../src/parser/parser.c \
    ../../src/parser/token_stack.c \
    ../../src/parser/free_ast.c \
    ../../src/lexer/lexer.c

OBJECTS = $(SOURCES:.c=.o)

TARGET = ir_test

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LLVM_FLAGS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(LLVM_FLAGS) -c $< -o $@

debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean $(TARGET)

clang:
	$(MAKE) CC=clang

clang-debug:
	$(MAKE) CC=clang debug

clean:
	rm -f $(OBJECTS) $(TARGET)