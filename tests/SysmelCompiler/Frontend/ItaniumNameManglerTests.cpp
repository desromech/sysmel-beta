#include "Environment/RuntimeContext.hpp"
#include "Environment/ProgramModule.hpp"
#include "Environment/LanguageSupport.hpp"
#include "Environment/Type.hpp"
#include "Environment/FunctionalType.hpp"
#include "Environment/Error.hpp"
#include "Environment/ItaniumNameMangler.hpp"

#include "UnitTest++/UnitTest++.h"

using namespace Sysmel::Environment;

static AnyValuePtr evaluateString(const std::string &source)
{
    return RuntimeContext::getActive()->getSysmelLanguageSupport()->evaluateSourceStringNamed(source, "unit-test");
}

static std::string mangleProgramEntity(const ProgramEntityPtr &programEntity)
{
    return basicMakeObject<ItaniumNameMangler> ()->mangleProgramEntity(staticObjectCast<ProgramEntity> (programEntity));
}

static std::string mangleType(const TypePtr &type)
{
    return basicMakeObject<ItaniumNameMangler> ()->mangleType(type);
}

static std::string mangleTypeFromString(const std::string &source)
{
    auto type = evaluateString(source);
    CHECK(type->isType());
    return mangleType(staticObjectCast<Type> (type));
}

static ProgramEntityPtr extractProgramEntityFrom(const AnyValuePtr &value)
{
    if(value->isProgramEntity())
        return staticObjectCast<ProgramEntity> (value);
    else if(value->isFunctionalTypeValue())
        return extractProgramEntityFrom(value.staticAs<FunctionalTypeValue> ()->functionalImplementation);
    signalNewWithMessage<Error> ("Expected a program entity.");
}

static std::string mangleProgramEntityFromString(const std::string &source)
{
    return mangleProgramEntity(extractProgramEntityFrom(evaluateString(source)));
}

