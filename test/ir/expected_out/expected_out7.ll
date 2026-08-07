; ModuleID = 'test/ir/actual_out/actual_out7.ll'
source_filename = "test/ir/actual_out/actual_out7.ll"

define i32 @func() {
entry:
  br i1 false, label %then, label %end

then:                                             ; preds = %entry
  ret i32 77

end:                                              ; preds = %entry
  ret i32 4
}

define i32 @main() {
entry:
  %x = alloca i32, align 4
  br i1 true, label %then, label %end

then:                                             ; preds = %entry
  %load_var = load i32, ptr %x, align 4
  %calltmp = call i32 @func()
  store i32 %calltmp, ptr %x, align 4
  br label %end

end:                                              ; preds = %then, %entry
  br i1 false, label %then1, label %end2

then1:                                            ; preds = %end
  ret i32 1

end2:                                             ; preds = %end
  %load_var3 = load i32, ptr %x, align 4
  ret i32 %load_var3
}
