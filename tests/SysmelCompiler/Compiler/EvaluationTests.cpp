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
