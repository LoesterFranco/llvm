// REQUIRES: powerpc-registered-target

// RUN: %clang -S -emit-llvm -target powerpc64-gnu-linux -mcpu=pwr8 -DNO_WARN_X86_INTRINSICS %s \
// RUN:   -fno-discard-value-names -mllvm -disable-llvm-optzns -o - | llvm-cxxfilt | FileCheck %s --check-prefixes=CHECK-P8,CHECK,CHECK-BE
// RUN: %clang -S -emit-llvm -target powerpc64le-gnu-linux -mcpu=pwr8 -DNO_WARN_X86_INTRINSICS %s \
// RUN:   -fno-discard-value-names -mllvm -disable-llvm-optzns -o - | llvm-cxxfilt | FileCheck %s --check-prefixes=CHECK-P8,CHECK,CHECK-LE
// RUN: %clang -S -emit-llvm -target powerpc64-gnu-linux -mcpu=pwr9 -DNO_WARN_X86_INTRINSICS %s \
// RUN:   -fno-discard-value-names -mllvm -disable-llvm-optzns -o - | llvm-cxxfilt | FileCheck %s --check-prefixes=CHECK-P9,CHECK,CHECK-BE
// RUN: %clang -S -emit-llvm -target powerpc64le-gnu-linux -mcpu=pwr9 -DNO_WARN_X86_INTRINSICS %s \
// RUN:   -fno-discard-value-names -mllvm -disable-llvm-optzns -o - | llvm-cxxfilt | FileCheck %s --check-prefixes=CHECK-P9,CHECK,CHECK-LE

#include <mmintrin.h>

unsigned long long int ull1, ull2;
int i1, i2;
short s[4];
signed char c[8];
long long int ll1;
__m64 m1, m2, res;

void __attribute__((noinline))
test_add() {
  res = _mm_add_pi32(m1, m2);
  res = _mm_add_pi16(m1, m2);
  res = _mm_add_pi8(m1, m2);
  res = _mm_adds_pu16(m1, m2);
  res = _mm_adds_pu8(m1, m2);
  res = _mm_adds_pi16(m1, m2);
  res = _mm_adds_pi8(m1, m2);
}

// CHECK-LABEL: @test_add

// CHECK: define available_externally i64 @_mm_add_pi32

// CHECK-P9: [[REG1:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)
// CHECK-P9-NEXT: [[REG2:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG1]] to <4 x i32>
// CHECK-P9-NEXT: store <4 x i32> [[REG2]], <4 x i32>* [[REG3:[0-9a-zA-Z_%.]+]], align 16
// CHECK-P9-NEXT: [[REG4:[0-9a-zA-Z_%.]+]] = load i64, i64* {{[0-9a-zA-Z_%.]+}}, align 8
// CHECK-P9-NEXT: [[REG5:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)
// CHECK-P9-NEXT: [[REG6:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG5]] to <4 x i32>
// CHECK-P9-NEXT: store <4 x i32> [[REG6]], <4 x i32>* [[REG7:[0-9a-zA-Z_%.]+]], align 16
// CHECK-P9-NEXT: [[REG8:[0-9a-zA-Z_%.]+]] = load <4 x i32>, <4 x i32>* [[REG3]], align 16
// CHECK-P9-NEXT: [[REG9:[0-9a-zA-Z_%.]+]] = load <4 x i32>, <4 x i32>* [[REG7]], align 16
// CHECK-P9-NEXT: [[REG10:[0-9a-zA-Z_%.]+]] = call <4 x i32> @vec_add(int vector[4], int vector[4])(<4 x i32> [[REG8]], <4 x i32> [[REG9]])
// CHECK-P9-NEXT: store <4 x i32> [[REG10]], <4 x i32>* [[REG11:[0-9a-zA-Z_%.]+]], align 16
// CHECK-P9-NEXT: [[REG12:[0-9a-zA-Z_%.]+]] = load <4 x i32>, <4 x i32>* [[REG11]], align 16
// CHECK-P9-NEXT: [[REG13:[0-9a-zA-Z_%.]+]] = bitcast <4 x i32> %6 to <2 x i64>
// CHECK-P9-NEXT: [[REG14:[0-9a-zA-Z_%.]+]] = extractelement <2 x i64> [[REG13]], i32 0
// CHECK-P9-NEXT: ret i64 [[REG14]]

// CHECK-P8: [[REG15:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 0
// CHECK-P8-NEXT: [[REG16:[0-9a-zA-Z_%.]+]] = load i32, i32* [[REG15]]
// CHECK-P8: [[REG17:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 0
// CHECK-P8-NEXT: [[REG18:[0-9a-zA-Z_%.]+]] = load i32, i32* [[REG17]]
// CHECK-P8-NEXT: add nsw i32 [[REG16]], [[REG18]]
// CHECK-P8: [[REG19:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 1
// CHECK-P8-NEXT: [[REG20:[0-9a-zA-Z_%.]+]] = load i32, i32* [[REG19]]
// CHECK-P8: [[REG21:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 1
// CHECK-P8-NEXT: [[REG22:[0-9a-zA-Z_%.]+]] = load i32, i32* [[REG21]]
// CHECK-P8-NEXT: add nsw i32 [[REG20]], [[REG22]]

// CHECK: define available_externally i64 @_mm_add_pi16
// CHECK: [[REG23:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats
// CHECK-NEXT: [[REG24:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG23]] to <8 x i16>
// CHECK-NEXT: store <8 x i16> [[REG24]], <8 x i16>* [[REG25:[0-9a-zA-Z_%.]+]]
// CHECK: [[REG26:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats
// CHECK-NEXT: [[REG27:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG26]] to <8 x i16>
// CHECK-NEXT: store <8 x i16> [[REG27]], <8 x i16>* [[REG28:[0-9a-zA-Z_%.]+]]
// CHECK-NEXT: [[REG29:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG25]], align 16
// CHECK-NEXT: [[REG30:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG28]], align 16
// CHECK-NEXT: call <8 x i16> @vec_add(short vector[8], short vector[8])(<8 x i16> [[REG29]], <8 x i16> [[REG30]])

// CHECK: define available_externally i64 @_mm_add_pi8
// CHECK: [[REG31:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats
// CHECK-NEXT: [[REG32:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG31]] to <16 x i8>
// CHECK-NEXT: store <16 x i8> [[REG32]], <16 x i8>* [[REG33:[0-9a-zA-Z_%.]+]]
// CHECK: [[REG34:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats
// CHECK-NEXT: [[REG35:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG34]] to <16 x i8>
// CHECK-NEXT: store <16 x i8> [[REG35]], <16 x i8>* [[REG36:[0-9a-zA-Z_%.]+]]
// CHECK-NEXT: [[REG37:[0-9a-zA-Z_%.]+]] = load <16 x i8>, <16 x i8>* [[REG33]], align 16
// CHECK-NEXT: [[REG38:[0-9a-zA-Z_%.]+]] = load <16 x i8>, <16 x i8>* [[REG36]], align 16
// CHECK-NEXT: call <16 x i8> @vec_add(signed char vector[16], signed char vector[16])(<16 x i8> [[REG37]], <16 x i8> [[REG38]])

// CHECK: define available_externally i64 @_mm_adds_pu16
// CHECK: [[REG39:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats
// CHECK-NEXT: [[REG40:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG39]] to <8 x i16>
// CHECK-NEXT: store <8 x i16> [[REG40]], <8 x i16>* [[REG41:[0-9a-zA-Z_%.]+]]
// CHECK: [[REG42:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats
// CHECK-NEXT: [[REG43:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG42]] to <8 x i16>
// CHECK-NEXT: store <8 x i16> [[REG43]], <8 x i16>* [[REG44:[0-9a-zA-Z_%.]+]]
// CHECK-NEXT: [[REG45:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG41]], align 16
// CHECK-NEXT: [[REG46:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG44]], align 16
// CHECK-NEXT: call <8 x i16> @vec_adds(unsigned short vector[8], unsigned short vector[8])(<8 x i16> [[REG45]], <8 x i16> [[REG46]])

// CHECK: define available_externally i64 @_mm_adds_pu8
// CHECK: [[REG47:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats
// CHECK-NEXT: [[REG48:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG47]] to <16 x i8>
// CHECK-NEXT: store <16 x i8> [[REG48]], <16 x i8>* [[REG49:[0-9a-zA-Z_%.]+]]
// CHECK: [[REG50:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats
// CHECK-NEXT: [[REG51:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG50]] to <16 x i8>
// CHECK-NEXT: store <16 x i8> [[REG51]], <16 x i8>* [[REG52:[0-9a-zA-Z_%.]+]]
// CHECK-NEXT: [[REG53:[0-9a-zA-Z_%.]+]] = load <16 x i8>, <16 x i8>* [[REG49]], align 16
// CHECK-NEXT: [[REG54:[0-9a-zA-Z_%.]+]] = load <16 x i8>, <16 x i8>* [[REG52]], align 16
// CHECK-NEXT: call <16 x i8> @vec_adds(unsigned char vector[16], unsigned char vector[16])(<16 x i8> [[REG53]], <16 x i8> [[REG54]])

// CHECK: define available_externally i64 @_mm_adds_pi16
// CHECK: [[REG55:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats
// CHECK-NEXT: [[REG56:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG55]] to <8 x i16>
// CHECK-NEXT: store <8 x i16> [[REG56]], <8 x i16>* [[REG57:[0-9a-zA-Z_%.]+]]
// CHECK: [[REG58:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats
// CHECK-NEXT: [[REG59:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG58]] to <8 x i16>
// CHECK-NEXT: store <8 x i16> [[REG59]], <8 x i16>* [[REG60:[0-9a-zA-Z_%.]+]]
// CHECK-NEXT: [[REG61:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG57]], align 16
// CHECK-NEXT: [[REG62:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG60]], align 16
// CHECK-NEXT: call <8 x i16> @vec_adds(short vector[8], short vector[8])(<8 x i16> [[REG61]], <8 x i16> [[REG62]])

// CHECK: define available_externally i64 @_mm_adds_pi8
// CHECK: [[REG63:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats
// CHECK-NEXT: [[REG64:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG63]] to <16 x i8>
// CHECK-NEXT: store <16 x i8> [[REG64]], <16 x i8>* [[REG65:[0-9a-zA-Z_%.]+]]
// CHECK: [[REG66:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats
// CHECK-NEXT: [[REG67:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG66]] to <16 x i8>
// CHECK-NEXT: store <16 x i8> [[REG67]], <16 x i8>* [[REG68:[0-9a-zA-Z_%.]+]]
// CHECK-NEXT: [[REG69:[0-9a-zA-Z_%.]+]] = load <16 x i8>, <16 x i8>* [[REG65]], align 16
// CHECK-NEXT: [[REG70:[0-9a-zA-Z_%.]+]] = load <16 x i8>, <16 x i8>* [[REG68]], align 16
// CHECK-NEXT: call <16 x i8> @vec_adds(signed char vector[16], signed char vector[16])(<16 x i8> [[REG69]], <16 x i8> [[REG70]])

void __attribute__((noinline))
test_alt_name_add() {
  res = _m_paddb(m1, m2);
  res = _m_paddd(m1, m2);
  res = _m_paddsb(m1, m2);
  res = _m_paddsw(m1, m2);
  res = _m_paddusb(m1, m2);
  res = _m_paddusw(m1, m2);
  res = _m_paddw(m1, m2);
}

