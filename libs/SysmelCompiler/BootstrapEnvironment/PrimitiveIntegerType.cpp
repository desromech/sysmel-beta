#include "sysmel/BootstrapEnvironment/PrimitiveIntegerType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/StringUtilities.hpp"
#include "IntrinsicPrimitiveIntegerMethods.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

bool PrimitiveIntegerType::isPrimitiveIntegerTypeValue() const
{
    return true;
}

MethodCategories UInt8::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<UInt8>::instanceMethods();
}

MethodCategories UInt8::__typeMacroMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<UInt8>::typeMacroMethods();
}

uint8_t UInt8::unwrapAsUInt8() const
{
    return value;
}

std::string UInt8::asString() const
{
    return castToString(value);
}

std::string UInt8::printString() const
{
    return "UInt8(" + castToString(value) + ")";
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

uint16_t UInt16::unwrapAsUInt16() const
{
    return value;
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

uint32_t UInt32::unwrapAsUInt32() const
{
    return value;
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

uint64_t UInt64::unwrapAsUInt64() const
{
    return value;
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

int8_t Int8::unwrapAsInt8() const
{
    return value;
}

std::string Int8::asString() const
{
    return castToString(value);
}

std::string Int8::printString() const
{
    return "Int8(" + castToString(value) + ")";
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

int16_t Int16::unwrapAsInt16() const
{
    return value;
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

MethodCategories Int32::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<Int32>::instanceMethods();
}

MethodCategories Int32::__typeMacroMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<Int32>::typeMacroMethods();
}

int32_t Int32::unwrapAsInt32() const
{
    return value;
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

int64_t Int64::unwrapAsInt64() const
{
    return value;
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
} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius