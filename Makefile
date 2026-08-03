CC ?= gcc

CFLAGS = -Wall -Wextra
DEBUG_FLAGS = -g

LLVM_FLAGS = $(shell llvm-config --cflags --ldflags --libs core)

SOURCES = \
    src/compiler/compiler.c \
    src/lexer/lexer.c \
    src/parser/parser.c \
    src/parser/free_ast.c \
    src/parser/validate_ast.c \
    src/parser/token_stack.c \
    src/ir/ir.c

OBJECTS = $(SOURCES:.c=.o)

TARGET = bin/compiler

all: $(TARGET)

$(TARGET): $(OBJECTS)
	mkdir -p bin
	$(CC) $(OBJECTS) $(LLVM_FLAGS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(LLVM_FLAGS) -c $< -o $@

debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean $(TARGET)

clang:
	$(MAKE CC=clang)

clang-debug:
	$(MAKE CC=clang debug)

clean:
	rm -f $(OBJECTS) $(TARGET)