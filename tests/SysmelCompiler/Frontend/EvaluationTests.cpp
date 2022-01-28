#include "Environment/AnyValue.hpp"
#include "Environment/Type.hpp"
#include "Environment/EnumType.hpp"
#include "Environment/StructureType.hpp"
#include "Environment/ClassType.hpp"
#include "Environment/UnionType.hpp"
#include "Environment/FunctionType.hpp"
#include "Environment/ClosureType.hpp"
#include "Environment/MethodType.hpp"
#include "Environment/PrimitiveIntegerType.hpp"
#include "Environment/PrimitiveFloatType.hpp"
#include "Environment/Wrappers.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/BootstrapModule.hpp"
#include "Environment/ScriptModule.hpp"
#include "Environment/LanguageSupport.hpp"

#include "UnitTest++/UnitTest++.h"

using namespace Sysmel::Environment;

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
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int> ("0"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int> ("1"));
                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int> ("-1"));

                CHECK_EQUAL(true, evaluateStringWithValueOfType<bool> ("true"));
                CHECK_EQUAL(false, evaluateStringWithValueOfType<bool> ("false"));
                CHECK(evaluateString("void")->isVoid());
                CHECK(evaluateString("nil")->isUndefined());

                CHECK(evaluateString("#()")->isLiteralArray());
                {
                    CHECK(evaluateString("#(1 c true false nil)")->isLiteralArray());
                    auto literalArray = evaluateString("#(1 c true false nil)")->unwrapAsArray();
                    CHECK_EQUAL(5u, literalArray.size());
                    
                    CHECK(literalArray[0]->isLiteralInteger());
                    CHECK_EQUAL(1, literalArray[0]->unwrapAsInt32());

                    CHECK(literalArray[1]->isLiteralSymbol());
                    CHECK_EQUAL("c", literalArray[1]->unwrapAsString());

                    CHECK(literalArray[2]->isLiteralBoolean());
                    CHECK_EQUAL(true, literalArray[2]->unwrapAsBoolean());

                    CHECK(literalArray[3]->isLiteralBoolean());
                    CHECK_EQUAL(false, literalArray[3]->unwrapAsBoolean());

                    CHECK(validAnyValue(literalArray[4])->isUndefined());
                }

                CHECK(evaluateString("#{}")->isLiteralDictionary());
                {
                    CHECK(evaluateString("#{Nil: . First: 1 . #Second : 2}")->isLiteralDictionary());
                    auto literalDictionary = evaluateString("#{Nil: . First: 1 . #Second : 2}")->unwrapAsArray();
                    CHECK_EQUAL(3u, literalDictionary.size());
                    
                    CHECK(literalDictionary[0]->isLiteralAssociation());
                    CHECK_EQUAL(internSymbol("Nil"), literalDictionary[0]->perform<AnyValuePtr> ("key"));
                    CHECK(validAnyValue(literalDictionary[0]->perform<AnyValuePtr> ("value"))->isUndefined());

                    CHECK(literalDictionary[1]->isLiteralAssociation());
                    CHECK_EQUAL(internSymbol("First"), literalDictionary[1]->perform<AnyValuePtr> ("key"));
                    CHECK_EQUAL(1, literalDictionary[1]->perform<int> ("value"));

                    CHECK(literalDictionary[2]->isLiteralAssociation());
                    CHECK_EQUAL(internSymbol("Second"), literalDictionary[2]->perform<AnyValuePtr> ("key"));
                    CHECK_EQUAL(2, literalDictionary[2]->perform<int> ("value"));
                }

            });
        });
    }

    TEST(CompileTimeTypes)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
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
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                const auto &targetDescription = RuntimeContext::getActive()->getTargetDescription();
                CHECK(evaluateString("Boolean8")->isType());

                CHECK(evaluateString("UInt8")->isType());
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint64_t> ("UInt8 memorySize"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint64_t> ("UInt8 memoryAlignment"));

                CHECK(evaluateString("UInt16")->isType());
                CHECK_EQUAL(2u, evaluateStringWithValueOfType<uint64_t> ("UInt16 memorySize"));
                CHECK_EQUAL(2u, evaluateStringWithValueOfType<uint64_t> ("UInt16 memoryAlignment"));

                CHECK(evaluateString("UInt32")->isType());
                CHECK_EQUAL(4u, evaluateStringWithValueOfType<uint64_t> ("UInt32 memorySize"));
                CHECK_EQUAL(4u, evaluateStringWithValueOfType<uint64_t> ("UInt32 memoryAlignment"));

                CHECK(evaluateString("UInt64")->isType());
                CHECK_EQUAL(8u, evaluateStringWithValueOfType<uint64_t> ("UInt64 memorySize"));
                CHECK_EQUAL(8u, evaluateStringWithValueOfType<uint64_t> ("UInt64 memoryAlignment"));

                CHECK(evaluateString("Int8")->isType());
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint64_t> ("Int8 memorySize"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint64_t> ("Int8 memoryAlignment"));

                CHECK(evaluateString("Int16")->isType());
                CHECK_EQUAL(2u, evaluateStringWithValueOfType<uint64_t> ("Int16 memorySize"));
                CHECK_EQUAL(2u, evaluateStringWithValueOfType<uint64_t> ("Int16 memoryAlignment"));

                CHECK(evaluateString("Int32")->isType());
                CHECK_EQUAL(4u, evaluateStringWithValueOfType<uint64_t> ("Int32 memorySize"));
                CHECK_EQUAL(4u, evaluateStringWithValueOfType<uint64_t> ("Int32 memoryAlignment"));

                CHECK(evaluateString("Int64")->isType());
                CHECK_EQUAL(8u, evaluateStringWithValueOfType<uint64_t> ("Int64 memorySize"));
                CHECK_EQUAL(8u, evaluateStringWithValueOfType<uint64_t> ("Int64 memoryAlignment"));

                CHECK(evaluateString("Char8")->isType());
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint64_t> ("Char8 memorySize"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint64_t> ("Char8 memoryAlignment"));

                CHECK(evaluateString("Char16")->isType());
                CHECK_EQUAL(2u, evaluateStringWithValueOfType<uint64_t> ("Char16 memorySize"));
                CHECK_EQUAL(2u, evaluateStringWithValueOfType<uint64_t> ("Char16 memoryAlignment"));

                CHECK(evaluateString("Char32")->isType());
                CHECK_EQUAL(4u, evaluateStringWithValueOfType<uint64_t> ("Char32 memorySize"));
                CHECK_EQUAL(4u, evaluateStringWithValueOfType<uint64_t> ("Char32 memoryAlignment"));

                CHECK(evaluateString("Float16")->isType());
                CHECK_EQUAL(2u, evaluateStringWithValueOfType<uint64_t> ("Float16 memorySize"));
                CHECK_EQUAL(2u, evaluateStringWithValueOfType<uint64_t> ("Float16 memoryAlignment"));

                CHECK(evaluateString("Float32")->isType());
                CHECK_EQUAL(4u, evaluateStringWithValueOfType<uint64_t> ("Float32 memorySize"));
                CHECK_EQUAL(4u, evaluateStringWithValueOfType<uint64_t> ("Float32 memoryAlignment"));

                CHECK(evaluateString("Float64")->isType());
                CHECK_EQUAL(8u, evaluateStringWithValueOfType<uint64_t> ("Float64 memorySize"));
                CHECK_EQUAL(targetDescription.doubleAlignment, evaluateStringWithValueOfType<uint64_t> ("Float64 memoryAlignment"));

                CHECK(evaluateString("UIntPointer")->isType());
                CHECK_EQUAL(targetDescription.pointerSize, evaluateStringWithValueOfType<uint64_t> ("UIntPointer memorySize"));

                CHECK(evaluateString("IntPointer")->isType());
                CHECK_EQUAL(targetDescription.pointerSize, evaluateStringWithValueOfType<uint64_t> ("IntPointer memorySize"));
            });
        });
    }

    TEST(Arithmetic)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK_EQUAL(2, evaluateStringWithValueOfType<int> ("1 + 1"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int> ("1 - 1"));
                CHECK_EQUAL(2, evaluateStringWithValueOfType<int> ("1+1"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int> ("1-1"));

                CHECK_EQUAL(6, evaluateStringWithValueOfType<int> ("2*3"));
                CHECK_EQUAL(5.5, evaluateStringWithValueOfType<double> ("2.5 + 3"));
                CHECK_EQUAL(5.5, evaluateStringWithValueOfType<double> ("2.5+3"));

                CHECK_EQUAL(Fraction(1, 2), evaluateStringWithValueOfType<Fraction> ("1/2"));
                CHECK_EQUAL(Fraction(3, 2), evaluateStringWithValueOfType<Fraction> ("1 + 1/2"));
                CHECK_EQUAL(Fraction(3, 2), evaluateStringWithValueOfType<Fraction> ("1/2 + 1"));
            });
        });
    }

    TEST(ImmutableLocalVariable)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
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
        RuntimeContext::createForScripting()->activeDuring([&](){
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
        RuntimeContext::createForScripting()->activeDuring([&](){
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

    TEST(DecoratedTypes)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                {
                    auto a = evaluateString("Int32 const");
                    auto b = evaluateString("Int32 const");
                    CHECK(a->isDecoratedType());
                    CHECK(a->isConstDecoratedType());
                    CHECK(b->isDecoratedType());
                    CHECK(b->isConstDecoratedType());
                    CHECK_EQUAL(a, b);
                }

                {
                    auto a = evaluateString("Int32 volatile");
                    auto b = evaluateString("Int32 volatile");
                    CHECK(a->isDecoratedType());
                    CHECK(a->isVolatileDecoratedType());
                    CHECK(b->isDecoratedType());
                    CHECK(b->isVolatileDecoratedType());
                    CHECK_EQUAL(a, b);
                }

                {
                    auto a = evaluateString("Int32 restrict");
                    auto b = evaluateString("Int32 restrict");
                    CHECK(a->isDecoratedType());
                    CHECK(a->isRestrictDecoratedType());
                    CHECK(b->isDecoratedType());
                    CHECK(b->isRestrictDecoratedType());
                    CHECK_EQUAL(a, b);
                }

                {
                    auto a = evaluateString("Int32 const volatile");
                    auto b = evaluateString("Int32 volatile const");
                    CHECK(a->isDecoratedType());
                    CHECK(a->isConstDecoratedType());
                    CHECK(a->isVolatileDecoratedType());
                    CHECK(b->isDecoratedType());
                    CHECK(b->isConstDecoratedType());
                    CHECK(b->isVolatileDecoratedType());
                    CHECK_EQUAL(a, b);
                }

                {
                    auto a = evaluateString("Int32 ref const");
                    auto b = evaluateString("Int32 ref");
                    CHECK(a->isReferenceType());
                    CHECK(!a->isDecoratedType());
                    CHECK(!a->isConstDecoratedType());
                    CHECK(!a->isVolatileDecoratedType());
                    CHECK_EQUAL(a, b);
                }

                {
                    auto a = evaluateString("Int32 tempRef const");
                    auto b = evaluateString("Int32 tempRef");
                    CHECK(a->isTemporaryReferenceType());
                    CHECK(!a->isDecoratedType());
                    CHECK(!a->isConstDecoratedType());
                    CHECK(!a->isVolatileDecoratedType());
                    CHECK_EQUAL(a, b);
                }
            });
        });
    }

    TEST(ArrayTypes)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                {
                    auto a = evaluateString("Int32 array");
                    auto b = evaluateString("Int32 array: 0");
                    CHECK(a->isArrayType());
                    CHECK(b->isArrayType());
                    CHECK_EQUAL(a, b);
                }

                {
                    auto a = evaluateString("Int32 array");
                    auto b = evaluateString("Int32 const array: 0");
                    CHECK(a->isArrayType());
                    CHECK(b->isArrayType());
                    CHECK_EQUAL(a, b);
                }

                {
                    auto a = evaluateString("Int32[]");
                    auto b = evaluateString("Int32[]");
                    CHECK(a->isArrayType());
                    CHECK(b->isArrayType());
                    CHECK_EQUAL(a, b);
                }

                {
                    auto a = evaluateString("Int32[]");
                    auto b = evaluateString("Int32 const[]");
                    CHECK(a->isArrayType());
                    CHECK(b->isArrayType());
                    CHECK_EQUAL(a, b);
                }

                {
                    CHECK(evaluateString("Int32[]()")->isArrayTypeValue());
                    CHECK(evaluateString("Int32[2](1, 2)")->isArrayTypeValue());
                    CHECK(evaluateString("Int32[](1, 2)")->isArrayTypeValue());

                    CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("Int32[](1, 2)[0]"));
                    CHECK_EQUAL(2, evaluateStringWithValueOfType<int32_t> ("Int32[](1, 2)[1]"));
                    CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("Int32[2](1, 2)[0]"));
                    CHECK_EQUAL(2, evaluateStringWithValueOfType<int32_t> ("Int32[2](1, 2)[1]"));

                    CHECK_EQUAL(2u, evaluateStringWithValueOfType<uint32_t> ("Int32[2] size"));
                    CHECK_EQUAL(2u, evaluateStringWithValueOfType<uint32_t> ("Int32[2]() size"));
                    CHECK_EQUAL(2u, evaluateStringWithValueOfType<uint32_t> ("Int32[2](1, 2) size"));
                    CHECK_EQUAL(3u, evaluateStringWithValueOfType<uint32_t> ("Int32[](1, 2, 3) size"));
                }
            });
        });
    }

    TEST(TupleTypes)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                {
                    auto a = evaluateString("Int32 & Void");
                    auto b = evaluateString("Void & Int32");
                    CHECK_EQUAL(a, Int32::__staticType__());
                    CHECK_EQUAL(b, Int32::__staticType__());
                    CHECK_EQUAL(a, b);
                }

                {
                    CHECK_EQUAL(Type::getVoidType(), evaluateString("Void & Void"));
                }

                {
                    auto a = evaluateString("Int32 & Int64");
                    auto b = evaluateString("Int32 & Int64");
                    CHECK(a->isTupleType());
                    CHECK(b->isTupleType());
                    CHECK_EQUAL(a, b);

                    auto tupleInstance = evaluateString("Int32(1), Int64(2)");
                    CHECK(tupleInstance->isTupleTypeValue());
                    CHECK_EQUAL(tupleInstance->getType(), a);
                }
            });
        });
    }

    TEST(VariantTypes)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                {
                    auto a = evaluateString("Int32 | Void");
                    auto b = evaluateString("Void | Int32");
                    CHECK_EQUAL(a, Int32::__staticType__());
                    CHECK_EQUAL(b, Int32::__staticType__());
                    CHECK_EQUAL(a, b);
                }

                {
                    CHECK_EQUAL(Type::getVoidType(), evaluateString("Void | Void"));
                }

                {
                    auto a = evaluateString("Int32 | Int64");
                    auto b = evaluateString("Int32 | Int64");
                    CHECK(a->isVariantType());
                    CHECK(b->isVariantType());
                    CHECK_EQUAL(a, b);
                }

                {
                    auto a = evaluateString("(Int32 | Int64) | Int8");
                    auto b = evaluateString("Int32 | (Int64 | Int8)");
                    CHECK(a->isVariantType());
                    CHECK(b->isVariantType());
                    CHECK_EQUAL(a, b);
                }

                {
                    auto a = evaluateString("Int32 | Int64 | Int8");
                    auto b = evaluateString("Int32 | Int64 | Int8 | Int64");
                    CHECK(a->isVariantType());
                    CHECK(b->isVariantType());
                    CHECK_EQUAL(a, b);
                }

                {
                    auto a = evaluateString("Int32 | Int64 | Int8");
                    auto b = evaluateString("Int32 const | Int64 volatile | Int8 restrict");
                    CHECK(a->isVariantType());
                    CHECK(b->isVariantType());
                    CHECK_EQUAL(a, b);
                }

                {
                    auto a = evaluateString("Int32 | Int64 | Int8");
                    auto b = evaluateString("Int32 | Int64 | Int8 | Int32 const | Int64 volatile | Int8 restrict");
                    CHECK(a->isVariantType());
                    CHECK(b->isVariantType());
                    CHECK_EQUAL(a, b);
                }

                {
                    CHECK(evaluateString("(Int32 | Int64 | Int8)(Int32(1))")->isVariantTypeValue());
                    CHECK(evaluateString("(Int32 | Int64 | Int8)(Int64(2))")->isVariantTypeValue());
                    CHECK(evaluateString("(Int32 | Int64 | Int8)(Int8(3))")->isVariantTypeValue());

                    CHECK_EQUAL(0u, evaluateStringWithValueOfType<uint8_t> ("(Int32 | Int64 | Int8)(Int32(5)) typeSelector"));
                    CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint8_t> ("(Int32 | Int64 | Int8)(Int64(5)) typeSelector"));
                    CHECK_EQUAL(2u, evaluateStringWithValueOfType<uint8_t> ("(Int32 | Int64 | Int8)(Int8(5)) typeSelector"));

                    CHECK_EQUAL(5, evaluateStringWithValueOfType<int32_t> ("(Int32 | Int64 | Int8)(Int32(5)) get: Int32"));
                    CHECK_EQUAL(5, evaluateStringWithValueOfType<int64_t> ("(Int32 | Int64 | Int8)(Int64(5)) get: Int64"));
                    CHECK_EQUAL(5, evaluateStringWithValueOfType<int8_t> ("(Int32 | Int64 | Int8)(Int8(5)) get: Int8"));
                }
            });
        });
    }

    TEST(FunctionTypes)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                {
                    auto a = evaluateString("Void => Void");
                    auto b = evaluateString("Void => Void");
                    CHECK(a->isFunctionType());
                    CHECK(b->isFunctionType());
                    CHECK_EQUAL(a, b);

                    auto functionType = staticObjectCast<FunctionType> (a);
                    CHECK_EQUAL(0u, functionType->getArgumentCount());
                    CHECK_EQUAL(Type::getVoidType(), functionType->getResultType());
                }

                {
                    auto a = evaluateString("(Int64 & Int32) => Float32");
                    auto b = evaluateString("(Int64 & Int32) => Float32");
                    CHECK(a->isFunctionType());
                    CHECK(b->isFunctionType());
                    CHECK_EQUAL(a, b);

                    auto functionType = staticObjectCast<FunctionType> (a);
                    CHECK_EQUAL(2u, functionType->getArgumentCount());
                    CHECK_EQUAL(Int64::__staticType__(), functionType->getArgument(0));
                    CHECK_EQUAL(Int32::__staticType__(), functionType->getArgument(1));
                    CHECK_EQUAL(Float32::__staticType__(), functionType->getResultType());
                }

                {
                    auto a = evaluateString("(Void => Void) closure");
                    auto b = evaluateString("(Void => Void) closure");
                    CHECK(a->isClosureType());
                    CHECK(b->isClosureType());
                    CHECK_EQUAL(a, b);

                    auto closureType = staticObjectCast<ClosureType> (a);
                    CHECK_EQUAL(0u, closureType->getArgumentCount());
                    CHECK_EQUAL(Type::getVoidType(), closureType->getResultType());
                }

                {
                    auto a = evaluateString("((Int64 & Int32) => Float32) closure");
                    auto b = evaluateString("((Int64 & Int32) => Float32) closure");
                    CHECK(a->isClosureType());
                    CHECK(b->isClosureType());
                    CHECK_EQUAL(a, b);

                    auto closureType = staticObjectCast<ClosureType> (a);
                    CHECK_EQUAL(2u, closureType->getArgumentCount());
                    CHECK_EQUAL(Int64::__staticType__(), closureType->getArgument(0));
                    CHECK_EQUAL(Int32::__staticType__(), closureType->getArgument(1));
                    CHECK_EQUAL(Float32::__staticType__(), closureType->getResultType());
                }

                {
                    auto a = evaluateString("(Void => Void) methodWithReceiver: Void");
                    auto b = evaluateString("(Void => Void) methodWithReceiver: Void");
                    CHECK(a->isMethodType());
                    CHECK(b->isMethodType());
                    CHECK_EQUAL(a, b);

                    auto methodType = staticObjectCast<MethodType> (a);
                    CHECK_EQUAL(Type::getVoidType(), methodType->getReceiverType());
                    CHECK_EQUAL(0u, methodType->getArgumentCount());
                    CHECK_EQUAL(Type::getVoidType(), methodType->getResultType());
                }

                {
                    auto a = evaluateString("((Int64 & Int32) => Float32) methodWithReceiver: UInt8");
                    auto b = evaluateString("((Int64 & Int32) => Float32) methodWithReceiver: UInt8");
                    CHECK(a->isMethodType());
                    CHECK(b->isMethodType());
                    CHECK_EQUAL(a, b);

                    auto methodType = staticObjectCast<MethodType> (a);
                    CHECK_EQUAL(2u, methodType->getArgumentCount());
                    CHECK_EQUAL(UInt8::__staticType__(), methodType->getReceiverType());
                    CHECK_EQUAL(Int64::__staticType__(), methodType->getArgument(0));
                    CHECK_EQUAL(Int32::__staticType__(), methodType->getArgument(1));
                    CHECK_EQUAL(Float32::__staticType__(), methodType->getResultType());
                }
            });
        });
    }

    TEST(ImmutableGlobalVariable)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK_EQUAL(42, evaluateStringWithValueOfType<int> ("public global A := 42"));
                CHECK_EQUAL(42, evaluateStringWithValueOfType<int> ("A"));
            });
        });
    }

    TEST(NullaryFunction)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
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
        RuntimeContext::createForScripting()->activeDuring([&](){
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
        RuntimeContext::createForScripting()->activeDuring([&](){
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
        RuntimeContext::createForScripting()->activeDuring([&](){
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
        RuntimeContext::createForScripting()->activeDuring([&](){
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

    TEST(FunctionReturningClosure)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK(evaluateString("public function makePlusN(N: Int32) => (Int32 => Int32) closure := function(x: Int32) => Int32 := x + N")->isFunctionTypeValue());
                CHECK(evaluateString("makePlusN(2)")->isClosureTypeValue());
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int32_t> ("makePlusN(0)(0)."));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("makePlusN(0)(1)."));
                CHECK_EQUAL(2, evaluateStringWithValueOfType<int32_t> ("makePlusN(0)(2)."));

                CHECK_EQUAL(2, evaluateStringWithValueOfType<int32_t> ("makePlusN(2)(0)."));
                CHECK_EQUAL(3, evaluateStringWithValueOfType<int32_t> ("makePlusN(2)(1)."));
                CHECK_EQUAL(4, evaluateStringWithValueOfType<int32_t> ("makePlusN(2)(2)."));

                CHECK(evaluateString("public function makePlusN2(N: Int32) => (Int32 => Int32) closure := {:(Int32)x :: Int32 | x + N}")->isFunctionTypeValue());
                CHECK(evaluateString("makePlusN2(2)")->isClosureTypeValue());
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int32_t> ("makePlusN2(0)(0)."));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("makePlusN2(0)(1)."));
                CHECK_EQUAL(2, evaluateStringWithValueOfType<int32_t> ("makePlusN2(0)(2)."));

                CHECK_EQUAL(2, evaluateStringWithValueOfType<int32_t> ("makePlusN2(2)(0)."));
                CHECK_EQUAL(3, evaluateStringWithValueOfType<int32_t> ("makePlusN2(2)(1)."));
                CHECK_EQUAL(4, evaluateStringWithValueOfType<int32_t> ("makePlusN2(2)(2)."));
            });
        });
    }

    TEST(PrimitiveTypeConstruction)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
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

                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int8_t> ("-1i8"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int8_t> ("0i8"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int8_t> ("1i8"));

                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int16_t> ("-1i16"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int16_t> ("0i16"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int16_t> ("1i16"));

                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int32_t> ("-1i32"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int32_t> ("0i32"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("1i32"));

                CHECK_EQUAL(-1, evaluateStringWithValueOfType<int64_t> ("-1i64"));
                CHECK_EQUAL(0, evaluateStringWithValueOfType<int64_t> ("0i64"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int64_t> ("1i64"));

                CHECK_EQUAL(0u, evaluateStringWithValueOfType<uint8_t> ("0u8"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint8_t> ("1u8"));

                CHECK_EQUAL(0u, evaluateStringWithValueOfType<uint16_t> ("0u16"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint16_t> ("1u16"));

                CHECK_EQUAL(0u, evaluateStringWithValueOfType<uint32_t> ("0u32"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint32_t> ("1u32"));

                CHECK_EQUAL(0u, evaluateStringWithValueOfType<uint64_t> ("0u64"));
                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint64_t> ("1u64"));

                CHECK_EQUAL(char(0), evaluateStringWithValueOfType<char> ("0c8"));
                CHECK_EQUAL(char(1), evaluateStringWithValueOfType<char> ("1c8"));

                CHECK_EQUAL(char16_t(0), evaluateStringWithValueOfType<char16_t> ("0c16"));
                CHECK_EQUAL(char16_t(1), evaluateStringWithValueOfType<char16_t> ("1c16"));

                CHECK_EQUAL(char32_t(0), evaluateStringWithValueOfType<char32_t> ("0c32"));
                CHECK_EQUAL(char32_t(1), evaluateStringWithValueOfType<char32_t> ("1c32"));

                CHECK_EQUAL(42, evaluateStringWithValueOfType<float> ("42f32"));
                CHECK_EQUAL(42.5f, evaluateStringWithValueOfType<float> ("42.5f32"));

                CHECK_EQUAL(42, evaluateStringWithValueOfType<double> ("42f64"));
                CHECK_EQUAL(42.5, evaluateStringWithValueOfType<double> ("42.5f64"));
            });
        });
    }

    TEST(PrimitiveTypeImplicitCastConstruction)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
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
        RuntimeContext::createForScripting()->activeDuring([&](){
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
        RuntimeContext::createForScripting()->activeDuring([&](){
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
        RuntimeContext::createForScripting()->activeDuring([&](){
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
        RuntimeContext::createForScripting()->activeDuring([&](){
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
        RuntimeContext::createForScripting()->activeDuring([&](){
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
        RuntimeContext::createForScripting()->activeDuring([&](){
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
        RuntimeContext::createForScripting()->activeDuring([&](){
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
        RuntimeContext::createForScripting()->activeDuring([&](){
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
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structDeclaration = evaluateString("public struct TestStruct.");
                CHECK(structDeclaration->isStructureType());
                
                auto structDefinition = evaluateString("public struct TestStruct definition: {}.");
                CHECK(structDefinition->isStructureType());
                CHECK_EQUAL(structDeclaration, structDefinition);
                CHECK_EQUAL(Module::getActive(), structDefinition.staticAs<StructureType> ()->getDefinitionModule());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(StructInNamespace)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structDeclaration = evaluateString("namespace TestNamespace definition: {public struct TestStruct}. TestNamespace TestStruct");
                CHECK(structDeclaration->isStructureType());
                
                auto structDefinition = evaluateString("namespace TestNamespace definition: {public struct TestStruct definition: {}}. TestNamespace TestStruct");
                CHECK(structDefinition->isStructureType());
                CHECK_EQUAL(structDeclaration, structDefinition);
                CHECK_EQUAL(Module::getActive(), structDefinition.staticAs<StructureType> ()->getDefinitionModule());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(Union)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto unionDeclaration = evaluateString("public union TestUnion.");
                CHECK(unionDeclaration->isUnionType());
                
                auto unionDefinition = evaluateString("public union TestUnion definition: {}.");
                CHECK(unionDefinition->isUnionType());
                CHECK_EQUAL(unionDeclaration, unionDefinition);
                CHECK_EQUAL(Module::getActive(), staticObjectCast<UnionType> (unionDefinition)->getDefinitionModule());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(Class)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto classDeclaration = evaluateString("public class TestClass.");
                CHECK(classDeclaration->isClassType());
                
                auto classDefinition = evaluateString("public class TestClass definition: {}.");
                CHECK(classDefinition->isClassType());
                CHECK_EQUAL(classDeclaration, classDefinition);
                CHECK_EQUAL(Module::getActive(), classDefinition.staticAs<ClassType> ()->getDefinitionModule());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(SuperClass)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto superclass = evaluateString("public class SuperClass definition: {}.");
                CHECK(superclass->isClassType());
                
                auto subclass = evaluateString("public class SubClass superclass: SuperClass; definition: {}.");
                CHECK(subclass->isClassType());
                CHECK_EQUAL(superclass, staticObjectCast<Type> (subclass)->getSupertype());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(Enum)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto enumDeclaration = evaluateString("public enum TestEnum.");
                CHECK(enumDeclaration->isEnumType());
                
                auto enumDefinition = evaluateString("public enum TestEnum valueType: UInt32; values: #{First: 1. Second:. #Third : 3 . Fifth: Third + 2}; definition: {}.");
                CHECK(enumDefinition->isEnumType());
                CHECK_EQUAL(enumDeclaration, enumDefinition);
                CHECK_EQUAL(UInt32::__staticType__(), enumDefinition.staticAs<EnumType> ()->getBaseType());
                CHECK_EQUAL(Module::getActive(), enumDefinition.staticAs<EnumType> ()->getDefinitionModule());

                CHECK_EQUAL(1u, evaluateStringWithValueOfType<uint32_t> ("TestEnum First"));
                CHECK_EQUAL(2u, evaluateStringWithValueOfType<uint32_t> ("TestEnum Second"));
                CHECK_EQUAL(3u, evaluateStringWithValueOfType<uint32_t> ("TestEnum Third"));
                CHECK_EQUAL(5u, evaluateStringWithValueOfType<uint32_t> ("TestEnum Fifth"));

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(StructField)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structDefinition = evaluateString("public struct TestStruct definition: {public field int32Field type: Int32}.");
                CHECK(structDefinition->isStructureType());

                auto structType = staticObjectCast<Type> (structDefinition);
                CHECK_EQUAL(1u, structType->getFieldCount());

                CHECK_EQUAL(4u, structType->getMemorySize());
                CHECK_EQUAL(4u, structType->getMemoryAlignment());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(StructPadding)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structDefinition = evaluateString("public struct TestStruct definition: {public field first type: Int8. public field second type: Int32}.");
                CHECK(structDefinition->isStructureType());

                auto structType = staticObjectCast<Type> (structDefinition);
                CHECK_EQUAL(2u, structType->getFieldCount());

                CHECK_EQUAL(8u, structType->getMemorySize());
                CHECK_EQUAL(4u, structType->getMemoryAlignment());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(PackedStructNoPadding)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structDefinition = evaluateString("public struct TestStruct packed definition: {public field first type: Int8. public field second type: Int32}.");
                CHECK(structDefinition->isStructureType());

                auto structType = staticObjectCast<Type> (structDefinition);
                CHECK_EQUAL(2u, structType->getFieldCount());

                CHECK_EQUAL(5u, structType->getMemorySize());
                CHECK_EQUAL(1u, structType->getMemoryAlignment());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(StructMethod)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structDefinition = evaluateString("public struct TestStruct definition: {public method square: x := x*x}.");
                CHECK(structDefinition->isStructureType());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(StructExtensionMethod)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
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
        RuntimeContext::createForScripting()->activeDuring([&](){
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
        RuntimeContext::createForScripting()->activeDuring([&](){
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
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structDefinition = evaluateString("public struct TestStruct definition: {}.");
                CHECK(structDefinition->isStructureType());

                auto structValue = evaluateString("TestStruct()");
                CHECK(structValue->isStructureTypeValue());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }
    TEST(StructureGetSelf)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK(evaluateString("struct TestStruct definition: {public method getSelf => TestStruct := self}. TestStruct() getSelf")->isStructureTypeValue());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(StructureSelfFields)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structDefinition = evaluateString(
"public struct TestStruct definition: {\n"
"field x type: Int32.\n"
"public method getX => Int32 := x.\n"
"public method setX: (newX: Int32) ::=> Void := {x := newX} \n"
"}.");
                CHECK(structDefinition->isStructureType());

                CHECK_EQUAL(0, evaluateStringWithValueOfType<int32_t> ("TestStruct() getX"));
                CHECK_EQUAL(5, evaluateStringWithValueOfType<int32_t> ("TestStruct() setX: 5; getX"));

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(StructureMacroMethod)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structDefinition = evaluateString("public struct TestStruct definition: {public macro method myYourself := self}.");
                CHECK(structDefinition->isStructureType());

                CHECK(evaluateString("TestStruct()")->isStructureTypeValue());
                CHECK(evaluateString("TestStruct() myYourself")->isStructureTypeValue());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(StructurePublicFields)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structDefinition = evaluateString("public struct TestStruct definition: {public field x type: Int32}.");
                CHECK(structDefinition->isStructureType());

                CHECK_EQUAL(4u, evaluateStringWithValueOfType<uint64_t> ("TestStruct memorySize"));
                CHECK_EQUAL(4u, evaluateStringWithValueOfType<uint64_t> ("TestStruct memoryAlignment"));

                CHECK_EQUAL(0, evaluateStringWithValueOfType<int32_t> ("TestStruct() x"));
                CHECK_EQUAL(5, evaluateStringWithValueOfType<int32_t> ("TestStruct() x: 5"));
                CHECK_EQUAL(5, evaluateStringWithValueOfType<int32_t> ("TestStruct() x: 5; x"));

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(CompileTimeConstant)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto compileTimeConstant = evaluateString("public compileTime constant Test := 42.");
                CHECK(compileTimeConstant->isCompileTimeConstant());
                CHECK_EQUAL(42, evaluateString("Test")->unwrapAsInt32());
            });
        });
    }

    TEST(ClassPublicFields)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto classDefinition = evaluateString("public class TestClass definition: {public field x type: Int32}.");
                CHECK(classDefinition->isClassType());

                CHECK_EQUAL(4u, evaluateStringWithValueOfType<uint64_t> ("TestClass memorySize"));
                CHECK_EQUAL(4u, evaluateStringWithValueOfType<uint64_t> ("TestClass memoryAlignment"));

                CHECK_EQUAL(0, evaluateStringWithValueOfType<int32_t> ("TestClass() x"));
                CHECK_EQUAL(5, evaluateStringWithValueOfType<int32_t> ("TestClass() x: 5"));
                CHECK_EQUAL(5, evaluateStringWithValueOfType<int32_t> ("TestClass() x: 5; x"));

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(InheritedMethod)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto superClass = evaluateString(R"(
public class SuperClass definition: {
    public method one => Int32 := 1.
}.
)");
                CHECK(superClass->isClassType());

                auto subClass = evaluateString("public class SubClass superclass: SuperClass; definition: {}.");
                CHECK(subClass->isClassType());

                CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("SuperClass() one"));
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("SubClass() one"));

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(InheritedField)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto superClass = evaluateString(R"(
public class SuperClass definition: {
    public field x type: Int32.
}.
)");
                CHECK(superClass->isClassType());

                CHECK_EQUAL(4u, evaluateStringWithValueOfType<uint64_t> ("SuperClass memorySize"));
                CHECK_EQUAL(4u, evaluateStringWithValueOfType<uint64_t> ("SuperClass memoryAlignment"));

                auto subClass = evaluateString(R"(
public class SubClass superclass: SuperClass; definition: {
    public field y type: Int32.
}.
)");
                CHECK(subClass->isClassType());

                CHECK_EQUAL(8u, evaluateStringWithValueOfType<uint64_t> ("SubClass memorySize"));
                CHECK_EQUAL(4u, evaluateStringWithValueOfType<uint64_t> ("SubClass memoryAlignment"));

                CHECK_EQUAL(2, evaluateStringWithValueOfType<int32_t> ("SuperClass() x: 2; x"));
                CHECK_EQUAL(2, evaluateStringWithValueOfType<int32_t> ("SubClass() x: 2; y: 4; x"));
                CHECK_EQUAL(4, evaluateStringWithValueOfType<int32_t> ("SubClass() x: 2; y: 4; y"));

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(TemplateStructure)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto templateEntity = evaluateString(
"public template ValueBox (VT: Type) := struct definition: {\n"
"public compileTime constant ValueType := VT.\n"
"public field value type: ValueType.\n"
"}.\n");
                CHECK(templateEntity->isTemplate());

                auto int32ValueBox = evaluateString("ValueBox(Int32)");
                auto int32ValueBox2 = evaluateString("ValueBox(Int32)");
                CHECK(int32ValueBox->isStructureType());
                CHECK_EQUAL(int32ValueBox, int32ValueBox2);

                auto int64ValueBox = evaluateString("ValueBox(Int64)");
                auto int64ValueBox2 = evaluateString("ValueBox(Int64)");
                CHECK(int64ValueBox->isStructureType());
                CHECK_EQUAL(int64ValueBox, int64ValueBox2);
                CHECK(int32ValueBox != int64ValueBox);

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(ClosureArgumentPassing)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                CHECK((std::vector<int> {1, 4, 9, 16, 25}) == evaluateStringWithValueOfType<std::vector<int>> ("#(1 2 3 4 5) collect: (function(x) := x*x)"));
                CHECK((std::vector<int> {2, 4}) == evaluateStringWithValueOfType<std::vector<int>> ("#(1 2 3 4 5) select: (function(x) := x%2 = 0)"));
                CHECK((std::vector<int> {1, 3, 5}) == evaluateStringWithValueOfType<std::vector<int>> ("#(1 2 3 4 5) select: (function(x) := x%2 = 1)"));

                CHECK((std::vector<int> {1, 4, 9, 16, 25}) == evaluateStringWithValueOfType<std::vector<int>> ("#(1 2 3 4 5) collect: {:x | x*x }"));
                CHECK((std::vector<int> {2, 4}) == evaluateStringWithValueOfType<std::vector<int>> ("#(1 2 3 4 5) select: {:x | x%2 = 0 }"));
                CHECK((std::vector<int> {1, 3, 5}) == evaluateStringWithValueOfType<std::vector<int>> ("#(1 2 3 4 5) select: {:x | x%2 = 1 }"));
            });
        });
    }

    TEST(NonTrivialInitialization)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structureType = evaluateStringWithValueOfType<TypePtr> ("public function sideEffect() => Void := {}. struct TestStruct definition: {public method initialize => Void := sideEffect().}");
                CHECK(!structureType->hasTrivialInitialization());

                auto initializeMethod = structureType->getInitializeMethod();
                CHECK(!structureType->isCompiledMethod());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(NonTrivialFinalizer)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structureType = evaluateStringWithValueOfType<TypePtr> ("public function sideEffect() => Void := {}. struct TestStruct definition: {public method finalize => Void := sideEffect().}");
                CHECK(!structureType->hasTrivialFinalization());

                auto finalizedMethod = structureType->getFinalizeMethod();
                CHECK(!structureType->isCompiledMethod());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(NonTrivialInitializeCopyingFrom)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structureType = evaluateStringWithValueOfType<TypePtr> ("public function sideEffect() => Void := {}. struct TestStruct definition: {public method initializeCopyingFrom: (other: TestStruct const ref) ::=> Void := sideEffect().}");
                CHECK(!structureType->hasTrivialInitializationCopyingFrom());

                auto initializeMethod = structureType->getInitializeCopyingFromMethod();
                CHECK(!structureType->isCompiledMethod());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(NonTrivialInitializeMovingFrom)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structureType = evaluateStringWithValueOfType<TypePtr> ("public function sideEffect() => Void := {}. struct TestStruct definition: {public method initializeMovingFrom: (other: TestStruct tempRef) ::=> Void := sideEffect().}");
                CHECK(!structureType->hasTrivialInitializationMovingFrom());

                auto initializeMethod = structureType->getInitializeMovingFromMethod();
                CHECK(!structureType->isCompiledMethod());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(NonTrivialAssignCopyingFrom)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structureType = evaluateStringWithValueOfType<TypePtr> ("public function sideEffect() => Void := {}. struct TestStruct definition: {(public method := (other: TestStruct const ref)) => TestStruct const ref := {sideEffect() . self}.}");
                CHECK(!structureType->hasTrivialAssignCopyingFrom());
                CHECK(structureType->getAssignCopyingFromMethod()->isCompiledMethod());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(NonTrivialAssignMovingFrom)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                auto structureType = evaluateStringWithValueOfType<TypePtr> ("public function sideEffect() => Void := {}. struct TestStruct definition: {(public method := (other: TestStruct tempRef)) => TestStruct const ref := {sideEffect() . self}.}");
                CHECK(!structureType->hasTrivialAssignMovingFrom());
                CHECK(structureType->getAssignMovingFromMethod()->isCompiledMethod());

                Module::getActive()->analyzeAllPendingProgramEntities();
            });
        });
    }

    TEST(QuasiQuote)
    {
        RuntimeContext::createForScripting()->activeDuring([&](){
            ScriptModule::create()->activeDuring([&](){
                evaluateString(R"(
Boolean8 extend: {
    public macro method ifTrue: trueBlock ifFalse: falseBlock := ``(if: `,self then: `,trueBlock else: `, falseBlock)
}
)");
                CHECK_EQUAL(1, evaluateStringWithValueOfType<int32_t> ("Boolean8(true) ifTrue: Int32(1) ifFalse: Int32(2)"));
                CHECK_EQUAL(2, evaluateStringWithValueOfType<int32_t> ("Boolean8(false) ifTrue: Int32(1) ifFalse: Int32(2)"));
            });
        });
    }

}
