#include "sysmel/BootstrapEnvironment/AnyValue.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/EnumType.hpp"
#include "sysmel/BootstrapEnvironment/StructureType.hpp"
#include "sysmel/BootstrapEnvironment/ClassType.hpp"
#include "sysmel/BootstrapEnvironment/UnionType.hpp"
#include "sysmel/BootstrapEnvironment/PrimitiveIntegerType.hpp"
#include "sysmel/BootstrapEnvironment/Wrappers.hpp"
#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapModule.hpp"
#include "sysmel/BootstrapEnvironment/ScriptModule.hpp"
#include "sysmel/BootstrapEnvironment/LanguageSupport.hpp"

#include "UnitTest++/UnitTest++.h"

using namespace SysmelMoebius::BootstrapEnvironment;

static AnyValuePtr evaluateString(const std::string &source)
{
    return RuntimeContext::getActive()->getSysmelLanguageSupport()->evaluateSourceStringNamed(source, "unit-test");
}

template<typename T>
static T evaluateStringWithValueOfType(const std::string &source)
{
    return unwrapValue<T> (evaluateString(source));
}

SUITE(SysmelCompileTimeEvaluation)
{
    TEST(Literals)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int> ("0"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int> ("1"));
                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int> ("-1"));

                CHECK_EQUAL(true, evaluateStringWithValueOfType<bool> ("true"));
                CHECK_EQUAL(false, evaluateStringWithValueOfType<bool> ("false"));
                CHECK(evaluateString("void")->isVoid());
                CHECK(evaluateString("nil")->isUndefined());
            });
        });
    }

    TEST(CompileTimeTypes)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK(evaluateString("LiteralValue")->isType());
                CHECK(evaluateString("LiteralInteger")->isType());
                CHECK(evaluateString("Undefined")->isType());
                CHECK(evaluateString("AnyValue")->isType());
                CHECK(evaluateString("Type")->isType());
                CHECK(evaluateString("Void")->isType());
            });
        });
    }

    TEST(PrimitiveTypes)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK(evaluateString("Boolean8")->isType());

                CHECK(evaluateString("UInt8")->isType());
                CHECK(evaluateString("UInt16")->isType());
                CHECK(evaluateString("UInt32")->isType());
                CHECK(evaluateString("UInt64")->isType());

                CHECK(evaluateString("Int8")->isType());
                CHECK(evaluateString("Int16")->isType());
                CHECK(evaluateString("Int32")->isType());
                CHECK(evaluateString("Int64")->isType());

                CHECK(evaluateString("Char8")->isType());
                CHECK(evaluateString("Char16")->isType());
                CHECK(evaluateString("Char32")->isType());

                CHECK(evaluateString("Float16")->isType());
                CHECK(evaluateString("Float32")->isType());
                CHECK(evaluateString("Float64")->isType());
            });
        });
    }

    TEST(Arithmetic)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK_EQUAL(2, evaluateStringWithValueOfType<int> ("1 + 1"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int> ("1 - 1"));
                CHECK_EQUAL(2, evaluateStringWithValueOfType<int> ("1+1"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int> ("1-1"));

                CHECK_EQUAL(6, evaluateStringWithValueOfType<int> ("2*3"));
                CHECK_EQUAL(5.5, evaluateStringWithValueOfType<double> ("2.5 + 3"));
                CHECK_EQUAL(5.5, evaluateStringWithValueOfType<double> ("2.5+3"));

                CHECK_EQUAL(Fraction({1}, {2}), evaluateStringWithValueOfType<Fraction> ("1/2"));
                CHECK_EQUAL(Fraction({3}, {2}), evaluateStringWithValueOfType<Fraction> ("1 + 1/2"));
                CHECK_EQUAL(Fraction({3}, {2}), evaluateStringWithValueOfType<Fraction> ("1/2 + 1"));
            });
        });
    }

    TEST(ImmutableLocalVariable)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK_EQUAL(42, evaluateStringWithValueOfType<int> ("let a := 42"));
                CHECK_EQUAL(42, evaluateStringWithValueOfType<int> ("let a := 42. a"));
                CHECK_EQUAL(43, evaluateStringWithValueOfType<int> ("let a := 42. a + 1"));

                CHECK_EQUAL(42, evaluateStringWithValueOfType<int> ("let a type: LiteralInteger := 42"));
                CHECK_EQUAL(42, evaluateStringWithValueOfType<int> ("let a type: LiteralInteger := 42. a"));
                CHECK_EQUAL(43, evaluateStringWithValueOfType<int> ("let a type: LiteralInteger := 42. a + 1"));
            });
        });
    }

    TEST(MutableLocalVariable)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK_EQUAL(42, evaluateStringWithValueOfType<int> ("let a mutable := 42"));
                CHECK_EQUAL(42, evaluateStringWithValueOfType<int> ("let a mutable := 42. a"));
                CHECK_EQUAL(4, evaluateStringWithValueOfType<int> ("let a mutable := 42. a := 4"));

                CHECK_EQUAL(4, evaluateStringWithValueOfType<int> ("let a mutable := 42. a := 4. a"));

                CHECK_EQUAL(3, evaluateStringWithValueOfType<int> ("let a mutable := 1. let b mutable := 2. a + b"));
                CHECK_EQUAL(3, evaluateStringWithValueOfType<int> ("let a mutable := Int32(1). let b mutable := Int32(2). a + b"));
            });
        });
    }

    TEST(PointerLikeTypes)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                {
                    auto a = evaluateString("Int32 pointer");
                    auto b = evaluateString("Int32 pointer");
                    CHECK(a->isPointerType());
                    CHECK(b->isPointerType());
                    CHECK_EQUAL(a, b);

                    auto c = evaluateString("Int32 pointerFor: #function");
                    auto d = evaluateString("Int32 pointerFor: #function");
                    CHECK(c->isPointerType());
                    CHECK(d->isPointerType());
                    CHECK_EQUAL(c, d);
                }

                {
                    auto a = evaluateString("Int32 ref");
                    auto b = evaluateString("Int32 ref");
                    auto c = evaluateString("Int32 ref ref");
                    auto d = evaluateString("Int32 ref tempRef");
                    CHECK(a->isReferenceType());
                    CHECK(b->isReferenceType());
                    CHECK(c->isReferenceType());
                    CHECK(d->isReferenceType());
                    CHECK_EQUAL(a, b);
                    CHECK_EQUAL(b, c);
                    CHECK_EQUAL(c, d);

                    auto e = evaluateString("Int32 refFor: #function");
                    auto f = evaluateString("Int32 refFor: #function");
                    auto g = evaluateString("(Int32 refFor: #function) ref");
                    auto h = evaluateString("(Int32 refFor: #function) refFor: #function");
                    CHECK(e->isReferenceType());
                    CHECK(f->isReferenceType());
                    CHECK(g->isReferenceType());
                    CHECK(h->isReferenceType());
                    CHECK_EQUAL(e, f);
                    CHECK_EQUAL(f, g);
                    CHECK_EQUAL(g, h);
                }

                {
                    auto a = evaluateString("Int32 tempRef");
                    auto b = evaluateString("Int32 tempRef");
                    auto c = evaluateString("Int32 tempRef tempRef");
                    auto d = evaluateString("Int32 tempRef ref");
                    CHECK(a->isTemporaryReferenceType());
                    CHECK(b->isTemporaryReferenceType());
                    CHECK(c->isTemporaryReferenceType());
                    CHECK(d->isReferenceType());
                    CHECK_EQUAL(a, b);
                    CHECK_EQUAL(b, c);

                    auto e = evaluateString("Int32 tempRefFor: #function");
                    auto f = evaluateString("Int32 tempRefFor: #function");
                    auto g = evaluateString("(Int32 tempRefFor: #function) tempRef");
                    auto h = evaluateString("(Int32 tempRefFor: #function) tempRefFor: #function");
                    CHECK(e->isTemporaryReferenceType());
                    CHECK(f->isTemporaryReferenceType());
                    CHECK(g->isTemporaryReferenceType());
                    CHECK(h->isTemporaryReferenceType());
                    CHECK_EQUAL(e, f);
                    CHECK_EQUAL(g, h);
                }
            });
        });
    }

    TEST(ImmutableGlobalVariable)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK_EQUAL(42, evaluateStringWithValueOfType<int> ("public global A := 42"));
                CHECK_EQUAL(42, evaluateStringWithValueOfType<int> ("A"));
            });
        });
    }

    TEST(NullaryFunction)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK(evaluateString("function f() => LiteralInteger := 42")->isClosureTypeValue());
                CHECK_EQUAL(42, evaluateStringWithValueOfType<int> ("function f() => LiteralInteger := 42. f()."));

                CHECK(evaluateString("function f() := 42")->isClosureTypeValue());
                CHECK_EQUAL(42, evaluateStringWithValueOfType<int> ("function f() := 42. f()."));
            });
        });
    }

    TEST(PublicNullaryFunction)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK(evaluateString("public function f() => LiteralInteger := 42")->isFunctionTypeValue());
                CHECK_EQUAL(42, evaluateStringWithValueOfType<int> ("f()."));

                CHECK(evaluateString("public function f2() := 42")->isFunctionTypeValue());
                CHECK_EQUAL(42, evaluateStringWithValueOfType<int> ("f()."));
            });
        });
    }

    TEST(SingleArgumentFunction)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK(evaluateString("function plusOne(x: LiteralInteger) => LiteralInteger := x + 1")->isClosureTypeValue());
                CHECK_EQUAL(43, evaluateStringWithValueOfType<int> ("function plusOne(x: LiteralInteger) => LiteralInteger := x + 1. plusOne(42)."));

                CHECK(evaluateString("function plusOne(x: LiteralInteger) := x + 1")->isClosureTypeValue());
                CHECK_EQUAL(43, evaluateStringWithValueOfType<int> ("function plusOne(x: LiteralInteger) := x + 1. plusOne(42)."));
            });
        });
    }

    TEST(PublicSingleArgumentFunction)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK(evaluateString("public function plusOne(x: LiteralInteger) => LiteralInteger := x + 1")->isFunctionTypeValue());
                CHECK_EQUAL(43, evaluateStringWithValueOfType<int> ("plusOne(42)."));

                CHECK(evaluateString("public function plusOne2(x: LiteralInteger) := x + 1")->isFunctionTypeValue());
                CHECK_EQUAL(43, evaluateStringWithValueOfType<int> ("plusOne2(42)."));
            });
        });
    }

    TEST(PrimitiveInt32Sum)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK(evaluateString("public function sum(a: Int32, b: Int32) => Int32 := a + b")->isFunctionTypeValue());
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int32_t> ("sum(Int32(), Int32())."));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int32_t> ("sum(Int32 zero, Int32 zero)."));
                CHECK_EQUAL(2, evaluateStringWithValueOfType<int32_t> ("sum(Int32 one, Int32 one)."));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int32_t> ("sum(0, 0)."));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("sum(0, 1)."));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("sum(1, 0)."));
                CHECK_EQUAL(2, evaluateStringWithValueOfType<int32_t> ("sum(1, 1)."));
                CHECK_EQUAL(4, evaluateStringWithValueOfType<int32_t> ("sum(1, 3)."));
            });
        });
    }

    TEST(PrimitiveTypeConstruction)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK_EQUAL(false, evaluateStringWithValueOfType<bool> ("Boolean8(false)"));
                CHECK_EQUAL(true, evaluateStringWithValueOfType<bool> ("Boolean8(true)"));

                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int8_t> ("Int8(-1)"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int8_t> ("Int8(0)"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int8_t> ("Int8(1)"));

                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int16_t> ("Int16(-1)"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int16_t> ("Int16(0)"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int16_t> ("Int16(1)"));

                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int32_t> ("Int32(-1)"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int32_t> ("Int32(0)"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("Int32(1)"));

                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int64_t> ("Int64(-1)"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int64_t> ("Int64(0)"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int64_t> ("Int64(1)"));

                CHECK_EQUAL(0u, evaluateStringWithValueOfType<uint8_t> ("UInt8(0)"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint8_t> ("UInt8(1)"));

                CHECK_EQUAL(0u, evaluateStringWithValueOfType<uint16_t> ("UInt16(0)"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint16_t> ("UInt16(1)"));

                CHECK_EQUAL(0u, evaluateStringWithValueOfType<uint32_t> ("UInt32(0)"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint32_t> ("UInt32(1)"));

                CHECK_EQUAL(0u, evaluateStringWithValueOfType<uint64_t> ("UInt64(0)"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint64_t> ("UInt64(1)"));

                CHECK_EQUAL(char(0), evaluateStringWithValueOfType<char> ("Char8(0)"));
                CHECK_EQUAL(char(1), evaluateStringWithValueOfType<char> ("Char8(1)"));

                CHECK_EQUAL(char16_t(0), evaluateStringWithValueOfType<char16_t> ("Char16(0)"));
                CHECK_EQUAL(char16_t(1), evaluateStringWithValueOfType<char16_t> ("Char16(1)"));

                CHECK_EQUAL(char32_t(0), evaluateStringWithValueOfType<char32_t> ("Char32(0)"));
                CHECK_EQUAL(char32_t(1), evaluateStringWithValueOfType<char32_t> ("Char32(1)"));

                CHECK_EQUAL(42, evaluateStringWithValueOfType<float> ("Float32(42)"));
                CHECK_EQUAL(42.5f, evaluateStringWithValueOfType<float> ("Float32(42.5)"));

                CHECK_EQUAL(42, evaluateStringWithValueOfType<double> ("Float64(42)"));
                CHECK_EQUAL(42.5, evaluateStringWithValueOfType<double> ("Float64(42.5)"));
            });
        });
    }

    TEST(PrimitiveTypeImplicitCastConstruction)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK_EQUAL(false, evaluateStringWithValueOfType<bool> ("false implicitCastTo: Boolean8"));
                CHECK_EQUAL(true, evaluateStringWithValueOfType<bool> ("true implicitCastTo: Boolean8"));

                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int8_t> ("-1 implicitCastTo: Int8"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int8_t> ("0 implicitCastTo: Int8"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int8_t> ("1 implicitCastTo: Int8"));

                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int16_t> ("-1 implicitCastTo: Int16"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int16_t> ("0 implicitCastTo: Int16"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int16_t> ("1 implicitCastTo: Int16"));

                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int32_t> ("-1 implicitCastTo: Int32"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int32_t> ("0 implicitCastTo: Int32"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("1 implicitCastTo: Int32"));

                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int64_t> ("-1 implicitCastTo: Int64"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int64_t> ("0 implicitCastTo: Int64"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int64_t> ("1 implicitCastTo: Int64"));

                CHECK_EQUAL(0u, evaluateStringWithValueOfType<uint8_t> ("0 implicitCastTo: UInt8"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint8_t> ("1 implicitCastTo: UInt8"));

                CHECK_EQUAL(0u, evaluateStringWithValueOfType<uint16_t> ("0 implicitCastTo: UInt16"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint16_t> ("1 implicitCastTo: UInt16"));

                CHECK_EQUAL(0u, evaluateStringWithValueOfType<uint32_t> ("0 implicitCastTo: UInt32"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint32_t> ("1 implicitCastTo: UInt32"));

                CHECK_EQUAL(0u, evaluateStringWithValueOfType<uint64_t> ("0 implicitCastTo: UInt64"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint64_t> ("1 implicitCastTo: UInt64"));

                CHECK_EQUAL(char(0), evaluateStringWithValueOfType<char> ("0 implicitCastTo: Char8"));
                CHECK_EQUAL(char(1), evaluateStringWithValueOfType<char> ("1 implicitCastTo: Char8"));
                CHECK_EQUAL('A', evaluateStringWithValueOfType<char> ("'A' implicitCastTo: Char8"));

                CHECK_EQUAL(char16_t(0), evaluateStringWithValueOfType<char16_t> ("0 implicitCastTo: Char16"));
                CHECK_EQUAL(char16_t(1), evaluateStringWithValueOfType<char16_t> ("1 implicitCastTo: Char16"));
                CHECK_EQUAL(char16_t('A'), evaluateStringWithValueOfType<char16_t> ("'A' implicitCastTo: Char16"));

                CHECK_EQUAL(char32_t(0), evaluateStringWithValueOfType<char32_t> ("0 implicitCastTo: Char32"));
                CHECK_EQUAL(char32_t(1), evaluateStringWithValueOfType<char32_t> ("1 implicitCastTo: Char32"));
                CHECK_EQUAL(char32_t('A'), evaluateStringWithValueOfType<char32_t> ("'A' implicitCastTo: Char32"));

                CHECK_EQUAL(42, evaluateStringWithValueOfType<float> ("42 implicitCastTo: Float32"));
                CHECK_EQUAL(42.5f, evaluateStringWithValueOfType<float> ("42.5 implicitCastTo: Float32"));

                CHECK_EQUAL(42, evaluateStringWithValueOfType<double> ("42 implicitCastTo: Float64"));
                CHECK_EQUAL(42.5, evaluateStringWithValueOfType<double> ("42.5 implicitCastTo: Float64"));
            });
        });
    }

    TEST(PrimitiveTypeExplicitCastConstruction)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK_EQUAL(false, evaluateStringWithValueOfType<bool> ("false castTo: Boolean8"));
                CHECK_EQUAL(true, evaluateStringWithValueOfType<bool> ("true castTo: Boolean8"));

                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int8_t> ("-1 castTo: Int8"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int8_t> ("0 castTo: Int8"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int8_t> ("1 castTo: Int8"));

                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int16_t> ("-1 castTo: Int16"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int16_t> ("0 castTo: Int16"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int16_t> ("1 castTo: Int16"));

                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int32_t> ("-1 castTo: Int32"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int32_t> ("0 castTo: Int32"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("1 castTo: Int32"));

                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int64_t> ("-1 castTo: Int64"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int64_t> ("0 castTo: Int64"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int64_t> ("1 castTo: Int64"));

                CHECK_EQUAL(0u, evaluateStringWithValueOfType<uint8_t> ("0 castTo: UInt8"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint8_t> ("1 castTo: UInt8"));

                CHECK_EQUAL(0u, evaluateStringWithValueOfType<uint16_t> ("0 castTo: UInt16"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint16_t> ("1 castTo: UInt16"));

                CHECK_EQUAL(0u, evaluateStringWithValueOfType<uint32_t> ("0 castTo: UInt32"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint32_t> ("1 castTo: UInt32"));

                CHECK_EQUAL(0u, evaluateStringWithValueOfType<uint64_t> ("0 castTo: UInt64"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint64_t> ("1 castTo: UInt64"));

                CHECK_EQUAL(char(0), evaluateStringWithValueOfType<char> ("0 castTo: Char8"));
                CHECK_EQUAL(char(1), evaluateStringWithValueOfType<char> ("1 castTo: Char8"));
                CHECK_EQUAL('A', evaluateStringWithValueOfType<char> ("'A' castTo: Char8"));

                CHECK_EQUAL(char16_t(0), evaluateStringWithValueOfType<char16_t> ("0 castTo: Char16"));
                CHECK_EQUAL(char16_t(1), evaluateStringWithValueOfType<char16_t> ("1 castTo: Char16"));
                CHECK_EQUAL(char16_t('A'), evaluateStringWithValueOfType<char16_t> ("'A' castTo: Char16"));

                CHECK_EQUAL(char32_t(0), evaluateStringWithValueOfType<char32_t> ("0 castTo: Char32"));
                CHECK_EQUAL(char32_t(1), evaluateStringWithValueOfType<char32_t> ("1 castTo: Char32"));
                CHECK_EQUAL(char32_t('A'), evaluateStringWithValueOfType<char32_t> ("'A' castTo: Char32"));

                CHECK_EQUAL(42, evaluateStringWithValueOfType<float> ("42 castTo: Float32"));
                CHECK_EQUAL(42.5f, evaluateStringWithValueOfType<float> ("42.5 castTo: Float32"));

                CHECK_EQUAL(42, evaluateStringWithValueOfType<double> ("42 castTo: Float64"));
                CHECK_EQUAL(42.5, evaluateStringWithValueOfType<double> ("42.5 castTo: Float64"));

                CHECK_EQUAL(false, evaluateStringWithValueOfType<bool> ("false explicitCastTo: Boolean8"));
                CHECK_EQUAL(true, evaluateStringWithValueOfType<bool> ("true explicitCastTo: Boolean8"));

                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int8_t> ("-1 explicitCastTo: Int8"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int8_t> ("0 explicitCastTo: Int8"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int8_t> ("1 explicitCastTo: Int8"));

                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int16_t> ("-1 explicitCastTo: Int16"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int16_t> ("0 explicitCastTo: Int16"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int16_t> ("1 explicitCastTo: Int16"));

                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int32_t> ("-1 explicitCastTo: Int32"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int32_t> ("0 explicitCastTo: Int32"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("1 explicitCastTo: Int32"));

                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int64_t> ("-1 explicitCastTo: Int64"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int64_t> ("0 explicitCastTo: Int64"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int64_t> ("1 explicitCastTo: Int64"));

                CHECK_EQUAL(0u, evaluateStringWithValueOfType<uint8_t> ("0 explicitCastTo: UInt8"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint8_t> ("1 explicitCastTo: UInt8"));

                CHECK_EQUAL(0u, evaluateStringWithValueOfType<uint16_t> ("0 explicitCastTo: UInt16"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint16_t> ("1 explicitCastTo: UInt16"));

                CHECK_EQUAL(0u, evaluateStringWithValueOfType<uint32_t> ("0 explicitCastTo: UInt32"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint32_t> ("1 explicitCastTo: UInt32"));

                CHECK_EQUAL(0u, evaluateStringWithValueOfType<uint64_t> ("0 explicitCastTo: UInt64"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint64_t> ("1 explicitCastTo: UInt64"));

                CHECK_EQUAL(char(0), evaluateStringWithValueOfType<char> ("0 explicitCastTo: Char8"));
                CHECK_EQUAL(char(1), evaluateStringWithValueOfType<char> ("1 explicitCastTo: Char8"));
                CHECK_EQUAL('A', evaluateStringWithValueOfType<char> ("'A' explicitCastTo: Char8"));

                CHECK_EQUAL(char16_t(0), evaluateStringWithValueOfType<char16_t> ("0 explicitCastTo: Char16"));
                CHECK_EQUAL(char16_t(1), evaluateStringWithValueOfType<char16_t> ("1 explicitCastTo: Char16"));
                CHECK_EQUAL(char16_t('A'), evaluateStringWithValueOfType<char16_t> ("'A' explicitCastTo: Char16"));

                CHECK_EQUAL(char32_t(0), evaluateStringWithValueOfType<char32_t> ("0 explicitCastTo: Char32"));
                CHECK_EQUAL(char32_t(1), evaluateStringWithValueOfType<char32_t> ("1 explicitCastTo: Char32"));
                CHECK_EQUAL(char32_t('A'), evaluateStringWithValueOfType<char32_t> ("'A' explicitCastTo: Char32"));

                CHECK_EQUAL(42, evaluateStringWithValueOfType<float> ("42 explicitCastTo: Float32"));
                CHECK_EQUAL(42.5f, evaluateStringWithValueOfType<float> ("42.5 explicitCastTo: Float32"));

                CHECK_EQUAL(42, evaluateStringWithValueOfType<double> ("42 explicitCastTo: Float64"));
                CHECK_EQUAL(42.5, evaluateStringWithValueOfType<double> ("42.5 explicitCastTo: Float64"));
            });
        });
    }

    TEST(ImmutableCopyConstructor)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int8_t> ("Int8(Int8(-1))"));
                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int16_t> ("Int16(Int16(-1))"));
                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int32_t> ("Int32(Int32(-1))"));
                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int64_t> ("Int64(Int64(-1))"));

                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint8_t> ("UInt8(UInt8(1))"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint16_t> ("UInt16(UInt16(1))"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint32_t> ("UInt32(UInt32(1))"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint64_t> ("UInt64(UInt64(1))"));

                CHECK_EQUAL(char(1), evaluateStringWithValueOfType<char> ("Char8(Char8(1))"));
                CHECK_EQUAL(char16_t(1), evaluateStringWithValueOfType<char16_t> ("Char16(Char16(1))"));
                CHECK_EQUAL(char32_t(1), evaluateStringWithValueOfType<char32_t> ("Char32(Char32(1))"));
            });
        });
    }

    TEST(If)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK(evaluateString("public function select(condition, a, b) := if: condition then: a else: b")->isFunctionTypeValue());
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("select(true, 1, 2)"));
                CHECK_EQUAL(2, evaluateStringWithValueOfType<int32_t> ("select(false, 1, 2)"));

                CHECK(evaluateString("public function selectBoolean8(condition: Boolean8, a: Int32, b: Int32) => Int32 := if: condition then: a else: b")->isFunctionTypeValue());
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("selectBoolean8(true, 1, 2)"));
                CHECK_EQUAL(2, evaluateStringWithValueOfType<int32_t> ("selectBoolean8(false, 1, 2)"));
            });
        });
    }

    TEST(Return)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK(evaluateString("public function returnObject(object) := {return: object}")->isFunctionTypeValue());
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("returnObject(1)"));
                CHECK_EQUAL(2, evaluateStringWithValueOfType<int32_t> ("returnObject(2)"));

                CHECK(evaluateString("public function returnInt32(value: Int32) => Int32 := {return: value}")->isFunctionTypeValue());
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("returnInt32(1)"));
                CHECK_EQUAL(2, evaluateStringWithValueOfType<int32_t> ("returnInt32(2)"));

                CHECK(evaluateString("public function returnObjectDeadCode(object) := {return: object . 2}")->isFunctionTypeValue());
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("returnObjectDeadCode(1)"));
                CHECK_EQUAL(2, evaluateStringWithValueOfType<int32_t> ("returnObjectDeadCode(2)"));

                CHECK(evaluateString("public function returnInt32DeadCode(value: Int32) => Int32 := {return: value . -42.0}")->isFunctionTypeValue());
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("returnInt32DeadCode(1)"));
                CHECK_EQUAL(2, evaluateStringWithValueOfType<int32_t> ("returnInt32DeadCode(2)"));
            });
        });
    }

    TEST(WhileDoContinueWith)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK(evaluateString(
"public function sumNaturals(n) := {\n"
"let sum mutable := 0.\n"
"let i mutable := 0.\n"
"while: (i <= n) do: {\n"
"sum := sum + i\n"
"} continueWith: (i := i + 1).\n"
"sum\n"
"}\n")->isFunctionTypeValue());
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int32_t> ("sumNaturals(0)"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("sumNaturals(1)"));
                CHECK_EQUAL(3, evaluateStringWithValueOfType<int32_t> ("sumNaturals(2)"));
                CHECK_EQUAL(6, evaluateStringWithValueOfType<int32_t> ("sumNaturals(3)"));
                CHECK_EQUAL(10, evaluateStringWithValueOfType<int32_t> ("sumNaturals(4)"));
                CHECK_EQUAL(15, evaluateStringWithValueOfType<int32_t> ("sumNaturals(5)"));
                CHECK_EQUAL(21, evaluateStringWithValueOfType<int32_t> ("sumNaturals(6)"));
                CHECK_EQUAL(28, evaluateStringWithValueOfType<int32_t> ("sumNaturals(7)"));
                CHECK_EQUAL(36, evaluateStringWithValueOfType<int32_t> ("sumNaturals(8)"));
                CHECK_EQUAL(45, evaluateStringWithValueOfType<int32_t> ("sumNaturals(9)"));
                CHECK_EQUAL(55, evaluateStringWithValueOfType<int32_t> ("sumNaturals(10)"));

                CHECK(evaluateString(
"public function sumNaturalsInt32(n: Int32) => Int32 := {\n"
"let sum mutable type: Int32 := 0.\n"
"let i mutable type: Int32 := 0.\n"
"while: (i <= n) do: {\n"
"sum := sum + i\n"
"} continueWith: (i := i + 1).\n"
"sum\n"
"}\n")->isFunctionTypeValue());
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int32_t> ("sumNaturalsInt32(0)"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("sumNaturalsInt32(1)"));
                CHECK_EQUAL(3, evaluateStringWithValueOfType<int32_t> ("sumNaturalsInt32(2)"));
                CHECK_EQUAL(6, evaluateStringWithValueOfType<int32_t> ("sumNaturalsInt32(3)"));
                CHECK_EQUAL(10, evaluateStringWithValueOfType<int32_t> ("sumNaturalsInt32(4)"));
                CHECK_EQUAL(15, evaluateStringWithValueOfType<int32_t> ("sumNaturalsInt32(5)"));
                CHECK_EQUAL(21, evaluateStringWithValueOfType<int32_t> ("sumNaturalsInt32(6)"));
                CHECK_EQUAL(28, evaluateStringWithValueOfType<int32_t> ("sumNaturalsInt32(7)"));
                CHECK_EQUAL(36, evaluateStringWithValueOfType<int32_t> ("sumNaturalsInt32(8)"));
                CHECK_EQUAL(45, evaluateStringWithValueOfType<int32_t> ("sumNaturalsInt32(9)"));
                CHECK_EQUAL(55, evaluateStringWithValueOfType<int32_t> ("sumNaturalsInt32(10)"));
            });
        });
    }

    TEST(DoWhileContinueWith)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK(evaluateString(
"public function sumNaturals(n) := {\n"
"let sum mutable := 0.\n"
"let i mutable := 0.\n"
"do: {\n"
"sum := sum + i\n"
"} while: (i < n) continueWith: (i := i + 1).\n"
"sum\n"
"}\n")->isFunctionTypeValue());
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int32_t> ("sumNaturals(0)"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("sumNaturals(1)"));
                CHECK_EQUAL(3, evaluateStringWithValueOfType<int32_t> ("sumNaturals(2)"));
                CHECK_EQUAL(6, evaluateStringWithValueOfType<int32_t> ("sumNaturals(3)"));
                CHECK_EQUAL(10, evaluateStringWithValueOfType<int32_t> ("sumNaturals(4)"));
                CHECK_EQUAL(15, evaluateStringWithValueOfType<int32_t> ("sumNaturals(5)"));
                CHECK_EQUAL(21, evaluateStringWithValueOfType<int32_t> ("sumNaturals(6)"));
                CHECK_EQUAL(28, evaluateStringWithValueOfType<int32_t> ("sumNaturals(7)"));
                CHECK_EQUAL(36, evaluateStringWithValueOfType<int32_t> ("sumNaturals(8)"));
                CHECK_EQUAL(45, evaluateStringWithValueOfType<int32_t> ("sumNaturals(9)"));
                CHECK_EQUAL(55, evaluateStringWithValueOfType<int32_t> ("sumNaturals(10)"));

                CHECK(evaluateString(
"public function sumNaturalsInt32(n: Int32) => Int32 := {\n"
"let sum mutable type: Int32 := 0.\n"
"let i mutable type: Int32 := 0.\n"
"do: {\n"
"sum := sum + i\n"
"} while: (i < n) continueWith: (i := i + 1).\n"
"sum\n"
"}\n")->isFunctionTypeValue());
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int32_t> ("sumNaturalsInt32(0)"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("sumNaturalsInt32(1)"));
                CHECK_EQUAL(3, evaluateStringWithValueOfType<int32_t> ("sumNaturalsInt32(2)"));
                CHECK_EQUAL(6, evaluateStringWithValueOfType<int32_t> ("sumNaturalsInt32(3)"));
                CHECK_EQUAL(10, evaluateStringWithValueOfType<int32_t> ("sumNaturalsInt32(4)"));
                CHECK_EQUAL(15, evaluateStringWithValueOfType<int32_t> ("sumNaturalsInt32(5)"));
                CHECK_EQUAL(21, evaluateStringWithValueOfType<int32_t> ("sumNaturalsInt32(6)"));
                CHECK_EQUAL(28, evaluateStringWithValueOfType<int32_t> ("sumNaturalsInt32(7)"));
                CHECK_EQUAL(36, evaluateStringWithValueOfType<int32_t> ("sumNaturalsInt32(8)"));
                CHECK_EQUAL(45, evaluateStringWithValueOfType<int32_t> ("sumNaturalsInt32(9)"));
                CHECK_EQUAL(55, evaluateStringWithValueOfType<int32_t> ("sumNaturalsInt32(10)"));
            });
        });
    }

    TEST(Namespace)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto first = evaluateString("namespace TestNamespace");
                CHECK(first->isNamespace());

                auto second = evaluateString("namespace TestNamespace definition: {}");
                CHECK(second->isNamespace());

                auto third = evaluateString("TestNamespace");
                CHECK(third->isNamespace());

                CHECK_EQUAL(first, second);
                CHECK_EQUAL(second, third);
            });
        });
    }

    TEST(NamespaceMethod)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                evaluateString("namespace TestNamespace definition: { public method square: x := x*x }");

                CHECK_EQUAL(1, unwrapValue<int> (evaluateString("TestNamespace square: 1")));
                CHECK_EQUAL(4, unwrapValue<int> (evaluateString("TestNamespace square: 2")));
                CHECK_EQUAL(9, unwrapValue<int> (evaluateString("TestNamespace square: 3")));
            });
        });
    }

    TEST(Struct)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structDeclaration = evaluateString("public struct TestStruct.");
                CHECK(structDeclaration->isStructureType());
                
                auto structDefinition = evaluateString("public struct TestStruct definition: {}.");
                CHECK(structDefinition->isStructureType());
                CHECK_EQUAL(structDeclaration, structDefinition);
                CHECK_EQUAL(Module::getActive(), std::static_pointer_cast<StructureType> (structDefinition)->getDefinitionModule());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(Union)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto unionDeclaration = evaluateString("public union TestUnion.");
                CHECK(unionDeclaration->isUnionType());
                
                auto unionDefinition = evaluateString("public union TestUnion definition: {}.");
                CHECK(unionDefinition->isUnionType());
                CHECK_EQUAL(unionDeclaration, unionDefinition);
                CHECK_EQUAL(Module::getActive(), std::static_pointer_cast<UnionType> (unionDefinition)->getDefinitionModule());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(Class)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto classDeclaration = evaluateString("public class TestClass.");
                CHECK(classDeclaration->isClassType());
                
                auto classDefinition = evaluateString("public class TestClass definition: {}.");
                CHECK(classDefinition->isClassType());
                CHECK_EQUAL(classDeclaration, classDefinition);
                CHECK_EQUAL(Module::getActive(), std::static_pointer_cast<ClassType> (classDefinition)->getDefinitionModule());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(SuperClass)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto superclass = evaluateString("public class SuperClass definition: {}.");
                CHECK(superclass->isClassType());
                
                auto subclass = evaluateString("public class SubClass superclass: SuperClass; definition: {}.");
                CHECK(subclass->isClassType());
                CHECK_EQUAL(superclass, std::static_pointer_cast<Type> (subclass)->getSupertype());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(Enum)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto enumDeclaration = evaluateString("public enum TestEnum.");
                CHECK(enumDeclaration->isEnumType());
                
                auto enumDefinition = evaluateString("public enum TestEnum valueType: UInt32; values: #{}; definition: {}.");
                CHECK(enumDefinition->isEnumType());
                CHECK_EQUAL(enumDeclaration, enumDefinition);
                CHECK_EQUAL(UInt32::__staticType__(), std::static_pointer_cast<EnumType> (enumDefinition)->getBaseType());
                CHECK_EQUAL(Module::getActive(), std::static_pointer_cast<EnumType> (enumDefinition)->getDefinitionModule());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(StructField)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structDefinition = evaluateString("public struct TestStruct definition: {public field int32Field type: Int32}.");
                CHECK(structDefinition->isStructureType());

                Module::getActive()->analyzeAllPendingProgramEntities();

                auto structType = std::static_pointer_cast<StructureType> (structDefinition);
                CHECK_EQUAL(1u, structType->fields.size());
            });
        });
    }

    TEST(StructMethod)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structDefinition = evaluateString("public struct TestStruct definition: {public method square: x := x*x}.");
                CHECK(structDefinition->isStructureType());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(StructExtensionMethod)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structDefinition = evaluateString("public struct TestStruct definition: {}.");
                CHECK(structDefinition->isStructureType());

                auto extensionDefinition = evaluateString("TestStruct extend: {public method square: x := x*x}.");
                CHECK_EQUAL(structDefinition, extensionDefinition);

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(EmptyStructBasicNewValue)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structDefinition = evaluateString("public struct TestStruct definition: {}.");
                CHECK(structDefinition->isStructureType());

                auto structValue = evaluateString("TestStruct basicNewValue");
                CHECK(structValue->isStructureTypeValue());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(EmptyStructNewValue)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structDefinition = evaluateString("public struct TestStruct definition: {}.");
                CHECK(structDefinition->isStructureType());

                auto structValue = evaluateString("TestStruct newValue");
                CHECK(structValue->isStructureTypeValue());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(EmptyStructNewValue2)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structDefinition = evaluateString("public struct TestStruct definition: {}.");
                CHECK(structDefinition->isStructureType());

                auto structValue = evaluateString("TestStruct()");
                CHECK(structValue->isStructureTypeValue());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }
}