// CHECK-LABEL: @test_alt_name_add

// CHECK: define available_externally i64 @_m_paddb
// CHECK: [[REG71:[0-9a-zA-Z_%.]+]] = call i64 @_mm_add_pi8
// CHECK-NEXT: ret i64 [[REG71]]

// CHECK: define available_externally i64 @_m_paddd
// CHECK: [[REG72:[0-9a-zA-Z_%.]+]] = call i64 @_mm_add_pi32
// CHECK-NEXT: ret i64 [[REG72]]

// CHECK: define available_externally i64 @_m_paddsb
// CHECK: [[REG73:[0-9a-zA-Z_%.]+]] = call i64 @_mm_adds_pi8
// CHECK-NEXT: ret i64 [[REG73]]

// CHECK: define available_externally i64 @_m_paddsw
// CHECK: [[REG74:[0-9a-zA-Z_%.]+]] = call i64 @_mm_adds_pi16
// CHECK-NEXT: ret i64 [[REG74]]

// CHECK: define available_externally i64 @_m_paddusb
// CHECK: [[REG75:[0-9a-zA-Z_%.]+]] = call i64 @_mm_adds_pu8
// CHECK-NEXT: ret i64 [[REG75]]

// CHECK: define available_externally i64 @_m_paddusw
// CHECK: [[REG76:[0-9a-zA-Z_%.]+]] = call i64 @_mm_adds_pu16
// CHECK-NEXT: ret i64 [[REG76]]

// CHECK: define available_externally i64 @_m_paddw
// CHECK: [[REG77:[0-9a-zA-Z_%.]+]] = call i64 @_mm_add_pi16
// CHECK-NEXT: ret i64 [[REG77]]

void __attribute__((noinline))
test_cmp() {
  res = _mm_cmpeq_pi32(m1, m2);
  res = _mm_cmpeq_pi16(m1, m2);
  res = _mm_cmpeq_pi8(m1, m2);
  res = _mm_cmpgt_pi32(m1, m2);
  res = _mm_cmpgt_pi16(m1, m2);
  res = _mm_cmpgt_pi8(m1, m2);
}

// CHECK-LABEL: @test_cmp

// CHECK: define available_externally i64 @_mm_cmpeq_pi32

// CHECK-P9: [[REG78:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)
// CHECK-P9-NEXT: [[REG79:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG78]] to <4 x i32>
// CHECK-P9-NEXT: store <4 x i32> [[REG79]], <4 x i32>* [[REG80:[0-9a-zA-Z_%.]+]]
// CHECK-P9: [[REG81:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)
// CHECK-P9-NEXT: [[REG82:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG81]] to <4 x i32>
// CHECK-P9-NEXT: store <4 x i32> [[REG82]], <4 x i32>* [[REG83:[0-9a-zA-Z_%.]+]]
// CHECK-P9-NEXT: [[REG84:[0-9a-zA-Z_%.]+]] = load <4 x i32>, <4 x i32>* [[REG80]]
// CHECK-P9-NEXT: [[REG85:[0-9a-zA-Z_%.]+]] = load <4 x i32>, <4 x i32>* [[REG83]]
// CHECK-P9-NEXT: call <4 x i32> @vec_cmpeq(int vector[4], int vector[4])(<4 x i32> [[REG84]], <4 x i32> [[REG85]])

// CHECK-P8-COUNT-2: {{[0-9a-zA-Z_%.]+}} = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}
// CHECK-P8: [[REG86:[0-9a-zA-Z_%.]+]] = icmp eq i32 {{[0-9a-zA-Z_%.]+}}, {{[0-9a-zA-Z_%.]+}}
// CHECK-P8: [[REG87:[0-9a-zA-Z_%.]+]] = select i1 [[REG86]], i32 -1, i32 0
// CHECK-P8: {{[0-9a-zA-Z_%.]+}} = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}
// CHECK-P8-NEXT: store i32 [[REG87]], i32* {{[0-9a-zA-Z_%.]+}}
// CHECK-P8-COUNT-2: {{[0-9a-zA-Z_%.]+}} = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}
// CHECK-P8: store i32 {{[0-9a-zA-Z_%.]+}}, i32* {{[0-9a-zA-Z_%.]+}}

// CHECK: define available_externally i64 @_mm_cmpeq_pi16
// CHECK: [[REG88:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)
// CHECK-NEXT: [[REG89:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG88]] to <8 x i16>
// CHECK-NEXT: store <8 x i16> [[REG89]], <8 x i16>* [[REG90:[0-9a-zA-Z_%.]+]]
// CHECK: [[REG91:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)
// CHECK-NEXT: [[REG92:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG91]] to <8 x i16>
// CHECK-NEXT: store <8 x i16> [[REG92]], <8 x i16>* [[REG93:[0-9a-zA-Z_%.]+]]
// CHECK-NEXT: [[REG94:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG90]], align 16
// CHECK-NEXT: [[REG95:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG93]], align 16
// CHECK-NEXT: call <8 x i16> @vec_cmpeq(short vector[8], short vector[8])(<8 x i16> [[REG94]], <8 x i16> [[REG95]])

// CHECK: define available_externally i64 @_mm_cmpeq_pi8
// CHECK: call i64 asm "cmpb $0,$1,$2;\0A", "=r,r,r"
// CHECK-NEXT: store i64 {{[0-9a-zA-Z_%.]+}}, i64* [[REG96:[0-9a-zA-Z_%.]+]], align 8
// CHECK-NEXT: [[REG97:[0-9a-zA-Z_%.]+]] = load i64, i64* [[REG96]], align 8
// CHECK-NEXT: ret i64 [[REG97]]

// CHECK: define available_externally i64 @_mm_cmpgt_pi32

// CHECK-P9: [[REG98:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)
// CHECK-P9-NEXT: [[REG99:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG98]] to <4 x i32>
// CHECK-P9-NEXT: store <4 x i32> [[REG99]], <4 x i32>* [[REG100:[0-9a-zA-Z_%.]+]]
// CHECK-P9: [[REG101:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)
// CHECK-P9-NEXT: [[REG102:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG101]] to <4 x i32>
// CHECK-P9-NEXT: store <4 x i32> [[REG102]], <4 x i32>* [[REG103:[0-9a-zA-Z_%.]+]]
// CHECK-P9-NEXT: [[REG104:[0-9a-zA-Z_%.]+]] = load <4 x i32>, <4 x i32>* [[REG100]]
// CHECK-P9-NEXT: [[REG105:[0-9a-zA-Z_%.]+]] = load <4 x i32>, <4 x i32>* [[REG103]]
// CHECK-P9-NEXT: call <4 x i32> @vec_cmpgt(int vector[4], int vector[4])(<4 x i32> [[REG104]], <4 x i32> [[REG85]])

// CHECK-P8-COUNT-2: {{[0-9a-zA-Z_%.]+}} = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}
// CHECK-P8: [[REG106:[0-9a-zA-Z_%.]+]] = icmp sgt i32 {{[0-9a-zA-Z_%.]+}}, {{[0-9a-zA-Z_%.]+}}
// CHECK-P8: [[REG107:[0-9a-zA-Z_%.]+]] = select i1 [[REG106]], i32 -1, i32 0
// CHECK-P8: {{[0-9a-zA-Z_%.]+}} = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}
// CHECK-P8-NEXT: store i32 [[REG107]], i32* {{[0-9a-zA-Z_%.]+}}
// CHECK-P8-COUNT-2: {{[0-9a-zA-Z_%.]+}} = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}
// CHECK-P8: store i32 {{[0-9a-zA-Z_%.]+}}, i32* {{[0-9a-zA-Z_%.]+}}

// CHECK: define available_externally i64 @_mm_cmpgt_pi16
// CHECK: [[REG108:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)
// CHECK-NEXT: [[REG109:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG108]] to <8 x i16>
// CHECK-NEXT: store <8 x i16> [[REG109]], <8 x i16>* [[REG110:[0-9a-zA-Z_%.]+]]
// CHECK: [[REG111:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)
// CHECK-NEXT: [[REG112:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG111]] to <8 x i16>
// CHECK-NEXT: store <8 x i16> [[REG112]], <8 x i16>* [[REG113:[0-9a-zA-Z_%.]+]]
// CHECK-NEXT: [[REG114:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG110]]
// CHECK-NEXT: [[REG115:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG113]]
// CHECK-NEXT: call <8 x i16> @vec_cmpgt(short vector[8], short vector[8])(<8 x i16> [[REG114]], <8 x i16> [[REG115]])

// CHECK: define available_externally i64 @_mm_cmpgt_pi8
// CHECK: [[REG116:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)
// CHECK-NEXT: [[REG117:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG116]] to <16 x i8>
// CHECK-NEXT: store <16 x i8> [[REG117]], <16 x i8>* [[REG118:[0-9a-zA-Z_%.]+]]
// CHECK: [[REG119:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)
// CHECK-NEXT: [[REG120:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG119]] to <16 x i8>
// CHECK-NEXT: store <16 x i8> [[REG120]], <16 x i8>* [[REG121:[0-9a-zA-Z_%.]+]]
// CHECK-NEXT: [[REG122:[0-9a-zA-Z_%.]+]] = load <16 x i8>, <16 x i8>* [[REG118]]
// CHECK-NEXT: [[REG123:[0-9a-zA-Z_%.]+]] = load <16 x i8>, <16 x i8>* [[REG121]]
// CHECK-NEXT: call <16 x i8> @vec_cmpgt(signed char vector[16], signed char vector[16])(<16 x i8> [[REG122]], <16 x i8> [[REG123]])

void __attribute__((noinline))
test_alt_name_cmp() {
  res = _m_pcmpeqb(m1, m2);
  res = _m_pcmpeqd(m1, m2);
  res = _m_pcmpeqw(m1, m2);
  res = _m_pcmpgtb(m1, m2);
  res = _m_pcmpgtd(m1, m2);
  res = _m_pcmpgtw(m1, m2);
}

// CHECK-LABEL: @test_alt_name_cmp

// CHECK: define available_externally i64 @_m_pcmpeqb
// CHECK: [[REG124:[0-9a-zA-Z_%.]+]] = call i64 @_mm_cmpeq_pi8
// CHECK-NEXT: ret i64 [[REG124]]

// CHECK: define available_externally i64 @_m_pcmpeqd
// CHECK: [[REG125:[0-9a-zA-Z_%.]+]] = call i64 @_mm_cmpeq_pi32
// CHECK-NEXT: ret i64 [[REG125]]

// CHECK: define available_externally i64 @_m_pcmpeqw
// CHECK: [[REG126:[0-9a-zA-Z_%.]+]] = call i64 @_mm_cmpeq_pi16
// CHECK-NEXT: ret i64 [[REG126]]

// CHECK: define available_externally i64 @_m_pcmpgtb
// CHECK: [[REG127:[0-9a-zA-Z_%.]+]] = call i64 @_mm_cmpgt_pi8
// CHECK-NEXT: ret i64 [[REG127]]

// CHECK: define available_externally i64 @_m_pcmpgtd
// CHECK: [[REG128:[0-9a-zA-Z_%.]+]] = call i64 @_mm_cmpgt_pi32
// CHECK-NEXT: ret i64 [[REG128]]

