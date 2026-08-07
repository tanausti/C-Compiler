; ModuleID = 'test/ir/actual_out/actual_out8.ll'
source_filename = "test/ir/actual_out/actual_out8.ll"

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
