LLVM_CONFIG = llvm-config

CFLAGS  = $(shell $(LLVM_CONFIG) --cflags)
LDFLAGS = $(shell $(LLVM_CONFIG) --ldflags --libs core analysis irreader)

TARGET = compiler
SRC = ir.c parser.c lexer.c

$(TARGET): $(SRC)
	clang $(SRC) $(CFLAGS) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)