SUITE(SSATests)
{
    TEST(PrimitiveTypes)
    {
        RuntimeContext::createForTarget(RuntimeContextTargetDescription::makeForCPU64())->activeDuring([&](){
            ProgramModule::create("Test")->activeDuring([&](){
              CHECK_EQUAL("v", mangleTypeFromString("Void"));
              CHECK_EQUAL("Dn", mangleTypeFromString("Undefined"));
              CHECK_EQUAL("b", mangleTypeFromString("Boolean8"));
              CHECK_EQUAL("h", mangleTypeFromString("UInt8"));
              CHECK_EQUAL("t", mangleTypeFromString("UInt16"));
              CHECK_EQUAL("j", mangleTypeFromString("UInt32"));
              CHECK_EQUAL("y", mangleTypeFromString("UInt64"));

              CHECK_EQUAL("a", mangleTypeFromString("Int8"));
              CHECK_EQUAL("s", mangleTypeFromString("Int16"));
              CHECK_EQUAL("i", mangleTypeFromString("Int32"));
              CHECK_EQUAL("x", mangleTypeFromString("Int64"));

              CHECK_EQUAL("Du", mangleTypeFromString("Char8"));
              CHECK_EQUAL("Ds", mangleTypeFromString("Char16"));
              CHECK_EQUAL("Di", mangleTypeFromString("Char32"));

              CHECK_EQUAL("Dh", mangleTypeFromString("Float16"));
              CHECK_EQUAL("f", mangleTypeFromString("Float32"));
              CHECK_EQUAL("d", mangleTypeFromString("Float64"));
          });
        });
    }

    TEST(GlobalNamespace)
    {
        RuntimeContext::createForTarget(RuntimeContextTargetDescription::makeForCPU64())->activeDuring([&](){
            ProgramModule::create("TestModule")->activeDuring([&](){
                CHECK_EQUAL("puts", mangleProgramEntityFromString("public externC function puts(string: Char8 const pointer) => Int32."));
                CHECK_EQUAL("printf", mangleProgramEntityFromString("public externC function printf(string: Char8 const pointer, params: CVarArg params) => Int32."));
                CHECK_EQUAL("_ZN6Sysmel10TestModule8myPrintfEPKDuz", mangleProgramEntityFromString("public function myPrintf(string: Char8 const pointer, params: CVarArg params) => Int32."));

                CHECK_EQUAL("_ZN6Sysmel10TestModule10sideEffectEv", mangleProgramEntityFromString("public function sideEffect() => Void := {}"));

                CHECK_EQUAL("_ZN6Sysmel10TestModule8zeroInt8Ev", mangleProgramEntityFromString("public function zeroInt8() => Int8 := 0"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule9zeroInt16Ev", mangleProgramEntityFromString("public function zeroInt16() => Int16 := 0"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule9zeroInt32Ev", mangleProgramEntityFromString("public function zeroInt32() => Int32 := 0"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule9zeroInt64Ev", mangleProgramEntityFromString("public function zeroInt64() => Int64 := 0"));

                CHECK_EQUAL("_ZN6Sysmel10TestModule9zeroUInt8Ev", mangleProgramEntityFromString("public function zeroUInt8() => UInt8 := 0"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule10zeroUInt16Ev", mangleProgramEntityFromString("public function zeroUInt16() => UInt16 := 0"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule10zeroUInt32Ev", mangleProgramEntityFromString("public function zeroUInt32() => UInt32 := 0"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule10zeroUInt64Ev", mangleProgramEntityFromString("public function zeroUInt64() => UInt64 := 0"));

                CHECK_EQUAL("_ZN6Sysmel10TestModule9zeroChar8Ev", mangleProgramEntityFromString("public function zeroChar8() => Char8 := 0"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule10zeroChar16Ev", mangleProgramEntityFromString("public function zeroChar16() => Char16 := 0"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule10zeroChar32Ev", mangleProgramEntityFromString("public function zeroChar32() => Char32 := 0"));

                //CHECK_EQUAL("", mangleProgramEntityFromString("public function zeroFloat16() => Float16 := 0"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule11zeroFloat32Ev", mangleProgramEntityFromString("public function zeroFloat32() => Float32 := 0"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule11zeroFloat64Ev", mangleProgramEntityFromString("public function zeroFloat64() => Float64 := 0"));

                CHECK_EQUAL("_ZN6Sysmel10TestModule16identityBoolean8Eb", mangleProgramEntityFromString("public function identityBoolean8(x: Boolean8) => Boolean8 := x"));

                CHECK_EQUAL("_ZN6Sysmel10TestModule12identityInt8Ea", mangleProgramEntityFromString("public function identityInt8(x: Int8) => Int8 := x"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule13identityInt16Es", mangleProgramEntityFromString("public function identityInt16(x: Int16) => Int16 := x"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule13identityInt32Ei", mangleProgramEntityFromString("public function identityInt32(x: Int32) => Int32 := x"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule13identityInt64Ex", mangleProgramEntityFromString("public function identityInt64(x: Int64) => Int64 := x"));

                CHECK_EQUAL("_ZN6Sysmel10TestModule13identityUInt8Eh", mangleProgramEntityFromString("public function identityUInt8(x: UInt8) => UInt8 := x"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule14identityUInt16Et", mangleProgramEntityFromString("public function identityUInt16(x: UInt16) => UInt16 := x"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule14identityUInt32Ej", mangleProgramEntityFromString("public function identityUInt32(x: UInt32) => UInt32 := x"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule14identityUInt64Ey", mangleProgramEntityFromString("public function identityUInt64(x: UInt64) => UInt64 := x"));

                CHECK_EQUAL("_ZN6Sysmel10TestModule13identityChar8EDu", mangleProgramEntityFromString("public function identityChar8(x: Char8) => Char8 := x"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule14identityChar16EDs", mangleProgramEntityFromString("public function identityChar16(x: Char16) => Char16 := x"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule14identityChar32EDi", mangleProgramEntityFromString("public function identityChar32(x: Char32) => Char32 := x"));

                CHECK_EQUAL("_ZN6Sysmel10TestModule15identityFloat16EDh", mangleProgramEntityFromString("public function identityFloat16(x: Float16) => Float16 := x"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule15identityFloat32Ef", mangleProgramEntityFromString("public function identityFloat32(x: Float32) => Float32 := x"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule15identityFloat64Ed", mangleProgramEntityFromString("public function identityFloat64(x: Float64) => Float64 := x"));

                CHECK_EQUAL("_ZN6Sysmel10TestModule10withArray0EA_i", mangleProgramEntityFromString("public function withArray0(array: Int32 array) => Void := {}"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule11withArray10EA10_i", mangleProgramEntityFromString("public function withArray10(array: (Int32 array: 10)) => Void := {}"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule9withTupleE5tupleIifE", mangleProgramEntityFromString("public function withTuple(pair: Int32 & Float32) => Void := {}"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule11withVariantE7variantIifE", mangleProgramEntityFromString("public function withVariant(pair: Int32 | Float32) => Void := {}"));

                CHECK_EQUAL("_ZN6Sysmel10TestModule12withFunctionEPFifE", mangleProgramEntityFromString("public function withFunction(function: ((Int32 & Float32) => Int32) pointer) => Void := {}"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule11withClosureEP7closureIifE", mangleProgramEntityFromString("public function withClosure(function: ((Int32 & Float32) => Int32) closure) => Void := {}"));
                CHECK_EQUAL("_ZN6Sysmel10TestModule14withMethodTypeEPFiifE", mangleProgramEntityFromString("public function withMethodType(function: (((Int32 & Float32) => Int32) methodWithReceiver: Int32) pointer) => Void := {}"));
            });
        });
    }

    TEST(NestedType)
    {
        RuntimeContext::createForTarget(RuntimeContextTargetDescription::makeForCPU64())->activeDuring([&](){
            ProgramModule::create("Test")->activeDuring([&](){
                auto structureType = evaluateString(
R"(
public struct TestStruct := {
    public field first type: Int8.
    
    public const method getFirst => Int8
        := first.
}.
)");
                CHECK(structureType->isStructureType());
                auto method = structureType.staticAs<Type> ()->lookupSelector(internSymbol("getFirst"));
                CHECK(method->isCompiledMethod());
                CHECK_EQUAL("_ZNK6Sysmel4Test10TestStruct8getFirstEv", mangleProgramEntity(staticObjectCast<ProgramEntity> (method)));
            });
        });
    }

    TEST(TemplateStruct)
    {
        RuntimeContext::createForTarget(RuntimeContextTargetDescription::makeForCPU64())->activeDuring([&](){
            ProgramModule::create("Test")->activeDuring([&](){
                auto structureType = evaluateString(
R"(
public template TestStruct(T: Type) := struct definition: {
    public field first type: T.
    
    public const method getFirst => T
        := first.
}.

TestStruct(Int32)
)");
                CHECK(structureType->isStructureType());
                auto method = structureType.staticAs<Type> ()->lookupSelector(internSymbol("getFirst"));
                CHECK(method->isCompiledMethod());
                CHECK_EQUAL("_ZNK6Sysmel4Test10TestStructIiE8getFirstEv", mangleProgramEntity(staticObjectCast<ProgramEntity> (method)));
            });
        });
    }
}