// CHECK: define available_externally i64 @_m_pcmpgtw
// CHECK: [[REG129:[0-9a-zA-Z_%.]+]] = call i64 @_mm_cmpgt_pi16
// CHECK-NEXT: ret i64 [[REG129]]

void __attribute__((noinline))
test_convert() {
  ll1 = _mm_cvtm64_si64(m1);
  m1 = _mm_cvtsi32_si64(i1);
  m1 = _mm_cvtsi64_m64(ll1);
  i1 = _mm_cvtsi64_si32(m1);
}

// CHECK-LABEL: @test_convert

// CHECK: define available_externally i64 @_mm_cvtm64_si64
// CHECK: [[REG130:[0-9a-zA-Z_%.]+]] = load i64, i64* {{[0-9a-zA-Z_%.]+}}, align 8
// CHECK-NEXT: ret i64 [[REG130]]

// CHECK: define available_externally i64 @_mm_cvtsi32_si64
// CHECK: [[REG131:[0-9a-zA-Z_%.]+]] = load i32, i32* {{[0-9a-zA-Z_%.]+}}
// CHECK-NEXT: [[REG132:[0-9a-zA-Z_%.]+]] = zext i32 [[REG131]] to i64
// CHECK-NEXT: ret i64 [[REG132]]

// CHECK: define available_externally i64 @_mm_cvtsi64_m64
// CHECK: [[REG133:[0-9a-zA-Z_%.]+]] = load i64, i64* {{[0-9a-zA-Z_%.]+}}, align 8
// CHECK-NEXT: ret i64 [[REG133]]

// CHECK: define available_externally signext i32 @_mm_cvtsi64_si32
// CHECK: [[REG134:[0-9a-zA-Z_%.]+]] = load i64, i64* {{[0-9a-zA-Z_%.]+}}, align 8
// CHECK-NEXT: [[REG135:[0-9a-zA-Z_%.]+]] = trunc i64 [[REG134]] to i32
// CHECK-NEXT: ret i32 [[REG135]]

void __attribute__((noinline))
test_alt_name_convert() {
  m1 = _m_from_int(i1);
  m1 = _m_from_int64(ll1);
  i1 = _m_to_int(m1);
  ll1 = _m_to_int64(m1);
}

// CHECK-LABEL: @test_alt_name_convert

// CHECK: define available_externally i64 @_m_from_int
// CHECK: [[REG136:[0-9a-zA-Z_%.]+]] = call i64 @_mm_cvtsi32_si64
// CHECK-NEXT: ret i64 [[REG136]]

// CHECK: define available_externally i64 @_m_from_int64
// CHECK: [[REG137:[0-9a-zA-Z_%.]+]] = load i64, i64* {{[0-9a-zA-Z_%.]+}}
// CHECK-NEXT: ret i64 [[REG137]]

// CHECK: define available_externally signext i32 @_m_to_int
// CHECK: [[REG138:[0-9a-zA-Z_%.]+]] = call signext i32 @_mm_cvtsi64_si32
// CHECK-NEXT: ret i32 [[REG138]]

// CHECK: define available_externally i64 @_m_to_int64
// CHECK: [[REG139:[0-9a-zA-Z_%.]+]] = load i64, i64* {{[0-9a-zA-Z_%.]+}}
// CHECK-NEXT: ret i64 [[REG139]]

void __attribute__((noinline))
test_empty() {
  _mm_empty();
  _m_empty();
}

// CHECK-LABEL: @test_empty

// CHECK: define available_externally void @_mm_empty
// CHECK-NEXT: entry
// CHECK-NEXT: ret void

// CHECK: define available_externally void @_m_empty
// CHECK-NEXT: entry
// CHECK-NEXT: ret void

void __attribute__((noinline))
test_logic() {
  res = _mm_and_si64(m1, m2);
  res = _mm_andnot_si64(m1, m2);
  res = _mm_or_si64(m1, m2);
  res = _mm_xor_si64(m1, m2);
}

// CHECK-LABEL: @test_logic

// CHECK: define available_externally i64 @_mm_and_si64
// CHECK: [[REG140:[0-9a-zA-Z_%.]+]] = and i64 {{[0-9a-zA-Z_%.]+}}, {{[0-9a-zA-Z_%.]+}}
// CHECK-NEXT: ret i64 [[REG140]]

// CHECK: define available_externally i64 @_mm_andnot_si64
// CHECK: [[REG141:[0-9a-zA-Z_%.]+]] = xor i64 {{[0-9a-zA-Z_%.]+}}, -1
// CHECK: [[REG142:[0-9a-zA-Z_%.]+]] = and i64 [[REG141]], {{[0-9a-zA-Z_%.]+}}
// CHECK-NEXT: ret i64 [[REG142]]

// CHECK: define available_externally i64 @_mm_or_si64
// CHECK: [[REG143:[0-9a-zA-Z_%.]+]] = or i64 {{[0-9a-zA-Z_%.]+}}, {{[0-9a-zA-Z_%.]+}}
// CHECK-NEXT: ret i64 [[REG143]]

// CHECK: define available_externally i64 @_mm_xor_si64
// CHECK: [[REG144:[0-9a-zA-Z_%.]+]] = xor i64 {{[0-9a-zA-Z_%.]+}}, {{[0-9a-zA-Z_%.]+}}
// CHECK-NEXT: ret i64 [[REG144]]

void __attribute__((noinline))
test_alt_name_logic() {
  res = _m_pand(m1, m2);
  res = _m_pandn(m1, m2);
  res = _m_por(m1, m2);
  res = _m_pxor(m1, m2);
}

// CHECK-LABEL: @test_alt_name_logic

// CHECK: define available_externally i64 @_m_pand
// CHECK: [[REG145:[0-9a-zA-Z_%.]+]] = call i64 @_mm_and_si64
// CHECK-NEXT: ret i64 [[REG145]]

// CHECK: define available_externally i64 @_m_pandn
// CHECK: [[REG146:[0-9a-zA-Z_%.]+]] = call i64 @_mm_andnot_si64
// CHECK-NEXT: ret i64 [[REG146]]

// CHECK: define available_externally i64 @_m_por
// CHECK: [[REG147:[0-9a-zA-Z_%.]+]] = call i64 @_mm_or_si64
// CHECK-NEXT: ret i64 [[REG147]]

// CHECK: define available_externally i64 @_m_pxor
// CHECK: [[REG148:[0-9a-zA-Z_%.]+]] = call i64 @_mm_xor_si64
// CHECK-NEXT: ret i64 [[REG148]]

void __attribute__((noinline))
test_madd() {
  res = _mm_madd_pi16(m1, m2);
  res = _m_pmaddwd(m1, m2);
}

// CHECK-LABEL: @test_madd

// CHECK: define available_externally i64 @_mm_madd_pi16
// CHECK: store <4 x i32> zeroinitializer, <4 x i32>* [[REG149:[0-9a-zA-Z_%.]+]], align 16
// CHECK: [[REG150:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)(i64 {{[0-9a-zA-Z_%.]+}})
// CHECK-NEXT: [[REG151:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG150]] to <8 x i16>
// CHECK-NEXT: store <8 x i16> [[REG151]], <8 x i16>* [[REG152:[0-9a-zA-Z_%.]+]], align 16
// CHECK: [[REG153:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)(i64 {{[0-9a-zA-Z_%.]+}})
// CHECK-NEXT: [[REG154:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG153]] to <8 x i16>
// CHECK-NEXT: store <8 x i16> [[REG154]], <8 x i16>* [[REG155:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG156:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG152]], align 16
// CHECK-NEXT: [[REG157:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG155]], align 16
// CHECK-NEXT: [[REG158:[0-9a-zA-Z_%.]+]] = load <4 x i32>, <4 x i32>* [[REG149]], align 16
// CHECK-NEXT: [[REG159:[0-9a-zA-Z_%.]+]] = call <4 x i32> @vec_vmsumshm(<8 x i16> [[REG156]], <8 x i16> [[REG157]], <4 x i32> [[REG158]])
// CHECK-NEXT: store <4 x i32> [[REG159]], <4 x i32>* {{[0-9a-zA-Z_%.]+}}, align 16

// CHECK: define available_externally i64 @_m_pmaddwd
// CHECK: [[REG160:[0-9a-zA-Z_%.]+]] = call i64 @_mm_madd_pi16
// CHECK-NEXT: ret i64 [[REG160]]

void __attribute__((noinline))
test_mul() {
  res = _mm_mulhi_pi16(m1, m2);
  res = _mm_mullo_pi16(m1, m2);
}

// CHECK-LABEL: @test_mul

// CHECK: define available_externally i64 @_mm_mulhi_pi16
// CHECK-BE: store <16 x i8> <i8 0, i8 1, i8 16, i8 17, i8 4, i8 5, i8 20, i8 21, i8 0, i8 1, i8 16, i8 17, i8 4, i8 5, i8 20, i8 21>, <16 x i8>* {{[0-9a-zA-Z_%.]+}}, align 16
// CHECK-LE: store <16 x i8> <i8 2, i8 3, i8 18, i8 19, i8 6, i8 7, i8 22, i8 23, i8 10, i8 11, i8 26, i8 27, i8 14, i8 15, i8 30, i8 31>, <16 x i8>* {{[0-9a-zA-Z_%.]+}}, align 16
// CHECK: [[REG161:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)
// CHECK: store <8 x i16> {{[0-9a-zA-Z_%.]+}}, <8 x i16>* [[REG162:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG163:[0-9a-zA-Z_%.]+]] = load i64, i64* {{[0-9a-zA-Z_%.]+}}, align 8
// CHECK-NEXT: [[REG164:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)
// CHECK-NEXT: [[REG165:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG164]] to <8 x i16>
// CHECK-NEXT: store <8 x i16> [[REG165]], <8 x i16>* [[REG166:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG167:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG162]], align 16
// CHECK-NEXT: [[REG168:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG166]], align 16
// CHECK-NEXT: [[REG169:[0-9a-zA-Z_%.]+]] = call <4 x i32> @vec_vmulesh(<8 x i16> [[REG167]], <8 x i16> [[REG168]])
// CHECK-NEXT: store <4 x i32> [[REG169]], <4 x i32>* [[REG170:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG171:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG162]], align 16
// CHECK-NEXT: [[REG172:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG166]], align 16
// CHECK-NEXT: [[REG173:[0-9a-zA-Z_%.]+]] = call <4 x i32> @vec_vmulosh(<8 x i16> [[REG171]], <8 x i16> [[REG172]])
// CHECK-NEXT: store <4 x i32> [[REG173]], <4 x i32>* [[REG174:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG175:[0-9a-zA-Z_%.]+]] = load <4 x i32>, <4 x i32>* [[REG170]], align 16
// CHECK-NEXT: [[REG176:[0-9a-zA-Z_%.]+]] = load <4 x i32>, <4 x i32>* [[REG174]], align 16
// CHECK-NEXT: [[REG177:[0-9a-zA-Z_%.]+]] = load <16 x i8>, <16 x i8>* {{[0-9a-zA-Z_%.]+}}, align 16
// CHECK-NEXT: call <4 x i32> @vec_perm(int vector[4], int vector[4], unsigned char vector[16])(<4 x i32> [[REG175]], <4 x i32> [[REG176]], <16 x i8> [[REG177]])

