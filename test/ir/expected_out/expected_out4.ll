; ModuleID = 'test/ir/actual_out/actual_out4.ll'
source_filename = "test/ir/actual_out/actual_out4.ll"

define i32 @add() {
entry:
  ret i32 4
}

define i32 @main() {
entry:
  %calltmp = call i32 @add()
  ret i32 %calltmp
}
