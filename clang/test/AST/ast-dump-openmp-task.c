// RUN: %clang_cc1 -triple x86_64-unknown-unknown -fopenmp -ast-dump %s | FileCheck --match-full-lines -implicit-check-not=openmp_structured_block %s

void test() {
#pragma omp task
  ;
}

// CHECK: TranslationUnitDecl {{.*}} <<invalid sloc>> <invalid sloc>
// CHECK: `-FunctionDecl {{.*}} <{{.*}}ast-dump-openmp-task.c:3:1, line:6:1> line:3:6 test 'void ()'
// CHECK-NEXT:   `-CompoundStmt {{.*}} <col:13, line:6:1>
// CHECK-NEXT:     `-OMPTaskDirective {{.*}} <line:4:9, col:17>
// CHECK-NEXT:       `-CapturedStmt {{.*}} <line:5:3>
// CHECK-NEXT:         `-CapturedDecl {{.*}} <<invalid sloc>> <invalid sloc> nothrow
// CHECK-NEXT:           |-NullStmt {{.*}} <col:3> openmp_structured_block
// CHECK-NEXT:           |-AlwaysInlineAttr {{.*}} <<invalid sloc>> Implicit __forceinline
// CHECK-NEXT:           |-ImplicitParamDecl {{.*}} <line:4:9> col:9 implicit .global_tid. 'const int'
// CHECK-NEXT:           |-ImplicitParamDecl {{.*}} <col:9> col:9 implicit .part_id. 'const int *const restrict'
// CHECK-NEXT:           |-ImplicitParamDecl {{.*}} <col:9> col:9 implicit .privates. 'void *const restrict'
// CHECK-NEXT:           |-ImplicitParamDecl {{.*}} <col:9> col:9 implicit .copy_fn. 'void (*const restrict)(void *const restrict, ...)'
// CHECK-NEXT:           |-ImplicitParamDecl {{.*}} <col:9> col:9 implicit .task_t. 'void *const'
// CHECK-NEXT:           `-ImplicitParamDecl {{.*}} <col:9> col:9 implicit __context 'struct (anonymous at {{.*}}ast-dump-openmp-task.c:4:9) *const restrict'
