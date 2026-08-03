# Kiln
Kiln is a compiler for a subset of the C programming language, implemented in C, that generates LLVM IR. It includes the core stages of a modern compiler, including lexical analysis, recursive-descent parsing, abstract syntax tree (AST) construction, and LLVM IR generation. The generated LLVM IR can be compiled into native executables using the LLVM toolchain.

This project was developed to explore systems programming and language design while creating a modular architecture that can later be extended to support additional features of the C language.


---
## Requirements

Kiln requires the following tools:

* C compiler (GCC or Clang)
* Make
* LLVM development libraries

### Installing Dependencies

On Ubuntu/Debian:

```bash
sudo apt install build-essential llvm clang libllvm-dev
```

On Arch Linux:
```bash
sudo pacman -S base-devel llvm clang
```
On macOS:
```bash
brew install llvm
```
## Features

* Compiles a subset of the C programming language
* Lexical analysis (tokenization)
* Recursive descent parser
* Abstract syntax tree (AST) generation
* LLVM IR code generation
* Lex and parse error reporting
* Custom regression test harness

---

## Supported Language Features

Kiln currently supports the following language features:

* 32 bit int literals
* Variable declarations and assignments
* Arithmetic expressions
* `if` statements
* Function definitions
* Function calls
* `return` statements


## Compiler Pipeline

Kiln follows a traditional compiler pipeline:

```text
C Source
    │
    ▼
Lexical Analysis
    │
    ▼
Recursive Descent Parser
    │
    ▼
Abstract Syntax Tree (AST)
    │
    ▼
LLVM IR Generation
```

The lexer converts source code into a stream of tokens. The parser consumes these tokens to construct an abstract syntax tree representing the program structure. The backend traverses the AST and emits LLVM Intermediate Representation (IR), which can then be compiled into a native executable using LLVM tools such as `clang`.

---

---

## Installation

Clone the repository:

```bash
git clone https://github.com/tanausti/Kiln.git
cd kiln
```

Build the compiler:

```bash
make
```

The compiler executable will be generated in the `bin/` directory.

---

## Usage

Compile a C source file into LLVM IR:

```bash
./bin/kiln input.c output.ll
```

Compile the generated LLVM IR into a native executable:

```bash
clang output.ll -o output
```

Run the executable:

```bash
./output
```

---

## Example

Input (`add.c`):

```c
int add() {

    int x = 1 + 1;
    return 1 + x;
}

int main() {

    if(add() + 3){
        return 0;
    }

    return 1;
}
```

Output (`add.ll`):
```c
; ModuleID = 'add.ll'
source_filename = "add.ll"


define i32 @add() {
entry:
  %x = alloca i32, align 4
  %load_var = load i32, ptr %x, align 4
  store i32 2, ptr %x, align 4
  %load_var1 = load i32, ptr %x, align 4
  %sum = add i32 1, %load_var1
  ret i32 %sum
}

define i32 @main() {
entry:
  %calltmp = call i32 @add()
  %sum = add i32 %calltmp, 3
  %boolean = icmp ne i32 %sum, i32 0
  br i1 %boolean, label %then, label %end

then:                                             ; preds = %entry
  ret i32 0

end:                                              ; preds = %entry
  ret i32 1
}
```

Generate LLVM IR:

```bash
./bin/kiln add.c add.ll
```

Compile and run:

```bash
clang add.ll -o add
./add
```

---

## Error Reporting

Kiln reports lex and parse errors with source locations.

Example:

```text
Syntax error (7, 9): expected ';' at end of statement.
```

---

## Testing
Kiln uses a custom regression test harness to verify compiler functionality.

Tests are organized by compiler component and compare expected output files against compiler-generated output, making it straightforward to verify existing behavior as new language features are added. Each test suite processes a collection of input files, generates the corresponding output files, and then invokes the test harness to compare the generated results against the expected outputs.



The test infrastructure includes:

* Shared testing harness
* Individual test suites
* A set of inputs and expected outputs for each test suite
* Independent Makefiles for each suite

Compile a test suite for a compiler component:

```bash
cd test/parser
make
```

Run tests:

```bash
./test 5
```

The tests in The test harness executes every test from `1` through the specified test number and compares the generated output against the expected output.

---

## License

This project is licensed under the MIT License.
