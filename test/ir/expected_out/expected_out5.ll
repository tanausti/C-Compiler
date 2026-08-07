; ModuleID = 'test/ir/actual_out/actual_out5.ll'
source_filename = "test/ir/actual_out/actual_out5.ll"

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %load_var = load i32, ptr %x, align 4
  store i32 2, ptr %x, align 4
  ret i32 0
}
