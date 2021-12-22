#include "Environment/RuntimeContext.hpp"
#include "Environment/ProgramModule.hpp"
#include "Environment/LanguageSupport.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/SSAValue.hpp"

#include "UnitTest++/UnitTest++.h"
#include <iostream>

using namespace Sysmel::Environment;

static AnyValuePtr evaluateString(const std::string &source)
{
    return RuntimeContext::getActive()->getSysmelLanguageSupport()->evaluateSourceStringNamed(source, "unit-test");
}

static SExpression evaluateStringForSSA(const std::string &source)
{
    return stripSExpressionFromPositions(evaluateString(source)->asSSAValueRequiredInPosition(ASTSourcePosition::empty())->asFullDefinitionSExpression());
}

bool checkStringEvaluationSSAIsSameTo(const std::string &source, const std::string &expectedSSA)
{
    auto sexpression = evaluateStringForSSA(source);
    auto sexpressionPretty = sexpressionToPrettyString(sexpression);
    auto sexpressionCanonical = sexpressionCanonicalizeString(sexpressionPretty);
    auto expectedCanonical = sexpressionCanonicalizeString(expectedSSA);
    if(expectedCanonical == sexpressionCanonical)
        return true;

    std::cerr << "Expected SSA:\n" << expectedSSA << "\nGotten SSA:\n" << sexpressionPretty << std::endl;
    //std::cerr << "Expected SSA:\n" << expectedCanonical << "\nGotten SSA:\n" << sexpressionCanonical << std::endl;
    return false;
}