// CHECK: define available_externally i64 @_mm_mullo_pi16
// CHECK: [[REG178:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)
// CHECK-NEXT: [[REG179:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG178]] to <8 x i16>
// CHECK-NEXT: store <8 x i16> [[REG179]], <8 x i16>* [[REG180:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG181:[0-9a-zA-Z_%.]+]] = load i64, i64* {{[0-9a-zA-Z_%.]+}}, align 8
// CHECK-NEXT: [[REG182:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)
// CHECK-NEXT: [[REG183:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG182]] to <8 x i16>
// CHECK-NEXT: store <8 x i16> [[REG183]], <8 x i16>* [[REG184:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG185:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG180]], align 16
// CHECK-NEXT: [[REG186:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG184]], align 16
// CHECK-NEXT: [[REG187:[0-9a-zA-Z_%.]+]] = mul <8 x i16> [[REG185]], [[REG186]]
// CHECK-NEXT: store <8 x i16> [[REG187]], <8 x i16>* {{[0-9a-zA-Z_%.]+}}, align 16

void __attribute__((noinline))
test_alt_name_mul() {
  res = _m_pmulhw(m1, m2);
  res = _m_pmullw(m1, m2);
}

// CHECK-LABEL: @test_alt_name_mul

// CHECK: define available_externally i64 @_m_pmulhw
// CHECK: [[REG188:[0-9a-zA-Z_%.]+]] = call i64 @_mm_mulhi_pi16
// CHECK-NEXT: ret i64 [[REG188]]

// CHECK: define available_externally i64 @_m_pmullw
// CHECK: [[REG189:[0-9a-zA-Z_%.]+]] = call i64 @_mm_mullo_pi16
// CHECK-NEXT: ret i64 [[REG189]]

void __attribute__((noinline))
test_packs() {
  res = _mm_packs_pu16((__m64)ull1, (__m64)ull2);
  res = _mm_packs_pi16((__m64)ull1, (__m64)ull2);
  res = _mm_packs_pi32((__m64)ull1, (__m64)ull2);
}

// CHECK-LABEL: @test_packs

// CHECK: define available_externally i64 @_mm_packs_pu16(i64 [[REG190:[0-9a-zA-Z_%.]+]], i64 [[REG191:[0-9a-zA-Z_%.]+]])
// CHECK: store i64 [[REG190]], i64* [[REG192:[0-9a-zA-Z_%.]+]], align 8
// CHECK-NEXT: store i64 [[REG191]], i64* [[REG193:[0-9a-zA-Z_%.]+]], align 8
// CHECK-LE: load i64, i64* [[REG192]], align 8
// CHECK: load i64, i64* [[REG193]], align 8
// CHECK-BE: load i64, i64* [[REG192]], align 8
// CHECK: [[REG194:[0-9a-zA-Z_%.]+]] = call <8 x i16> @vec_cmplt
// CHECK-NEXT: store <8 x i16> [[REG194]], <8 x i16>* [[REG195:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG196:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG197:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG198:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG197]], align 16
// CHECK-NEXT: [[REG199:[0-9a-zA-Z_%.]+]] = call <16 x i8> @vec_packs(unsigned short vector[8], unsigned short vector[8])(<8 x i16> [[REG196]], <8 x i16> [[REG198]])
// CHECK-NEXT: store <16 x i8> [[REG199]], <16 x i8>* [[REG200:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG201:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG195]], align 16
// CHECK-NEXT: [[REG202:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG195]], align 16
// CHECK-NEXT: [[REG203:[0-9a-zA-Z_%.]+]] = call <16 x i8> @vec_pack(bool vector[8], bool vector[8])(<8 x i16> [[REG201]], <8 x i16> [[REG202]])
// CHECK-NEXT: store <16 x i8> [[REG203]], <16 x i8>* [[REG204:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG205:[0-9a-zA-Z_%.]+]] = load <16 x i8>, <16 x i8>* [[REG200]], align 16
// CHECK-NEXT: [[REG206:[0-9a-zA-Z_%.]+]] = load <16 x i8>, <16 x i8>* [[REG204]], align 16
// CHECK-NEXT: call <16 x i8> @vec_sel(unsigned char vector[16], unsigned char vector[16], bool vector[16])(<16 x i8> [[REG205]], <16 x i8> zeroinitializer, <16 x i8> [[REG206]])

// CHECK: define available_externally i64 @_mm_packs_pi16(i64 [[REG207:[0-9a-zA-Z_%.]+]], i64 [[REG208:[0-9a-zA-Z_%.]+]])
// CHECK: store i64 [[REG207]], i64* [[REG209:[0-9a-zA-Z_%.]+]], align 8
// CHECK-NEXT: store i64 [[REG208]], i64* [[REG210:[0-9a-zA-Z_%.]+]], align 8
// CHECK-LE: load i64, i64* [[REG209]], align 8
// CHECK: load i64, i64* [[REG210]], align 8
// CHECK-BE: load i64, i64* [[REG209]], align 8
// CHECK: [[REG211:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG212:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG213:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG212]], align 16
// CHECK-NEXT: call <16 x i8> @vec_packs(short vector[8], short vector[8])(<8 x i16> [[REG211]], <8 x i16> [[REG213]])

// CHECK: define available_externally i64 @_mm_packs_pi32(i64 [[REG214:[0-9a-zA-Z_%.]+]], i64 [[REG215:[0-9a-zA-Z_%.]+]])
// CHECK: store i64 [[REG214]], i64* [[REG216:[0-9a-zA-Z_%.]+]], align 8
// CHECK-NEXT: store i64 [[REG215]], i64* [[REG217:[0-9a-zA-Z_%.]+]], align 8
// CHECK-LE: load i64, i64* [[REG216]], align 8
// CHECK: load i64, i64* [[REG217]], align 8
// CHECK-BE: load i64, i64* [[REG216]], align 8
// CHECK: [[REG218:[0-9a-zA-Z_%.]+]] = load <4 x i32>, <4 x i32>* [[REG219:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG220:[0-9a-zA-Z_%.]+]] = load <4 x i32>, <4 x i32>* [[REG219]], align 16
// CHECK-NEXT: call <8 x i16> @vec_packs(int vector[4], int vector[4])(<4 x i32> [[REG218]], <4 x i32> [[REG220]])

void __attribute__((noinline))
test_alt_name_packs() {
  res = _m_packssdw(m1, m2);
  res = _m_packsswb(m1, m2);
  res = _m_packuswb(m1, m2);
}

// CHECK-LABEL: @test_alt_name_packs

// CHECK: define available_externally i64 @_m_packssdw
// CHECK: [[REG221:[0-9a-zA-Z_%.]+]] = call i64 @_mm_packs_pi32
// CHECK-NEXT: ret i64 [[REG221]]

// CHECK: define available_externally i64 @_m_packsswb
// CHECK: [[REG222:[0-9a-zA-Z_%.]+]] = call i64 @_mm_packs_pi16
// CHECK-NEXT: ret i64 [[REG222]]

// CHECK: define available_externally i64 @_m_packuswb
// CHECK: [[REG223:[0-9a-zA-Z_%.]+]] = call i64 @_mm_packs_pu16
// CHECK-NEXT: ret i64 [[REG223]]

void __attribute__((noinline))
test_set() {
  m1 = _mm_set_pi32(2134, -128);
  m1 = _mm_set_pi16(2134, -128, 1234, 6354);
  m1 = _mm_set_pi8(-128, 10, 0, 123, -1, -5, 127, 5);
}

// CHECK-LABEL: @test_set

// CHECK: define available_externally i64 @_mm_set_pi32
// CHECK: [[REG224:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 0
// CHECK-NEXT: store i32 {{[0-9a-zA-Z_%.]+}}, i32* [[REG224]]
// CHECK: [[REG225:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 1
// CHECK-NEXT: store i32 {{[0-9a-zA-Z_%.]+}}, i32* [[REG225]]

// CHECK: define available_externally i64 @_mm_set_pi16
// CHECK: [[REG226:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 0
// CHECK-NEXT: store i16 {{[0-9a-zA-Z_%.]+}}, i16* [[REG226]]
// CHECK: [[REG227:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 1
// CHECK-NEXT: store i16 {{[0-9a-zA-Z_%.]+}}, i16* [[REG227]]
// CHECK: [[REG228:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 2
// CHECK-NEXT: store i16 {{[0-9a-zA-Z_%.]+}}, i16* [[REG228]]
// CHECK: [[REG229:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 3
// CHECK-NEXT: store i16 {{[0-9a-zA-Z_%.]+}}, i16* [[REG229]]

// CHECK: define available_externally i64 @_mm_set_pi8
// CHECK: [[REG230:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [8 x i8], [8 x i8]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 0
// CHECK-NEXT: store i8 {{[0-9a-zA-Z_%.]+}}, i8* [[REG230]]
// CHECK: [[REG231:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [8 x i8], [8 x i8]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 1
// CHECK-NEXT: store i8 {{[0-9a-zA-Z_%.]+}}, i8* [[REG231]]
// CHECK: [[REG232:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [8 x i8], [8 x i8]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 2
// CHECK-NEXT: store i8 {{[0-9a-zA-Z_%.]+}}, i8* [[REG232]]
// CHECK: [[REG233:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [8 x i8], [8 x i8]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 3
// CHECK-NEXT: store i8 {{[0-9a-zA-Z_%.]+}}, i8* [[REG233]]
// CHECK: [[REG234:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [8 x i8], [8 x i8]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 4
// CHECK-NEXT: store i8 {{[0-9a-zA-Z_%.]+}}, i8* [[REG234]]
// CHECK: [[REG235:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [8 x i8], [8 x i8]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 5
// CHECK-NEXT: store i8 {{[0-9a-zA-Z_%.]+}}, i8* [[REG235]]
// CHECK: [[REG236:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [8 x i8], [8 x i8]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 6
// CHECK-NEXT: store i8 {{[0-9a-zA-Z_%.]+}}, i8* [[REG236]]
// CHECK: [[REG237:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [8 x i8], [8 x i8]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 7
// CHECK-NEXT: store i8 {{[0-9a-zA-Z_%.]+}}, i8* [[REG237]]

void __attribute__((noinline))
test_set1() {
  res = _mm_set1_pi32(i1);
  res = _mm_set1_pi16(s[0]);
  res = _mm_set1_pi8(c[0]);
}

// CHECK-LABEL: @test_set1

// CHECK: define available_externally i64 @_mm_set1_pi32
// CHECK: [[REG244:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 0
// CHECK-NEXT: store i32 {{[0-9a-zA-Z_%.]+}}, i32* [[REG244]], align 8
// CHECK-NEXT: [[REG245:[0-9a-zA-Z_%.]+]] = load i32, i32* {{[0-9a-zA-Z_%.]+}}, align 4
// CHECK: [[REG246:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 1
// CHECK-NEXT: store i32 {{[0-9a-zA-Z_%.]+}}, i32* [[REG246]], align 4

// CHECK: define available_externally i64 @_mm_set1_pi16

