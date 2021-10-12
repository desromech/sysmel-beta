#include "sysmel/BootstrapEnvironment/AnyValue.hpp"
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

    TEST(LocalVariable)
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

    TEST(NullaryFunction)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK(evaluateString("function f() => LiteralInteger := 42")->isMethod());
                CHECK_EQUAL(42, evaluateStringWithValueOfType<int> ("function f() => LiteralInteger := 42. f()."));

                CHECK(evaluateString("function f() := 42")->isMethod());
                CHECK_EQUAL(42, evaluateStringWithValueOfType<int> ("function f() := 42. f()."));
            });
        });
    }

    TEST(SingleArgumentFunction)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK(evaluateString("function plusOne(x: LiteralInteger) => LiteralInteger := x + 1")->isMethod());
                CHECK_EQUAL(43, evaluateStringWithValueOfType<int> ("function plusOne(x: LiteralInteger) => LiteralInteger := x + 1. f(42)."));

                CHECK(evaluateString("function plusOne(x: LiteralInteger) := x + 1")->isMethod());
                CHECK_EQUAL(43, evaluateStringWithValueOfType<int> ("function plusOne(x: LiteralInteger) := x + 1. f(42)."));
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

    TEST(Struct)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structDeclaration = evaluateString("public struct TestStruct.");
                CHECK(structDeclaration->isType());
                
                auto structDefinition = evaluateString("public struct TestStruct definition: {}.");
                CHECK(structDefinition->isType());
                CHECK_EQUAL(structDeclaration, structDefinition);
            });
        });
    }

    TEST(Union)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto unionDeclaration = evaluateString("public union TestUnion.");
                CHECK(unionDeclaration->isType());
                
                auto unionDefinition = evaluateString("public union TestUnion definition: {}.");
                CHECK(unionDefinition->isType());
                CHECK_EQUAL(unionDeclaration, unionDefinition);
            });
        });
    }

    TEST(Class)
    {
        RuntimeContext::create()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto classDeclaration = evaluateString("public class TestClass.");
                CHECK(classDeclaration->isType());
                
                auto classDefinition = evaluateString("public class TestClass definition: {}.");
                CHECK(classDefinition->isType());
                CHECK_EQUAL(classDeclaration, classDefinition);
            });
        });
    }
}