SUITE(SSATests)
{
    TEST(PrimitiveNumberConstants)
    {
        RuntimeContext::createForTarget(RuntimeContextTargetDescription::makeForCPU64())->activeDuring([&](){
            ProgramModule::create("Test")->activeDuring([&](){
                CHECK(checkStringEvaluationSSAIsSameTo("public function oneInt32() => Int32 := 1.",
R"(
(function #oneInt32 nil
(functionType () "Int32")
(region () () "Int32" ((basicBlock 0 (
    (doWithCleanUp 0 "Void"
    (region nil () () "Void" ((basicBlock 1 ((returnFromFunction 1 "Void"
        (constantLiteralValue (int32 1) "Int32"))))))
    (region nil () () "Void" ((basicBlock 2 ((returnFromRegion 2 "Void"
        (constantLiteralValue void "Void")))))))
    (unreachable 3 "Void"))))))
)"));
            });
        });
    }

    TEST(ControlFlow)
    {
        RuntimeContext::createForTarget(RuntimeContextTargetDescription::makeForCPU64())->activeDuring([&](){
            ProgramModule::create("Test")->activeDuring([&](){
                CHECK(checkStringEvaluationSSAIsSameTo(
R"(
    public function min(a: Int32, b: Int32) => Int32 := {
        if: a < b then: a else: b
    }
)",
R"(
(function #min nil
  (functionType ("Int32" "Int32") "Int32")
  (region (
    (argument 0 "Int32")
    (argument 1 "Int32")) () "Int32" ((basicBlock 0 (
    (doWithCleanUp 2 "Void"
      (region nil () () "Void" ((basicBlock 1
        (
          (call 3 "Boolean8"
            (function #< #"integer.less-than.signed")
            (local 0)
            (local 1))
          (if 4 "Int32"
            (local 3)
            (region nil () () "Int32" ((basicBlock 2 ((returnFromRegion 5 "Void" (local 0))))))
            (region nil () () "Int32" ((basicBlock 3 ((returnFromRegion 6 "Void" (local 1)))))))
          (returnFromFunction 7 "Void" (local 4))))))
      (region nil () () "Void" ((basicBlock 4 ((returnFromRegion 8 "Void"
        (constantLiteralValue void "Void")))))))
    (unreachable 9 "Void")))))))"));

CHECK(checkStringEvaluationSSAIsSameTo(
R"(
public function sumNaturalsInt32(n: Int32) => Int32 := {
    let sum mutable := Int32(0).
    let i mutable := Int32(0).
    while: (i < n) do: {
        sum := sum + i.
    } continueWith: (i := i + 1).
    sum.
}.
)",
R"(
(function #sumNaturalsInt32 nil
  (functionType ("Int32") "Int32")
  (region ((argument 0 "Int32")) () "Int32" ((basicBlock 0 (
    (doWithCleanUp 1 "Void"
      (region nil () () "Void" ((basicBlock 1
        (
          (localVariable 2
            (refType "Int32" #generic))
          (store 3 "Void"
            (constantLiteralValue (int32 0) "Int32")
            (local 2))
          (localVariable 4
            (refType "Int32" #generic))
          (store 5 "Void"
            (constantLiteralValue (int32 0) "Int32")
            (local 4))
          (while 6 "Void"
            (region nil () () "Boolean8" ((basicBlock 2
              (
                (call 7 "Int32"
                  (function #conv #"reference.load")
                  (local 4))
                (call 8 "Boolean8"
                  (function #< #"integer.less-than.signed")
                  (local 7)
                  (local 0))
                (returnFromRegion 9 "Void" (local 8))))))
            (region nil () () "Void" ((basicBlock 3 (
              (doWithCleanUp 10 "Void"
                (region nil () () "Void" ((basicBlock 4
                  (
                    (call 11 "Int32"
                      (function #conv #"reference.load")
                      (local 2))
                    (call 12 "Int32"
                      (function #conv #"reference.load")
                      (local 4))
                    (call 13 "Int32"
                      (function #+ #"integer.add")
                      (local 11)
                      (local 12))
                    (call 14
                      (refType "Int32" #generic)
                      (function #":=" #"reference.copy.assignment.trivial")
                      (local 2)
                      (local 13))
                    (returnFromRegion 15 "Void"
                      (constantLiteralValue void "Void"))))))
                (region nil () () "Void" ((basicBlock 5 ((returnFromRegion 16 "Void"
                  (constantLiteralValue void "Void")))))))
              (returnFromRegion 17 "Void"
                (constantLiteralValue void "Void"))))))
            (region nil () () "Void" ((basicBlock 6
              (
                (call 18 "Int32"
                  (function #conv #"reference.load")
                  (local 4))
                (call 19 "Int32"
                  (function #+ #"integer.add")
                  (local 18)
                  (constantLiteralValue (int32 1) "Int32"))
                (call 20
                  (refType "Int32" #generic)
                  (function #":=" #"reference.copy.assignment.trivial")
                  (local 4)
                  (local 19))
                (returnFromRegion 21 "Void"
                  (constantLiteralValue void "Void")))))))
          (call 22 "Int32"
            (function #conv #"reference.load")
            (local 2))
          (returnFromFunction 23 "Void" (local 22))))))
      (region nil () () "Void" ((basicBlock 7 ((returnFromRegion 24 "Void"
        (constantLiteralValue void "Void")))))))
    (unreachable 25 "Void"))))))
)"));

CHECK(checkStringEvaluationSSAIsSameTo(
R"(
public function sumNaturalsInt32DoWhile(n: Int32) => Int32 := {
    let sum mutable := Int32(0).
    let i mutable := Int32(0).
    do: {
        sum := sum + i.
    } while: (i < n) continueWith: (i := i + 1).
    
    sum.
}.
)",
R"(
(function #sumNaturalsInt32DoWhile nil
  (functionType ("Int32") "Int32")
  (region ((argument 0 "Int32")) () "Int32" ((basicBlock 0 (
    (doWithCleanUp 1 "Void"
      (region nil () () "Void" ((basicBlock 1
        (
          (localVariable 2
            (refType "Int32" #generic))
          (store 3 "Void"
            (constantLiteralValue (int32 0) "Int32")
            (local 2))
          (localVariable 4
            (refType "Int32" #generic))
          (store 5 "Void"
            (constantLiteralValue (int32 0) "Int32")
            (local 4))
          (doWhile 6 "Void"
            (region nil () () "Void" ((basicBlock 2 (
              (doWithCleanUp 7 "Void"
                (region nil () () "Void" ((basicBlock 3
                  (
                    (call 8 "Int32"
                      (function #conv #"reference.load")
                      (local 2))
                    (call 9 "Int32"
                      (function #conv #"reference.load")
                      (local 4))
                    (call 10 "Int32"
                      (function #+ #"integer.add")
                      (local 8)
                      (local 9))
                    (call 11
                      (refType "Int32" #generic)
                      (function #":=" #"reference.copy.assignment.trivial")
                      (local 2)
                      (local 10))
                    (returnFromRegion 12 "Void"
                      (constantLiteralValue void "Void"))))))
                (region nil () () "Void" ((basicBlock 4 ((returnFromRegion 13 "Void"
                  (constantLiteralValue void "Void")))))))
              (returnFromRegion 14 "Void"
                (constantLiteralValue void "Void"))))))
            (region nil () () "Boolean8" ((basicBlock 5
              (
                (call 15 "Int32"
                  (function #conv #"reference.load")
                  (local 4))
                (call 16 "Boolean8"
                  (function #< #"integer.less-than.signed")
                  (local 15)
                  (local 0))
                (returnFromRegion 17 "Void" (local 16))))))
            (region nil () () "Void" ((basicBlock 6
              (
                (call 18 "Int32"
                  (function #conv #"reference.load")
                  (local 4))
                (call 19 "Int32"
                  (function #+ #"integer.add")
                  (local 18)
                  (constantLiteralValue (int32 1) "Int32"))
                (call 20
                  (refType "Int32" #generic)
                  (function #":=" #"reference.copy.assignment.trivial")
                  (local 4)
                  (local 19))
                (returnFromRegion 21 "Void"
                  (constantLiteralValue void "Void")))))))
          (call 22 "Int32"
            (function #conv #"reference.load")
            (local 2))
          (returnFromFunction 23 "Void" (local 22))))))
      (region nil () () "Void" ((basicBlock 7 ((returnFromRegion 24 "Void"
        (constantLiteralValue void "Void")))))))
    (unreachable 25 "Void"))))))
)"));

            });
        });
    }

    TEST(Tuples)
    {
        RuntimeContext::createForTarget(RuntimeContextTargetDescription::makeForCPU64())->activeDuring([&](){
            ProgramModule::create("Test")->activeDuring([&](){
CHECK(checkStringEvaluationSSAIsSameTo(
R"(
public function makeTupleInt32Float32(first: Int32, second: Float32) => (Int32 & Float32)
  := (first, second).
)",
R"(
(function #makeTupleInt32Float32 nil
  (functionType ("Int32" "Float32") (tupleType ("Int32" "Float32")))
  (region
    (
      (argument 0
        (tempRefType (tupleType ("Int32" "Float32")) #generic)
        nil
        nil)
      (argument 1 "Int32")
      (argument 2 "Float32"))
    ()
    "Void"
    ((basicBlock 0 (
      (doWithCleanUp 3 "Void"
        (region nil () () "Void" ((basicBlock 1
          (
            (localVariable 4
              (tempRefType (tupleType ("Int32" "Float32")) #generic))
            (getAggregateSlot 5 "Int32"
              (local 4)
              (constantLiteralValue 0 "LiteralPositiveInteger"))
            (store 6 "Void" (local 1) (local 5))
            (getAggregateSlot 7 "Float32"
              (local 4)
              (constantLiteralValue 1 "LiteralPositiveInteger"))
            (store 8 "Void" (local 2) (local 7))
            (load 9 (tupleType ("Int32" "Float32")) (local 4))
            (store 10 "Void" (local 9) (local 0))
            (returnFromFunction 11 "Void"
              (constantLiteralValue void "Void"))))))
        (region nil () () "Void" ((basicBlock 2 ((returnFromRegion 12 "Void"
          (constantLiteralValue void "Void")))))))
      (unreachable 13 "Void"))))))
)"));
            });
        });
    }

    TEST(Struct)
    {
        RuntimeContext::createForTarget(RuntimeContextTargetDescription::makeForCPU64())->activeDuring([&](){
            ProgramModule::create("Test")->activeDuring([&](){
              auto structureType = evaluateString(
R"(
  public struct TestStruct := {
      public field first type: Int8.
      public field second type: Int32.
      
      public method extractFirst => Int8
          := first.
  }.
)");

CHECK(checkStringEvaluationSSAIsSameTo(
R"(
public function getFirst(s: TestStruct) => Int8
    := s first.
)", R"(
(function #getFirst nil
  (functionType ("TestStruct") "Int8")
  (region ((argument 0
    (refType "TestStruct" #generic))) () "Int8" ((basicBlock 0 (
    (doWithCleanUp 1 "Void"
      (region nil () () "Void" ((basicBlock 1
        (
          (getAggregateField 2
            (refType "Int8" #generic)
            (local 0))
          (call 3 "Int8"
            (function #conv #"reference.load")
            (local 2))
          (returnFromFunction 4 "Void" (local 3))))))
      (region nil () () "Void" ((basicBlock 2 ((returnFromRegion 5 "Void"
        (constantLiteralValue void "Void")))))))
    (unreachable 6 "Void"))))))
)"));

CHECK(checkStringEvaluationSSAIsSameTo(
R"(
public function getSecond(s: TestStruct) => Int32
    := s second.
)", R"(
(function #getSecond nil
  (functionType ("TestStruct") "Int32")
  (region ((argument 0
    (refType "TestStruct" #generic))) () "Int32" ((basicBlock 0 (
    (doWithCleanUp 1 "Void"
      (region nil () () "Void" ((basicBlock 1
        (
          (getAggregateField 2
            (refType "Int32" #generic)
            (local 0))
          (call 3 "Int32"
            (function #conv #"reference.load")
            (local 2))
          (returnFromFunction 4 "Void" (local 3))))))
      (region nil () () "Void" ((basicBlock 2 ((returnFromRegion 5 "Void"
        (constantLiteralValue void "Void")))))))
    (unreachable 6 "Void"))))))
)"));

CHECK(checkStringEvaluationSSAIsSameTo(
R"(
public function returnStruct(first: Int8, second: Int32) => TestStruct
    := TestStruct basicNewValue
        first: first;
        second: second;
        yourself.
)", R"(
(function #returnStruct nil
  (functionType ("Int8" "Int32") "TestStruct")
  (region
    (
      (argument 0
        (tempRefType "TestStruct" #generic)
        nil
        nil)
      (argument 1 "Int8")
      (argument 2 "Int32"))
    ()
    "Void"
    ((basicBlock 0 (
      (doWithCleanUp 3 "Void"
        (region nil () () "Void" ((basicBlock 1
          (
            (localVariable 4
              (tempRefType "TestStruct" #generic))
            (store 5 "Void"
              (constantLiteralValue
                (struct "TestStruct" ((int8 0) (int32 0)))
                "TestStruct")
              (local 4))
            (localVariable 6
              (tempRefType "TestStruct" #generic))
            (load 7 "TestStruct" (local 4))
            (store 8 "Void" (local 7) (local 6))
            (getAggregateField 9
              (refType "Int8" #generic)
              (local 6))
            (call 10
              (refType "Int8" #generic)
              (function #":=" #"reference.copy.assignment.trivial")
              (local 9)
              (local 1))
            (getAggregateField 11
              (refType "Int32" #generic)
              (local 6))
            (call 12
              (refType "Int32" #generic)
              (function #":=" #"reference.copy.assignment.trivial")
              (local 11)
              (local 2))
            (load 13 "TestStruct" (local 6))
            (store 14 "Void" (local 13) (local 0))
            (returnFromFunction 15 "Void"
              (constantLiteralValue void "Void"))))))
        (region nil () () "Void" ((basicBlock 2 ((returnFromRegion 16 "Void"
          (constantLiteralValue void "Void")))))))
      (unreachable 17 "Void"))))))
)"));

CHECK(checkStringEvaluationSSAIsSameTo(
R"(
public function returnEmptyStruct(first: Int8, second: Int32) => TestStruct
    := TestStruct basicNewValue.
)", R"(
(function #returnEmptyStruct nil
  (functionType ("Int8" "Int32") "TestStruct")
  (region
    (
      (argument 0
        (tempRefType "TestStruct" #generic)
        nil
        nil)
      (argument 1 "Int8")
      (argument 2 "Int32"))
    ()
    "Void"
    ((basicBlock 0 (
      (doWithCleanUp 3 "Void"
        (region nil () () "Void" ((basicBlock 1
          (
            (localVariable 4
              (tempRefType "TestStruct" #generic))
            (store 5 "Void"
              (constantLiteralValue
                (struct "TestStruct" ((int8 0) (int32 0)))
                "TestStruct")
              (local 4))
            (load 6 "TestStruct" (local 4))
            (store 7 "Void" (local 6) (local 0))
            (returnFromFunction 8 "Void"
              (constantLiteralValue void "Void"))))))
        (region nil () () "Void" ((basicBlock 2 ((returnFromRegion 9 "Void"
          (constantLiteralValue void "Void")))))))
      (unreachable 10 "Void"))))))
)"));
            });
        });
    }

    TEST(ImplicitCleanUp)
    {
        RuntimeContext::createForTarget(RuntimeContextTargetDescription::makeForCPU64())->activeDuring([&](){
            ProgramModule::create("Test")->activeDuring([&](){
                evaluateString(
R"(
public function sideEffect() => Void.

public struct Mutex definition: {
    public method initialize => Void := {
        sideEffect()
    }.

    public method finalize => Void := {
        sideEffect()
    }.
}.

)");

CHECK(checkStringEvaluationSSAIsSameTo(
R"(
public function implicitCleanUp() => Void := {
    let m mutable type: Mutex.
}.
)",
R"(
(function #implicitCleanUp nil
  (functionType () "Void")
  (region () () "Void" ((basicBlock 0 (
    (doWithCleanUp 0 "Void"
      (region nil () () "Void" ((basicBlock 1
        (
          (localVariable 1
            (refType "Mutex" #generic))
          (store 2 "Void"
            (constantLiteralValue
              (struct "Mutex" ())
              "Mutex")
            (local 1))
          (enableLocalFinalization 3 "Void" (local 1))
          (returnFromFunction 4 "Void"
            (constantLiteralValue void "Void"))))))
      (region nil () () "Void" ((basicBlock 2 (
        (localFinalization 5 "Void"
          (local 1)
          (region nil () () "Void" ((basicBlock 3 (
            (call 6 "Void"
              (function #finalize nil)
              (local 1))
            (returnFromRegion 7 "Void"
              (constantLiteralValue void "Void")))))))
        (returnFromRegion 8 "Void"
          (constantLiteralValue void "Void")))))))
    (unreachable 9 "Void"))))))
)"));

CHECK(checkStringEvaluationSSAIsSameTo(
R"(
public function implicitCleanUp2() => Void := {
    let m type: Mutex.
}.
)",
R"(
(function #implicitCleanUp2 nil
  (functionType () "Void")
  (region () () "Void" ((basicBlock 0 (
    (doWithCleanUp 0 "Void"
      (region nil () () "Void" ((basicBlock 1
        (
          (localVariable 1
            (tempRefType "Mutex" #generic))
          (store 2 "Void"
            (constantLiteralValue
              (struct "Mutex" ())
              "Mutex")
            (local 1))
          (enableLocalFinalization 3 "Void" (local 1))
          (returnFromFunction 4 "Void"
            (constantLiteralValue void "Void"))))))
      (region nil () () "Void" ((basicBlock 2 (
        (localFinalization 5 "Void"
          (local 1)
          (region nil () () "Void" ((basicBlock 3 (
            (call 6 "Void"
              (function #finalize nil)
              (local 1))
            (returnFromRegion 7 "Void"
              (constantLiteralValue void "Void")))))))
        (returnFromRegion 8 "Void"
          (constantLiteralValue void "Void")))))))
    (unreachable 9 "Void"))))))
)"));

            });
        });
    }
}