// CHECK-P9: [[REG247:[0-9a-zA-Z_%.]+]] = load i16, i16* {{[0-9a-zA-Z_%.]+}}, align 2
// CHECK-P9-NEXT: [[REG248:[0-9a-zA-Z_%.]+]] = call <8 x i16> @vec_splats(short)(i16 signext [[REG247]])
// CHECK-P9-NEXT: store <8 x i16> %call, <8 x i16>* [[REG249:[0-9a-zA-Z_%.]+]], align 16
// CHECK-P9-NEXT: [[REG250:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG249:[0-9a-zA-Z_%.]+]], align 16
// CHECK-P9-NEXT: [[REG251:[0-9a-zA-Z_%.]+]] = bitcast <8 x i16> [[REG250]] to <2 x i64>
// CHECK-P9-NEXT: [[REG252:[0-9a-zA-Z_%.]+]] = extractelement <2 x i64> [[REG251]], i32 0
// CHECK-P9-NEXT: ret i64 [[REG252]]

// CHECK-P8: [[REG253:[0-9a-zA-Z_%.]+]] = load i16, i16* [[REG254:[0-9a-zA-Z_%.]+]], align 2
// CHECK-P8: [[REG255:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 0
// CHECK-P8-NEXT: store i16 [[REG253]], i16* [[REG255]], align 8
// CHECK-P8-NEXT: [[REG250:[0-9a-zA-Z_%.]+]] = load i16, i16* [[REG254]], align 2
// CHECK-P8: [[REG256:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 1
// CHECK-P8-NEXT: store i16 [[REG250]], i16* [[REG256]], align 2
// CHECK-P8-NEXT: [[REG251:[0-9a-zA-Z_%.]+]] = load i16, i16* [[REG254]], align 2
// CHECK-P8: [[REG257:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 2
// CHECK-P8-NEXT: store i16 [[REG251]], i16* [[REG257]], align 4
// CHECK-P8-NEXT: [[REG258:[0-9a-zA-Z_%.]+]] = load i16, i16* [[REG254]], align 2
// CHECK-P8: [[REG259:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 3
// CHECK-P8-NEXT: store i16 [[REG258]], i16* [[REG259]], align 2

// CHECK: define available_externally i64 @_mm_set1_pi8
// CHECK: [[REG260:[0-9a-zA-Z_%.]+]] = call <16 x i8> @vec_splats(signed char)(i8 signext {{[0-9a-zA-Z_%.]+}})
// CHECK-NEXT: store <16 x i8> [[REG260]], <16 x i8>* [[REG261:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG262:[0-9a-zA-Z_%.]+]] = load <16 x i8>, <16 x i8>* [[REG261:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG263:[0-9a-zA-Z_%.]+]] = bitcast <16 x i8> %1 to <2 x i64>
// CHECK-NEXT: [[REG264:[0-9a-zA-Z_%.]+]] = extractelement <2 x i64> [[REG263]], i32 0
// CHECK-NEXT: ret i64 [[REG264]]

void __attribute__((noinline))
test_setr() {
  res = _mm_setr_pi32(i1, i2);
  res = _mm_setr_pi16(s[0], s[1], s[2], s[3]);
  res = _mm_setr_pi8(c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]);
}

// CHECK-LABEL: @test_setr

// CHECK: define available_externally i64 @_mm_setr_pi32
// CHECK: [[REG265:[0-9a-zA-Z_%.]+]] = load i32, i32* {{[0-9a-zA-Z_%.]+}}, align 4
// CHECK: [[REG266:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 0
// CHECK-NEXT: store i32 [[REG265:[0-9a-zA-Z_%.]+]], i32* [[REG266]], align 8
// CHECK-NEXT: [[REG267:[0-9a-zA-Z_%.]+]] = load i32, i32* {{[0-9a-zA-Z_%.]+}}, align 4
// CHECK: [[REG268:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 1
// CHECK-NEXT: store i32 [[REG267]], i32* [[REG268]], align 4

// CHECK: define available_externally i64 @_mm_setr_pi16
// CHECK: [[REG269:[0-9a-zA-Z_%.]+]] = call i64 @_mm_set_pi16
// CHECK-NEXT: ret i64 [[REG269]]

// CHECK: define available_externally i64 @_mm_setr_pi8
// CHECK: [[REG270:[0-9a-zA-Z_%.]+]] = call i64 @_mm_set_pi8
// CHECK-NEXT: ret i64 [[REG270]]

void __attribute__((noinline))
test_setzero() {
  res = _mm_setzero_si64();
}

// CHECK-LABEL: @test_setzero

// CHECK: define available_externally i64 @_mm_setzero_si64
// CHECK: entry
// CHECK-NEXT: ret i64 0

void __attribute__((noinline))
test_sll() {
  res = _mm_sll_pi16(m1, m2);
  res = _mm_sll_pi32(m1, m2);
  res = _mm_sll_si64(m1, m2);
  res = _mm_slli_pi16(m1, i1);
  res = _mm_slli_pi32(m1, i1);
  res = _mm_slli_si64(m1, i1);
}

// CHECK-LABEL: @test_sll

// CHECK: define available_externally i64 @_mm_sll_pi16
// CHECK: [[REG271:[0-9a-zA-Z_%.]+]] = icmp ule i64 {{[0-9a-zA-Z_%.]+}}, 15
// CHECK-NEXT: br i1 [[REG271]], label %[[REG272:[0-9a-zA-Z_.]+]], label %[[REG273:[0-9a-zA-Z_.]+]]
// CHECK: [[REG272]]
// CHECK: call <2 x i64> @vec_splats(unsigned long long)
// CHECK: trunc i64 {{[0-9a-zA-Z_%.]+}} to i16
// CHECK-NEXT: call <8 x i16> @vec_splats(unsigned short)
// CHECK: call <8 x i16> @vec_sl(short vector[8], unsigned short vector[8])
// CHECK: store i64 [[REG274:[0-9a-zA-Z_%.]+]], i64* [[REG275:[0-9a-zA-Z_%.]+]], align 8
// CHECK-NEXT: br label %[[REG276:[0-9a-zA-Z_%.]+]]
// CHECK: [[REG273]]
// CHECK-NEXT: store i64 0, i64* [[REG275]], align 8
// CHECK-NEXT: br label %[[REG276]]
// CHECK: [[REG276]]
// CHECK-NEXT: [[REG277:[0-9a-zA-Z_%.]+]] = load i64, i64* [[REG275]], align 8
// CHECK-NEXT: ret i64 [[REG277]]

// CHECK: define available_externally i64 @_mm_sll_pi32
// CHECK: [[REG278:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 0
// CHECK-NEXT: [[REG279:[0-9a-zA-Z_%.]+]] = load i32, i32* [[REG278]]
// CHECK-NEXT: [[REG280:[0-9a-zA-Z_%.]+]] = load i64, i64* {{[0-9a-zA-Z_%.]+}}
// CHECK-NEXT: [[REG281:[0-9a-zA-Z_%.]+]] = trunc i64 [[REG280]] to i32
// CHECK-NEXT: [[REG282:[0-9a-zA-Z_%.]+]] = shl i32 [[REG279]], [[REG281]]
// CHECK: [[REG283:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 0
// CHECK-NEXT: store i32 [[REG282]], i32* [[REG283]]
// CHECK: [[REG284:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 1
// CHECK-NEXT: [[REG285:[0-9a-zA-Z_%.]+]] = load i32, i32* [[REG284]], align 4
// CHECK: trunc i64 {{[0-9a-zA-Z_%.]+}} to i32
// CHECK-NEXT: [[REG286:[0-9a-zA-Z_%.]+]] = shl i32 [[REG285]], {{[0-9a-zA-Z_%.]+}}
// CHECK: [[REG287:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 1
// CHECK-NEXT: store i32 [[REG286]], i32* [[REG287]], align 4

// CHECK: define available_externally i64 @_mm_sll_si64
// CHECK: [[REG288:[0-9a-zA-Z_%.]+]] = shl i64 {{[0-9a-zA-Z_%.]+}}, {{[0-9a-zA-Z_%.]+}}
// CHECK-NEXT: ret i64 [[REG288]]

// CHECK: define available_externally i64 @_mm_slli_pi16
// CHECK: [[REG289:[0-9a-zA-Z_%.]+]] = sext i32 {{[0-9a-zA-Z_%.]+}} to i64
// CHECK-NEXT: [[REG290:[0-9a-zA-Z_%.]+]] = call i64 @_mm_sll_pi16(i64 {{[0-9a-zA-Z_%.]+}}, i64 [[REG289]])
// CHECK-NEXT: ret i64 [[REG290]]

// CHECK: define available_externally i64 @_mm_slli_pi32
// CHECK: [[REG291:[0-9a-zA-Z_%.]+]] = sext i32 {{[0-9a-zA-Z_%.]+}} to i64
// CHECK-NEXT: [[REG292:[0-9a-zA-Z_%.]+]] = call i64 @_mm_sll_pi32(i64 {{[0-9a-zA-Z_%.]+}}, i64 [[REG291]])
// CHECK-NEXT: ret i64 [[REG292]]

// CHECK: define available_externally i64 @_mm_slli_si64
// CHECK: [[REG293:[0-9a-zA-Z_%.]+]] = zext i32 {{[0-9a-zA-Z_%.]+}} to i64
// CHECK-NEXT: [[REG294:[0-9a-zA-Z_%.]+]] = shl i64 {{[0-9a-zA-Z_%.]+}}, [[REG293]]
// CHECK-NEXT: ret i64 [[REG294]]

void __attribute__((noinline))
test_alt_name_sll() {
  res = _m_pslld(m1, m2);
  res = _m_pslldi(m1, i1);
  res = _m_psllq(m1, m2);
  res = _m_psllqi(m1, i1);
  res = _m_psllw(m1, m2);
  res = _m_psllwi(m1, i1);
}

// CHECK-LABEL: @test_alt_name_sll

// CHECK: define available_externally i64 @_m_pslld
// CHECK: [[REG295:[0-9a-zA-Z_%.]+]] = call i64 @_mm_sll_pi32
// CHECK-NEXT: ret i64 [[REG295]]

// CHECK: define available_externally i64 @_m_pslldi
// CHECK: [[REG296:[0-9a-zA-Z_%.]+]] = call i64 @_mm_slli_pi32
// CHECK-NEXT: ret i64 [[REG296]]

// CHECK: define available_externally i64 @_m_psllq
// CHECK: [[REG297:[0-9a-zA-Z_%.]+]] = call i64 @_mm_sll_si64
// CHECK-NEXT: ret i64 [[REG297]]

// CHECK: define available_externally i64 @_m_psllqi
// CHECK: [[REG298:[0-9a-zA-Z_%.]+]] = call i64 @_mm_slli_si64
// CHECK-NEXT: ret i64 [[REG298]]

// CHECK: define available_externally i64 @_m_psllw
// CHECK: [[REG299:[0-9a-zA-Z_%.]+]] = call i64 @_mm_sll_pi16
// CHECK-NEXT: ret i64 [[REG299]]

// CHECK: define available_externally i64 @_m_psllwi
// CHECK: [[REG300:[0-9a-zA-Z_%.]+]] = call i64 @_mm_slli_pi16
// CHECK-NEXT: ret i64 [[REG300]]

void __attribute__((noinline))
test_sra() {
  res = _mm_sra_pi32(m1, m2);
  res = _mm_sra_pi16(m1, m2);
  res = _mm_srai_pi32(m1, i1);
  res = _mm_srai_pi16(m1, i1);
}

// CHECK-LABEL: @test_sra

