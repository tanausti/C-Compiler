# Kiln

Kiln is a compiler for a subset of the C programming language, implemented in C, that generates LLVM IR. It includes the core stages of a modern compiler, including lexical analysis, recursive-descent parsing, abstract syntax tree (AST) construction, and LLVM IR generation. The generated LLVM IR can be compiled into native executables using the LLVM toolchain.

This project was developed to explore systems programming and language design while creating a modular compiler architecture that can later be extended with additional C language features.

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

---

## Features

* Compiles a subset of the C programming language
* Lexical analysis (tokenization)
* Recursive-descent parser
* Abstract syntax tree (AST) generation
* LLVM IR code generation
* Lex and parse error reporting
* Custom regression test harness

---

## Supported Language Features

Kiln currently supports:

* 32-bit integer literals
* Variable declarations and assignments
* Arithmetic expressions
* `if` statements
* Function definitions
* Function calls
* `return` statements

---

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

The lexer converts source code into tokens. The parser consumes these tokens and constructs an abstract syntax tree representing the program structure. The backend traverses the AST and generates LLVM Intermediate Representation (IR), which can then be compiled into a native executable using LLVM tools such as `clang`.

---

## Installation

Clone the repository:

```bash
git clone https://github.com/tanausti/Kiln.git
cd Kiln
```

Build the compiler:

```bash
make
```

The compiler executable will be generated in:

```text
bin/kiln
```

---

## Build Options

Kiln supports both GCC and Clang builds.

Build with GCC:

```bash
make
```

Build with Clang:

```bash
make clang
```

Build a debug version:

```bash
make debug
```

Build a debug version with Clang:

```bash
make clang-debug
```

Clean build files:

```bash
make clean
```

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

Generate LLVM IR:

```bash
./bin/kiln add.c add.ll
```

Example output (`add.ll`):

```llvm
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

Compile and run:

```bash
clang add.ll -o add
./add
```

---

## Error Reporting

Kiln reports lex and pars errors with source locations.

Example:



```text
Syntax error (2, 12): expected ';' at end of statement.
```

---

## Testing

Kiln uses a custom regression test harness to verify functionality.

Tests are organized by compiler component. Each test suite uses:

* Shared testing harness
* Individual component test suites
* Input programs (`input#.c`)
* Expected output files (`expected_out#.<ext>`)
* Generated output files (`actual_out#.<ext>`)

The test harness compares each generated `actual_out#.<ext>` file against its corresponding `expected_out#.<ext>` file.

To add a new test, create a numbered input file (`input#.c`) and a matching expected output file (`expected_out#.<ext>`). Run the test suite with the highest test number, and the harness will automatically execute all tests from `1` through that number and compare the generated outputs against the expected outputs.

Compile all test suites:

```bash
cd test
make

```

Compile an individual test suite:

```bash
cd test
make ir

```
Run test executables from the project root so relative paths resolve correctly.

Run a test suite:
```bash
./build/ir_test/ir_test 8
```


---

## License

This project is licensed under the MIT License.
