CC ?= gcc

CFLAGS = -Wall -Wextra
DEBUG_FLAGS = -g

SOURCES = \
    parser_console_test.c \
    ../test_harness.c \
    ../../src/parser/parser.c \
    ../../src/parser/token_stack.c \
    ../../src/parser/free_ast.c \
    ../../src/parser/print_ast.c \
    ../../src/lexer/lexer.c

OBJECTS = $(SOURCES:.c=.o)

TARGET = parser_console_test

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean $(TARGET)

clang:
	$(MAKE) CC=clang

clang-debug:
	$(MAKE) CC=clang debug

clean:
	rm -f $(OBJECTS) $(TARGET)