// CHECK: define available_externally i64 @_mm_sra_pi32
// CHECK: [[REG301:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 0
// CHECK-NEXT: [[REG302:[0-9a-zA-Z_%.]+]] = load i32, i32* [[REG301]], align 8
// CHECK-NEXT: [[REG303:[0-9a-zA-Z_%.]+]] = load i64, i64* {{[0-9a-zA-Z_%.]+}}, align 8
// CHECK-NEXT: [[REG304:[0-9a-zA-Z_%.]+]] = trunc i64 [[REG303]] to i32
// CHECK-NEXT: [[REG305:[0-9a-zA-Z_%.]+]] = ashr i32 [[REG302]], [[REG304]]
// CHECK: [[REG306:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 0
// CHECK-NEXT: store i32 [[REG305]], i32* [[REG306]], align 8
// CHECK: [[REG307:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 1
// CHECK-NEXT: [[REG308:[0-9a-zA-Z_%.]+]] = load i32, i32* [[REG307]], align 4
// CHECK-NEXT: [[REG309:[0-9a-zA-Z_%.]+]] = load i64, i64* {{[0-9a-zA-Z_%.]+}}, align 8
// CHECK-NEXT: [[REG310:[0-9a-zA-Z_%.]+]] = trunc i64 [[REG309]] to i32
// CHECK-NEXT: [[REG311:[0-9a-zA-Z_%.]+]] = ashr i32 [[REG308]], [[REG310]]
// CHECK: [[REG312:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 1
// CHECK-NEXT: store i32 [[REG311]], i32* [[REG312]], align 4

// CHECK: define available_externally i64 @_mm_sra_pi16
// CHECK: [[REG313:[0-9a-zA-Z_%.]+]] = icmp ule i64 {{[0-9a-zA-Z_%.]+}}, 15
// CHECK-NEXT: br i1 [[REG313]], label %[[REG314:[0-9a-zA-Z_%.]+]], label %[[REG315:[0-9a-zA-Z_%.]+]]
// CHECK: [[REG314]]
// CHECK: [[REG316:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)(i64 {{[0-9a-zA-Z_%.]+}})
// CHECK-NEXT: [[REG317:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG316]] to <8 x i16>
// CHECK-NEXT: store <8 x i16> [[REG317]], <8 x i16>* [[REG318:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG319:[0-9a-zA-Z_%.]+]] = load i64, i64* {{[0-9a-zA-Z_%.]+}}, align 8
// CHECK-NEXT: [[REG320:[0-9a-zA-Z_%.]+]] = trunc i64 [[REG319]] to i16
// CHECK-NEXT: [[REG321:[0-9a-zA-Z_%.]+]] = call <8 x i16> @vec_splats(unsigned short)(i16 zeroext [[REG320]])
// CHECK-NEXT: store <8 x i16> [[REG321]], <8 x i16>* [[REG322:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG323:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG318]], align 16
// CHECK-NEXT: [[REG324:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG322]], align 16
// CHECK-NEXT: call <8 x i16> @vec_sra(short vector[8], unsigned short vector[8])(<8 x i16> [[REG323]], <8 x i16> [[REG324]])
// CHECK: store i64 {{[0-9a-zA-Z_%.]+}}, i64* [[REG325:[0-9a-zA-Z_%.]+]], align 8
// CHECK-NEXT: br label %[[REG326:[0-9a-zA-Z_%.]+]]
// CHECK: [[REG315]]
// CHECK-NEXT: store i64 0, i64* [[REG325]], align 8
// CHECK-NEXT: br label %[[REG326]]
// CHECK: [[REG326]]
// CHECK-NEXT: [[REG327:[0-9a-zA-Z_%.]+]] = load i64, i64* [[REG325]], align 8
// CHECK-NEXT: ret i64 [[REG327]]

// CHECK: define available_externally i64 @_mm_srai_pi32
// CHECK: [[REG328:[0-9a-zA-Z_%.]+]] = sext i32 {{[0-9a-zA-Z_%.]+}} to i64
// CHECK-NEXT: [[REG329:[0-9a-zA-Z_%.]+]] = call i64 @_mm_sra_pi32(i64 {{[0-9a-zA-Z_%.]+}}, i64 [[REG328]])
// CHECK-NEXT: ret i64 [[REG329]]

// CHECK: define available_externally i64 @_mm_srai_pi16
// CHECK: [[REG330:[0-9a-zA-Z_%.]+]] = sext i32 {{[0-9a-zA-Z_%.]+}} to i64
// CHECK-NEXT: [[REG331:[0-9a-zA-Z_%.]+]] = call i64 @_mm_sra_pi16(i64 {{[0-9a-zA-Z_%.]+}}, i64 [[REG330]])
// CHECK-NEXT: ret i64 [[REG331]]

void __attribute__((noinline))
test_alt_name_sra() {
  res = _m_psrad(m1, m2);
  res = _m_psraw(m1, m2);
  res = _m_psradi(m1, i1);
  res = _m_psrawi(m1, i1);
}

// CHECK-LABEL: @test_alt_name_sra

// CHECK: define available_externally i64 @_m_psrad
// CHECK: [[REG332:[0-9a-zA-Z_%.]+]] = call i64 @_mm_sra_pi32
// CHECK-NEXT: ret i64 [[REG332]]

// CHECK: define available_externally i64 @_m_psraw
// CHECK: [[REG333:[0-9a-zA-Z_%.]+]] = call i64 @_mm_sra_pi16
// CHECK-NEXT: ret i64 [[REG333]]

// CHECK: define available_externally i64 @_m_psradi
// CHECK: [[REG334:[0-9a-zA-Z_%.]+]] = call i64 @_mm_srai_pi32
// CHECK-NEXT: ret i64 [[REG334]]

// CHECK: define available_externally i64 @_m_psrawi
// CHECK: [[REG335:[0-9a-zA-Z_%.]+]] = call i64 @_mm_srai_pi16
// CHECK-NEXT: ret i64 [[REG335]]

void __attribute__((noinline))
test_srl() {
  res = _mm_srl_si64(m1, m2);
  res = _mm_srl_pi32(m1, m2);
  res = _mm_srl_pi16(m1, m2);
  res = _mm_srli_si64(m1, i1);
  res = _mm_srli_pi32(m1, i1);
  res = _mm_srli_pi16(m1, i1);
}

// CHECK-LABEL: @test_srl

// CHECK: define available_externally i64 @_mm_srl_si64
// CHECK: [[REG336:[0-9a-zA-Z_%.]+]] = lshr i64 {{[0-9a-zA-Z_%.]+}}, {{[0-9a-zA-Z_%.]+}}
// CHECK-NEXT: ret i64 [[REG336]]

// CHECK: define available_externally i64 @_mm_srl_pi32
// CHECK: [[REG337:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 0
// CHECK-NEXT: [[REG338:[0-9a-zA-Z_%.]+]] = load i32, i32* [[REG337]], align 8
// CHECK-NEXT: [[REG339:[0-9a-zA-Z_%.]+]] = load i64, i64* {{[0-9a-zA-Z_%.]+}}, align 8
// CHECK-NEXT: [[REG340:[0-9a-zA-Z_%.]+]] = trunc i64 [[REG339]] to i32
// CHECK-NEXT: [[REG341:[0-9a-zA-Z_%.]+]] = lshr i32 [[REG338]], [[REG340]]
// CHECK: [[REG342:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 0
// CHECK-NEXT: store i32 [[REG341]], i32* [[REG342]], align 8
// CHECK: [[REG343:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 1
// CHECK-NEXT: [[REG344:[0-9a-zA-Z_%.]+]] = load i32, i32* [[REG343]], align 4
// CHECK-NEXT: [[REG345:[0-9a-zA-Z_%.]+]] = load i64, i64* {{[0-9a-zA-Z_%.]+}}, align 8
// CHECK-NEXT: [[REG346:[0-9a-zA-Z_%.]+]] = trunc i64 [[REG345]] to i32
// CHECK-NEXT: [[REG347:[0-9a-zA-Z_%.]+]] = lshr i32 [[REG344]], [[REG346]]
// CHECK: [[REG348:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 1
// CHECK-NEXT: store i32 [[REG347]], i32* [[REG348]], align 4

// CHECK: define available_externally i64 @_mm_srl_pi16
// CHECK: [[REG349:[0-9a-zA-Z_%.]+]] = icmp ule i64 {{[0-9a-zA-Z_%.]+}}, 15
// CHECK-NEXT: br i1 [[REG349]], label %[[REG350:[0-9a-zA-Z_%.]+]], label %[[REG351:[0-9a-zA-Z_%.]+]]
// CHECK: [[REG350]]
// CHECK: [[REG352:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)(i64 {{[0-9a-zA-Z_%.]+}})
// CHECK-NEXT: [[REG353:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG352]] to <8 x i16>
// CHECK-NEXT: store <8 x i16> [[REG353]], <8 x i16>* [[REG354:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG355:[0-9a-zA-Z_%.]+]] = load i64, i64* {{[0-9a-zA-Z_%.]+}}, align 8
// CHECK-NEXT: [[REG356:[0-9a-zA-Z_%.]+]] = trunc i64 [[REG355]] to i16
// CHECK-NEXT: [[REG357:[0-9a-zA-Z_%.]+]] = call <8 x i16> @vec_splats(unsigned short)(i16 zeroext [[REG356]])
// CHECK-NEXT: store <8 x i16> [[REG357]], <8 x i16>* [[REG358:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG359:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG354]], align 16
// CHECK-NEXT: [[REG360:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG358]], align 16
// CHECK-NEXT: call <8 x i16> @vec_sr(unsigned short vector[8], unsigned short vector[8])(<8 x i16> [[REG359]], <8 x i16> [[REG360]])
// CHECK: store i64 {{[0-9a-zA-Z_%.]+}}, i64* [[REG361:[0-9a-zA-Z_%.]+]], align 8
// CHECK-NEXT: br label %[[REG362:[0-9a-zA-Z_%.]+]]
// CHECK: [[REG351]]
// CHECK-NEXT: store i64 0, i64* {{[0-9a-zA-Z_%.]+}}, align 8
// CHECK-NEXT: br label %[[REG362]]
// CHECK: [[REG362]]
// CHECK-NEXT: [[REG363:[0-9a-zA-Z_%.]+]] = load i64, i64* [[REG361]], align 8
// CHECK-NEXT: ret i64 [[REG363]]

// CHECK: define available_externally i64 @_mm_srli_si64
// CHECK: [[REG364:[0-9a-zA-Z_%.]+]] = zext i32 {{[0-9a-zA-Z_%.]+}} to i64
// CHECK-NEXT: [[REG365:[0-9a-zA-Z_%.]+]] = lshr i64 {{[0-9a-zA-Z_%.]+}}, [[REG364]]
// CHECK-NEXT: ret i64 [[REG365]]

// CHECK: define available_externally i64 @_mm_srli_pi32
// CHECK: [[REG366:[0-9a-zA-Z_%.]+]] = sext i32 {{[0-9a-zA-Z_%.]+}} to i64
// CHECK-NEXT: [[REG367:[0-9a-zA-Z_%.]+]] = call i64 @_mm_srl_pi32(i64 {{[0-9a-zA-Z_%.]+}}, i64 [[REG366]])
// CHECK-NEXT: ret i64 [[REG367]]

