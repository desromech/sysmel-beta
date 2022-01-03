#include "Environment/PrimitiveIntegerType.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/StringUtilities.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include "IntrinsicPrimitiveIntegerMethods.hpp"

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<PrimitiveIntegerType> PrimitiveIntegerTypeTypeRegistration;
static BootstrapTypeRegistration<UInt8> UInt8TypeRegistration;
static BootstrapTypeRegistration<UInt16> UInt16TypeRegistration;
static BootstrapTypeRegistration<UInt32> UInt32TypeRegistration;
static BootstrapTypeRegistration<UInt64> UInt64TypeRegistration;

static BootstrapTypeRegistration<Int8> Int8TypeRegistration;
static BootstrapTypeRegistration<Int16> Int16TypeRegistration;
static BootstrapTypeRegistration<Int32> Int32TypeRegistration;
static BootstrapTypeRegistration<Int64> Int64TypeRegistration;

TypePtr WrapperTypeFor<UIntPointerValue>::apply()
{
    return Type::getUIntPointerType();
}

TypePtr WrapperTypeFor<IntPointerValue>::apply()
{
    return Type::getIntPointerType();
}

AnyValuePtr WrapValue<UIntPointerValue>::apply(UIntPointerValue value)
{
    if(RuntimeContext::getActive()->getTargetDescription().pointerSize == 4)
    {
        auto result = basicMakeObject<UInt32> ();
        result->value = uint32_t(value.value);
        return result;
    }
    else
    {
        auto result = basicMakeObject<UInt64> ();
        result->value = uint64_t(value.value);
        return result;
    }
}

AnyValuePtr WrapValue<IntPointerValue>::apply(IntPointerValue value)
{
    if(RuntimeContext::getActive()->getTargetDescription().pointerSize == 4)
    {
        auto result = basicMakeObject<Int32> ();
        result->value = value.value;
        return result;
    }
    else
    {
        auto result = basicMakeObject<Int64> ();
        result->value = value.value;
        return result;
    }
}

bool PrimitiveIntegerType::isPrimitiveIntegerTypeValue() const
{
    return true;
}

AnyValuePtr PrimitiveIntegerType::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitPrimitiveIntegerType(selfFromThis());
}

TypePtr Type::getIntPointerType()
{
    return RuntimeContext::getActive()->getTargetDescription().pointerSize == 4 ? Int32::__staticType__() : Int64::__staticType__();
}

TypePtr Type::getUIntPointerType()
{
    return RuntimeContext::getActive()->getTargetDescription().pointerSize == 4 ? UInt32::__staticType__() : UInt64::__staticType__();
}

MethodCategories UInt8::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<UInt8>::instanceMethods();
}

MethodCategories UInt8::__typeMacroMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<UInt8>::typeMacroMethods();
}

AnyValuePtrList UInt8::__constructors__()
{
    return IntrinsicPrimitiveIntegerMethods<UInt8>::constructors();
}

bool UInt8::__canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveIntegerMethods<UInt8>::canBeInstantiatedWithLiteralValue(value);
}

AnyValuePtr UInt8::__instantiateWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveIntegerMethods<UInt8>::instantiateWithLiteralValue(value);
}

uint8_t UInt8::unwrapAsUInt8() const
{
    return value;
}

uint64_t UInt8::unwrapAsUInt64() const
{
    return value;
}

LargeInteger UInt8::unwrapAsLargeInteger() const
{
    return LargeInteger{value};
}

std::string UInt8::asString() const
{
    return castToString(int(value));
}

std::string UInt8::printString() const
{
    return "UInt8(" + castToString(int(value)) + ")";
}

SExpression UInt8::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"uint8"}},
        LargeInteger{value}
    }};
}

MethodCategories UInt16::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<UInt16>::instanceMethods();
}

MethodCategories UInt16::__typeMacroMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<UInt16>::typeMacroMethods();
}

AnyValuePtrList UInt16::__constructors__()
{
    return IntrinsicPrimitiveIntegerMethods<UInt16>::constructors();
}

bool UInt16::__canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveIntegerMethods<UInt16>::canBeInstantiatedWithLiteralValue(value);
}

AnyValuePtr UInt16::__instantiateWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveIntegerMethods<UInt16>::instantiateWithLiteralValue(value);
}

uint16_t UInt16::unwrapAsUInt16() const
{
    return value;
}

uint64_t UInt16::unwrapAsUInt64() const
{
    return value;
}

LargeInteger UInt16::unwrapAsLargeInteger() const
{
    return LargeInteger{value};
}

std::string UInt16::asString() const
{
    return castToString(value);
}

std::string UInt16::printString() const
{
    return "UInt16(" + castToString(value) + ")";
}

SExpression UInt16::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"uint16"}},
        LargeInteger{value}
    }};
}

MethodCategories UInt32::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<UInt32>::instanceMethods();
}

MethodCategories UInt32::__typeMacroMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<UInt32>::typeMacroMethods();
}

AnyValuePtrList UInt32::__constructors__()
{
    return IntrinsicPrimitiveIntegerMethods<UInt32>::constructors();
}

bool UInt32::__canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveIntegerMethods<UInt32>::canBeInstantiatedWithLiteralValue(value);
}

AnyValuePtr UInt32::__instantiateWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveIntegerMethods<UInt32>::instantiateWithLiteralValue(value);
}

