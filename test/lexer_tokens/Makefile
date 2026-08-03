CC ?= gcc

CFLAGS = -Wall -Wextra
DEBUG_FLAGS = -g

SOURCES = \
    lexer_tokens_test.c \
    ../test_harness.c \
    ../../src/lexer/lexer.c

OBJECTS = $(SOURCES:.c=.o)

TARGET = lexer_tokens_test

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