// CHECK: define available_externally i64 @_mm_srli_pi16
// CHECK: [[REG366:[0-9a-zA-Z_%.]+]] = sext i32 {{[0-9a-zA-Z_%.]+}} to i64
// CHECK-NEXT: [[REG368:[0-9a-zA-Z_%.]+]] = call i64 @_mm_srl_pi16(i64 {{[0-9a-zA-Z_%.]+}}, i64 [[REG366]])
// CHECK-NEXT: ret i64 [[REG368]]

void __attribute__((noinline))
test_alt_name_srl() {
  res = _m_psrlq(m1, m2);
  res = _m_psrld(m1, m2);
  res = _m_psrlw(m1, m2);
  res = _m_psrlqi(m1, i1);
  res = _m_psrldi(m1, i1);
  res = _m_psrlwi(m1, i1);
}

// CHECK-LABEL: @test_alt_name_srl

// CHECK: define available_externally i64 @_m_psrlq
// CHECK: [[REG369:[0-9a-zA-Z_%.]+]] = call i64 @_mm_srl_si64
// CHECK-NEXT: ret i64 [[REG369]]

// CHECK: define available_externally i64 @_m_psrld
// CHECK: [[REG370:[0-9a-zA-Z_%.]+]] = call i64 @_mm_srl_pi32
// CHECK-NEXT: ret i64 [[REG370]]

// CHECK: define available_externally i64 @_m_psrlw
// CHECK: [[REG371:[0-9a-zA-Z_%.]+]] = call i64 @_mm_srl_pi16
// CHECK-NEXT: ret i64 [[REG371]]

// CHECK: define available_externally i64 @_m_psrlqi
// CHECK: [[REG372:[0-9a-zA-Z_%.]+]] = call i64 @_mm_srli_si64
// CHECK-NEXT: ret i64 [[REG372]]

// CHECK: define available_externally i64 @_m_psrldi
// CHECK: [[REG373:[0-9a-zA-Z_%.]+]] = call i64 @_mm_srli_pi32
// CHECK-NEXT: ret i64 [[REG373]]

// CHECK: define available_externally i64 @_m_psrlwi
// CHECK: [[REG374:[0-9a-zA-Z_%.]+]] = call i64 @_mm_srli_pi16
// CHECK-NEXT: ret i64 [[REG374]]

void __attribute__((noinline))
test_sub() {
  res = _mm_sub_pi32(m1, m2);
  res = _mm_sub_pi16(m1, m2);
  res = _mm_sub_pi8(m1, m2);
  res = _mm_subs_pi16(m1, m2);
  res = _mm_subs_pi8(m1, m2);
  res = _mm_subs_pu16(m1, m2);
  res = _mm_subs_pu8(m1, m2);
}

// CHECK-LABEL: @test_sub

// CHECK: define available_externally i64 @_mm_sub_pi32

// CHECK-P8: [[REG375:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 0
// CHECK-P8-NEXT: [[REG376:[0-9a-zA-Z_%.]+]] = load i32, i32* [[REG375]], align 8
// CHECK-P8: [[REG377:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 0
// CHECK-P8-NEXT: [[REG378:[0-9a-zA-Z_%.]+]] = load i32, i32* [[REG377]], align 8
// CHECK-P8-NEXT: sub nsw i32 [[REG376]], [[REG378]]
// CHECK-P8: [[REG379:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 1
// CHECK-P8-NEXT: [[REG380:[0-9a-zA-Z_%.]+]] = load i32, i32* [[REG379]], align 4
// CHECK-P8: [[REG381:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 1
// CHECK-P8-NEXT: [[REG382:[0-9a-zA-Z_%.]+]] = load i32, i32* [[REG381]], align 4
// CHECK-P8-NEXT: sub nsw i32 [[REG380]], [[REG382]]

// CHECK-P9: [[REG383:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)(i64 {{[0-9a-zA-Z_%.]+}})
// CHECK-P9-NEXT: [[REG384:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG383]] to <4 x i32>
// CHECK-P9-NEXT: store <4 x i32> [[REG384]], <4 x i32>* [[REG385:[0-9a-zA-Z_%.]+]], align 16
// CHECK-P9: [[REG386:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)(i64 {{[0-9a-zA-Z_%.]+}})
// CHECK-P9-NEXT: [[REG387:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG386]] to <4 x i32>
// CHECK-P9-NEXT: store <4 x i32> [[REG387]], <4 x i32>* [[REG388:[0-9a-zA-Z_%.]+]], align 16
// CHECK-P9-NEXT: [[REG389:[0-9a-zA-Z_%.]+]] = load <4 x i32>, <4 x i32>* [[REG385]], align 16
// CHECK-P9-NEXT: [[REG390:[0-9a-zA-Z_%.]+]] = load <4 x i32>, <4 x i32>* [[REG388]], align 16
// CHECK-P9-NEXT: call <4 x i32> @vec_sub(int vector[4], int vector[4])(<4 x i32> [[REG389]], <4 x i32> [[REG390]])

// CHECK: define available_externally i64 @_mm_sub_pi16
// CHECK: [[REG391:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)(i64 {{[0-9a-zA-Z_%.]+}})
// CHECK-NEXT: [[REG392:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG391]] to <8 x i16>
// CHECK-NEXT: store <8 x i16> [[REG392]], <8 x i16>* [[REG393:[0-9a-zA-Z_%.]+]], align 16
// CHECK: [[REG394:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)(i64 {{[0-9a-zA-Z_%.]+}})
// CHECK-NEXT: [[REG395:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG394]] to <8 x i16>
// CHECK-NEXT: store <8 x i16> [[REG395]], <8 x i16>* [[REG396:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG397:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG393]], align 16
// CHECK-NEXT: [[REG398:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG396]], align 16
// CHECK-NEXT: call <8 x i16> @vec_sub(short vector[8], short vector[8])(<8 x i16> [[REG397]], <8 x i16> [[REG398]])

// CHECK: define available_externally i64 @_mm_sub_pi8
// CHECK: [[REG399:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)(i64 {{[0-9a-zA-Z_%.]+}})
// CHECK-NEXT: [[REG400:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG399]] to <16 x i8>
// CHECK-NEXT: store <16 x i8> [[REG400]], <16 x i8>* [[REG401:[0-9a-zA-Z_%.]+]], align 16
// CHECK: [[REG402:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)(i64 {{[0-9a-zA-Z_%.]+}})
// CHECK-NEXT: [[REG403:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG402]] to <16 x i8>
// CHECK-NEXT: store <16 x i8> [[REG403]], <16 x i8>* [[REG404:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG405:[0-9a-zA-Z_%.]+]] = load <16 x i8>, <16 x i8>* [[REG401]], align 16
// CHECK-NEXT: [[REG406:[0-9a-zA-Z_%.]+]] = load <16 x i8>, <16 x i8>* [[REG404]], align 16
// CHECK-NEXT: call <16 x i8> @vec_sub(signed char vector[16], signed char vector[16])(<16 x i8> [[REG405]], <16 x i8> [[REG406]])

// CHECK: define available_externally i64 @_mm_subs_pi16
// CHECK: [[REG407:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)(i64 {{[0-9a-zA-Z_%.]+}})
// CHECK-NEXT: [[REG408:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG407]] to <8 x i16>
// CHECK-NEXT: store <8 x i16> [[REG408]], <8 x i16>* [[REG409:[0-9a-zA-Z_%.]+]], align 16
// CHECK: [[REG410:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)(i64 {{[0-9a-zA-Z_%.]+}})
// CHECK-NEXT: [[REG411:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG410]] to <8 x i16>
// CHECK-NEXT: store <8 x i16> [[REG411]], <8 x i16>* [[REG412:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG413:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG409]], align 16
// CHECK-NEXT: [[REG414:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG412]], align 16
// CHECK-NEXT: call <8 x i16> @vec_subs(short vector[8], short vector[8])(<8 x i16> [[REG413]], <8 x i16> [[REG414]])

// CHECK: define available_externally i64 @_mm_subs_pi8
// CHECK: [[REG415:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)(i64 {{[0-9a-zA-Z_%.]+}})
// CHECK-NEXT: [[REG416:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG415]] to <16 x i8>
// CHECK-NEXT: store <16 x i8> [[REG416]], <16 x i8>* [[REG417:[0-9a-zA-Z_%.]+]], align 16
// CHECK: [[REG418:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)(i64 {{[0-9a-zA-Z_%.]+}})
// CHECK-NEXT: [[REG419:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG418]] to <16 x i8>
// CHECK-NEXT: store <16 x i8> [[REG419]], <16 x i8>* [[REG420:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG421:[0-9a-zA-Z_%.]+]] = load <16 x i8>, <16 x i8>* [[REG417]], align 16
// CHECK-NEXT: [[REG422:[0-9a-zA-Z_%.]+]] = load <16 x i8>, <16 x i8>* [[REG420]], align 16
// CHECK-NEXT: call <16 x i8> @vec_subs(signed char vector[16], signed char vector[16])(<16 x i8> [[REG421]], <16 x i8> [[REG422]])

// CHECK: define available_externally i64 @_mm_subs_pu16
// CHECK: [[REG423:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)(i64 {{[0-9a-zA-Z_%.]+}})
// CHECK-NEXT: [[REG424:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG423]] to <8 x i16>
// CHECK-NEXT: store <8 x i16> [[REG424]], <8 x i16>* [[REG425:[0-9a-zA-Z_%.]+]], align 16
// CHECK: [[REG426:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)(i64 {{[0-9a-zA-Z_%.]+}})
// CHECK-NEXT: [[REG427:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG426]] to <8 x i16>
// CHECK-NEXT: store <8 x i16> [[REG427]], <8 x i16>* [[REG428:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG429:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG425]], align 16
// CHECK-NEXT: [[REG430:[0-9a-zA-Z_%.]+]] = load <8 x i16>, <8 x i16>* [[REG428]], align 16
// CHECK-NEXT: call <8 x i16> @vec_subs(unsigned short vector[8], unsigned short vector[8])(<8 x i16> [[REG429]], <8 x i16> [[REG430]])

// CHECK: define available_externally i64 @_mm_subs_pu8
// CHECK: [[REG431:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)(i64 {{[0-9a-zA-Z_%.]+}})
// CHECK-NEXT: [[REG432:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG431]] to <16 x i8>
// CHECK-NEXT: store <16 x i8> [[REG432]], <16 x i8>* [[REG433:[0-9a-zA-Z_%.]+]], align 16
// CHECK: [[REG434:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)(i64 {{[0-9a-zA-Z_%.]+}})
// CHECK-NEXT: [[REG435:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG434]] to <16 x i8>
// CHECK-NEXT: store <16 x i8> [[REG435]], <16 x i8>* [[REG436:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG437:[0-9a-zA-Z_%.]+]] = load <16 x i8>, <16 x i8>* [[REG433]], align 16
// CHECK-NEXT: [[REG438:[0-9a-zA-Z_%.]+]] = load <16 x i8>, <16 x i8>* [[REG436]], align 16
// CHECK-NEXT: call <16 x i8> @vec_subs(unsigned char vector[16], unsigned char vector[16])(<16 x i8> [[REG437]], <16 x i8> [[REG438]])

void __attribute__((noinline))
test_alt_name_sub() {
  res = _m_psubd(m1, m2);
  res = _m_psubw(m1, m2);
  res = _m_psubb(m1, m2);
  res = _m_psubsw(m1, m2);
  res = _m_psubsb(m1, m2);
  res = _m_psubusw(m1, m2);
  res = _m_psubusb(m1, m2);
}