uint32_t UInt32::unwrapAsUInt32() const
{
    return value;
}

uint64_t UInt32::unwrapAsUInt64() const
{
    return value;
}

LargeInteger UInt32::unwrapAsLargeInteger() const
{
    return LargeInteger{value};
}

std::string UInt32::asString() const
{
    return castToString(value);
}

std::string UInt32::printString() const
{
    return "UInt32(" + castToString(value) + ")";
}

SExpression UInt32::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"uint32"}},
        LargeInteger{value}
    }};
}

MethodCategories UInt64::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<UInt64>::instanceMethods();
}

MethodCategories UInt64::__typeMacroMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<UInt64>::typeMacroMethods();
}

AnyValuePtrList UInt64::__constructors__()
{
    return IntrinsicPrimitiveIntegerMethods<UInt64>::constructors();
}

bool UInt64::__canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveIntegerMethods<UInt64>::canBeInstantiatedWithLiteralValue(value);
}

AnyValuePtr UInt64::__instantiateWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveIntegerMethods<UInt64>::instantiateWithLiteralValue(value);
}

uint64_t UInt64::unwrapAsUInt64() const
{
    return value;
}

LargeInteger UInt64::unwrapAsLargeInteger() const
{
    return LargeInteger{value};
}

std::string UInt64::asString() const
{
    return castToString(value);
}

std::string UInt64::printString() const
{
    return "UInt64(" + castToString(value) + ")";
}

SExpression UInt64::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"uint64"}},
        LargeInteger{value}
    }};
}

MethodCategories Int8::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<Int8>::instanceMethods();
}

MethodCategories Int8::__typeMacroMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<Int8>::typeMacroMethods();
}

AnyValuePtrList Int8::__constructors__()
{
    return IntrinsicPrimitiveIntegerMethods<Int8>::constructors();
}

bool Int8::__canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveIntegerMethods<Int8>::canBeInstantiatedWithLiteralValue(value);
}

AnyValuePtr Int8::__instantiateWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveIntegerMethods<Int8>::instantiateWithLiteralValue(value);
}

int8_t Int8::unwrapAsInt8() const
{
    return value;
}

LargeInteger Int8::unwrapAsLargeInteger() const
{
    return LargeInteger{value};
}

std::string Int8::asString() const
{
    return castToString(int(value));
}

std::string Int8::printString() const
{
    return "Int8(" + castToString(int(value)) + ")";
}

SExpression Int8::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"int8"}},
        LargeInteger{value}
    }};
}


MethodCategories Int16::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<Int16>::instanceMethods();
}

MethodCategories Int16::__typeMacroMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<Int16>::typeMacroMethods();
}

AnyValuePtrList Int16::__constructors__()
{
    return IntrinsicPrimitiveIntegerMethods<Int16>::constructors();
}

bool Int16::__canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveIntegerMethods<Int16>::canBeInstantiatedWithLiteralValue(value);
}

AnyValuePtr Int16::__instantiateWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveIntegerMethods<Int16>::instantiateWithLiteralValue(value);
}

int16_t Int16::unwrapAsInt16() const
{
    return value;
}

LargeInteger Int16::unwrapAsLargeInteger() const
{
    return LargeInteger{value};
}

std::string Int16::asString() const
{
    return castToString(value);
}

std::string Int16::printString() const
{
    return "Int16(" + castToString(value) + ")";
}

SExpression Int16::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"int16"}},
        LargeInteger{value}
    }};
}

AnyValuePtrList Int32::__constructors__()
{
    return IntrinsicPrimitiveIntegerMethods<Int32>::constructors();
}

MethodCategories Int32::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<Int32>::instanceMethods();
}

MethodCategories Int32::__typeMacroMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<Int32>::typeMacroMethods();
}

bool Int32::__canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveIntegerMethods<Int32>::canBeInstantiatedWithLiteralValue(value);
}

AnyValuePtr Int32::__instantiateWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveIntegerMethods<Int32>::instantiateWithLiteralValue(value);
}

int32_t Int32::unwrapAsInt32() const
{
    return value;
}

LargeInteger Int32::unwrapAsLargeInteger() const
{
    return LargeInteger{value};
}

std::string Int32::asString() const
{
    return castToString(value);
}

std::string Int32::printString() const
{
    return "Int32(" + castToString(value) + ")";
}

SExpression Int32::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"int32"}},
        LargeInteger{value}
    }};
}

MethodCategories Int64::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<Int64>::instanceMethods();
}

MethodCategories Int64::__typeMacroMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<Int64>::typeMacroMethods();
}

AnyValuePtrList Int64::__constructors__()
{
    return IntrinsicPrimitiveIntegerMethods<Int64>::constructors();
}

bool Int64::__canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveIntegerMethods<Int64>::canBeInstantiatedWithLiteralValue(value);
}

AnyValuePtr Int64::__instantiateWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveIntegerMethods<Int64>::instantiateWithLiteralValue(value);
}

int64_t Int64::unwrapAsInt64() const
{
    return value;
}

LargeInteger Int64::unwrapAsLargeInteger() const
{
    return LargeInteger{value};
}

std::string Int64::asString() const
{
    return castToString(value);
}

std::string Int64::printString() const
{
    return "Int64(" + castToString(value) + ")";
}

SExpression Int64::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"int64"}},
        LargeInteger{value}
    }};
}
} // End of namespace Environment
} // End of namespace Sysmel