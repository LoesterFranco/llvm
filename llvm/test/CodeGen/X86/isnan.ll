; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -mtriple=i686-- | FileCheck %s

declare i1 @llvm.isunordered.f64(double)

define i1 @test_isnan(double %X) {
; CHECK-LABEL: test_isnan:
; CHECK:       # %bb.0:
; CHECK-NEXT:    fldl {{[0-9]+}}(%esp)
; CHECK-NEXT:    fucomp %st(0)
; CHECK-NEXT:    fnstsw %ax
; CHECK-NEXT:    # kill: def $ah killed $ah killed $ax
; CHECK-NEXT:    sahf
; CHECK-NEXT:    setp %al
; CHECK-NEXT:    retl
        %R = fcmp uno double %X, %X             ; <i1> [#uses=1]
        ret i1 %R
}