// CHECK-LABEL: @test_alt_name_sub

// CHECK: define available_externally i64 @_m_psubd
// CHECK: [[REG439:[0-9a-zA-Z_%.]+]] = call i64 @_mm_sub_pi32
// CHECK-NEXT: ret i64 [[REG439]]

// CHECK: define available_externally i64 @_m_psubw
// CHECK: [[REG440:[0-9a-zA-Z_%.]+]] = call i64 @_mm_sub_pi16
// CHECK-NEXT: ret i64 [[REG440]]

// CHECK: define available_externally i64 @_m_psubb
// CHECK: [[REG441:[0-9a-zA-Z_%.]+]] = call i64 @_mm_sub_pi8
// CHECK-NEXT: ret i64 [[REG441]]

// CHECK: define available_externally i64 @_m_psubsw
// CHECK: [[REG442:[0-9a-zA-Z_%.]+]] = call i64 @_mm_subs_pi16
// CHECK-NEXT: ret i64 [[REG442]]

// CHECK: define available_externally i64 @_m_psubsb
// CHECK: [[REG443:[0-9a-zA-Z_%.]+]] = call i64 @_mm_subs_pi8
// CHECK-NEXT: ret i64 [[REG443]]

// CHECK: define available_externally i64 @_m_psubusw
// CHECK: [[REG444:[0-9a-zA-Z_%.]+]] = call i64 @_mm_subs_pu16
// CHECK-NEXT: ret i64 [[REG444]]

// CHECK: define available_externally i64 @_m_psubusb
// CHECK: [[REG445:[0-9a-zA-Z_%.]+]] = call i64 @_mm_subs_pu8
// CHECK-NEXT: ret i64 [[REG445]]

void __attribute__((noinline))
test_unpack() {
  res = _mm_unpackhi_pi32(m1, m2);
  res = _mm_unpackhi_pi16(m1, m2);
  res = _mm_unpackhi_pi8(m1, m2);
  res = _mm_unpacklo_pi32(m1, m2);
  res = _mm_unpacklo_pi16(m1, m2);
  res = _mm_unpacklo_pi8(m1, m2);
}

// CHECK-LABEL: @test_unpack

// CHECK: define available_externally i64 @_mm_unpackhi_pi32
// CHECK: [[REG446:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 1
// CHECK-NEXT: [[REG447:[0-9a-zA-Z_%.]+]] = load i32, i32* [[REG446]], align 4
// CHECK-NEXT: [[REG448:[0-9a-zA-Z_%.]+]] = bitcast {{[0-9a-zA-Z_%.]+}}* [[REG449:[0-9a-zA-Z_%.]+]] to [2 x i32]*
// CHECK-NEXT: [[REG450:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* [[REG448]], i64 0, i64 0
// CHECK-NEXT: store i32 [[REG447]], i32* [[REG450]], align 8
// CHECK: [[REG451:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 1
// CHECK-NEXT: [[REG452:[0-9a-zA-Z_%.]+]] = load i32, i32* [[REG451]], align 4
// CHECK-NEXT: [[REG453:[0-9a-zA-Z_%.]+]] = bitcast {{[0-9a-zA-Z_%.]+}}* [[REG449]] to [2 x i32]*
// CHECK-NEXT: [[REG454:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* [[REG453]], i64 0, i64 1
// CHECK-NEXT: store i32 [[REG452]], i32* [[REG454]], align 4

// CHECK: define available_externally i64 @_mm_unpackhi_pi16
// CHECK: [[REG455:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 2
// CHECK-NEXT: [[REG456:[0-9a-zA-Z_%.]+]] = load i16, i16* [[REG455]], align 4
// CHECK: [[REG457:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 0
// CHECK-NEXT: store i16 [[REG456]], i16* [[REG457]], align 8
// CHECK: [[REG458:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 2
// CHECK-NEXT: [[REG459:[0-9a-zA-Z_%.]+]] = load i16, i16* [[REG458]], align 4
// CHECK: [[REG460:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 1
// CHECK-NEXT: store i16 [[REG459]], i16* [[REG460]], align 2
// CHECK: [[REG461:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 3
// CHECK-NEXT: [[REG462:[0-9a-zA-Z_%.]+]] = load i16, i16* [[REG461]], align 2
// CHECK: [[REG463:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 2
// CHECK-NEXT: store i16 [[REG462]], i16* [[REG463]], align 4
// CHECK: [[REG464:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 3
// CHECK-NEXT: [[REG465:[0-9a-zA-Z_%.]+]] = load i16, i16* [[REG464]], align 2
// CHECK: [[REG466:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 3
// CHECK-NEXT: store i16 [[REG465]], i16* [[REG466]], align 2

// CHECK: define available_externally i64 @_mm_unpackhi_pi8
// CHECK: [[REG467:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)
// CHECK-NEXT: [[REG468:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG467]] to <16 x i8>
// CHECK-NEXT: store <16 x i8> [[REG468]], <16 x i8>* [[REG469:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG470:[0-9a-zA-Z_%.]+]] = load i64, i64* {{[0-9a-zA-Z_%.]+}}, align 8
// CHECK-NEXT: [[REG471:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)
// CHECK-NEXT: [[REG472:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG471]] to <16 x i8>
// CHECK-NEXT: store <16 x i8> [[REG472]], <16 x i8>* [[REG473:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG474:[0-9a-zA-Z_%.]+]] = load <16 x i8>, <16 x i8>* [[REG469]], align 16
// CHECK-NEXT: [[REG475:[0-9a-zA-Z_%.]+]] = load <16 x i8>, <16 x i8>* [[REG473]], align 16
// CHECK-NEXT: call <16 x i8> @vec_mergel(unsigned char vector[16], unsigned char vector[16])(<16 x i8> [[REG474]], <16 x i8> [[REG475]])

// CHECK: define available_externally i64 @_mm_unpacklo_pi32
// CHECK: [[REG476:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 0
// CHECK-NEXT: [[REG477:[0-9a-zA-Z_%.]+]] = load i32, i32* [[REG476]], align 8
// CHECK: [[REG478:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 0
// CHECK-NEXT: store i32 [[REG477]], i32* [[REG478]], align 8
// CHECK: [[REG479:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 0
// CHECK-NEXT: [[REG480:[0-9a-zA-Z_%.]+]] = load i32, i32* [[REG479]], align 8
// CHECK: [[REG481:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [2 x i32], [2 x i32]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 1
// CHECK-NEXT: store i32 [[REG480]], i32* [[REG481]], align 4

// CHECK: define available_externally i64 @_mm_unpacklo_pi16
// CHECK: [[REG482:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 0
// CHECK-NEXT: [[REG483:[0-9a-zA-Z_%.]+]] = load i16, i16* [[REG482]], align 8
// CHECK: [[REG484:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 0
// CHECK-NEXT: store i16 [[REG483]], i16* [[REG484]], align 8
// CHECK: [[REG485:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 0
// CHECK-NEXT: [[REG486:[0-9a-zA-Z_%.]+]] = load i16, i16* [[REG485]], align 8
// CHECK: [[REG487:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 1
// CHECK-NEXT: store i16 [[REG486]], i16* [[REG487]], align 2
// CHECK: [[REG488:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 1
// CHECK-NEXT: [[REG489:[0-9a-zA-Z_%.]+]] = load i16, i16* [[REG488]], align 2
// CHECK: [[REG490:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 2
// CHECK-NEXT: store i16 [[REG489]], i16* [[REG490]], align 4
// CHECK: [[REG491:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 1
// CHECK-NEXT: [[REG492:[0-9a-zA-Z_%.]+]] = load i16, i16* [[REG491]], align 2
// CHECK: [[REG493:[0-9a-zA-Z_%.]+]] = getelementptr inbounds [4 x i16], [4 x i16]* {{[0-9a-zA-Z_%.]+}}, i64 0, i64 3
// CHECK-NEXT: store i16 [[REG492]], i16* [[REG493]], align 2

// CHECK: define available_externally i64 @_mm_unpacklo_pi8
// CHECK: [[REG494:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)
// CHECK-NEXT: [[REG495:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG494]] to <16 x i8>
// CHECK-NEXT: store <16 x i8> [[REG495]], <16 x i8>* [[REG496:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG497:[0-9a-zA-Z_%.]+]] = load i64, i64* {{[0-9a-zA-Z_%.]+}}, align 8
// CHECK-NEXT: [[REG498:[0-9a-zA-Z_%.]+]] = call <2 x i64> @vec_splats(unsigned long long)
// CHECK-NEXT: [[REG499:[0-9a-zA-Z_%.]+]] = bitcast <2 x i64> [[REG498]] to <16 x i8>
// CHECK-NEXT: store <16 x i8> [[REG499]], <16 x i8>* [[REG500:[0-9a-zA-Z_%.]+]], align 16
// CHECK-NEXT: [[REG501:[0-9a-zA-Z_%.]+]] = load <16 x i8>, <16 x i8>* [[REG496]], align 16
// CHECK-NEXT: [[REG502:[0-9a-zA-Z_%.]+]] = load <16 x i8>, <16 x i8>* [[REG500]], align 16
// CHECK-NEXT: [[REG503:[0-9a-zA-Z_%.]+]] = call <16 x i8> @vec_mergel(unsigned char vector[16], unsigned char vector[16])

void __attribute__((noinline))
test_alt_name_unpack() {
  res = _m_punpckhdq(m1, m2);
  res = _m_punpckhwd(m1, m2);
  res = _m_punpckhbw(m1, m2);
  res = _m_punpckldq(m1, m2);
  res = _m_punpcklwd(m1, m2);
  res = _m_punpcklbw(m1, m2);
}

// CHECK-LABEL: @test_alt_name_unpack

// CHECK: define available_externally i64 @_m_punpckhdq
// CHECK: [[REG238:[0-9a-zA-Z_%.]+]] = call i64 @_mm_unpackhi_pi32
// CHECK-NEXT: ret i64 [[REG238]]

// CHECK: define available_externally i64 @_m_punpckhwd
// CHECK: [[REG239:[0-9a-zA-Z_%.]+]] = call i64 @_mm_unpackhi_pi16
// CHECK-NEXT: ret i64 [[REG239]]

// CHECK: define available_externally i64 @_m_punpckhbw
// CHECK: [[REG240:[0-9a-zA-Z_%.]+]] = call i64 @_mm_unpackhi_pi8
// CHECK-NEXT: ret i64 [[REG240]]

// CHECK: define available_externally i64 @_m_punpckldq
// CHECK: [[REG241:[0-9a-zA-Z_%.]+]] = call i64 @_mm_unpacklo_pi32
// CHECK-NEXT: ret i64 [[REG241]]

// CHECK: define available_externally i64 @_m_punpcklwd
// CHECK: [[REG242:[0-9a-zA-Z_%.]+]] = call i64 @_mm_unpacklo_pi16
// CHECK-NEXT: ret i64 [[REG242]]

// CHECK: define available_externally i64 @_m_punpcklbw
// CHECK: [[REG243:[0-9a-zA-Z_%.]+]] = call i64 @_mm_unpacklo_pi8
// CHECK-NEXT: ret i64 [[REG243]]
