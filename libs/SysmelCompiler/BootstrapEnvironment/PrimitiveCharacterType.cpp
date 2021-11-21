#include "sysmel/BootstrapEnvironment/PrimitiveCharacterType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/StringUtilities.hpp"
#include "IntrinsicPrimitiveIntegerMethods.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<PrimitiveCharacterType> PrimitiveCharacterTypeRegistration;
static BootstrapTypeRegistration<Char8> Char8TypeRegistration;
static BootstrapTypeRegistration<Char16> Char16TypeRegistration;
static BootstrapTypeRegistration<Char32> Char32TypeRegistration;

bool PrimitiveCharacterType::isPrimitiveCharacterTypeValue() const
{
    return true;
}

MethodCategories Char8::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<Char8>::instanceMethods();
}

MethodCategories Char8::__typeMacroMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<Char8>::typeMacroMethods();
}

bool Char8::__canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveIntegerMethods<Char8>::canBeInstantiatedWithLiteralValue(value);
}

AnyValuePtr Char8::__instantiateWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveIntegerMethods<Char8>::instantiateWithLiteralValue(value);
}

char Char8::unwrapAsChar8() const
{
    return value;
}

std::string Char8::asString() const
{
    return castToString(value);
}

std::string Char8::printString() const
{
    return "Char8(" + castToString(value) + ")";
}

SExpression Char8::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"char8"}},
        LargeInteger{value}
    }};
}

MethodCategories Char16::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<Char16>::instanceMethods();
}

MethodCategories Char16::__typeMacroMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<Char16>::typeMacroMethods();
}

bool Char16::__canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveIntegerMethods<Char16>::canBeInstantiatedWithLiteralValue(value);
}

AnyValuePtr Char16::__instantiateWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveIntegerMethods<Char16>::instantiateWithLiteralValue(value);
}

char16_t Char16::unwrapAsChar16() const
{
    return value;
}

std::string Char16::asString() const
{
    return castToString(value);
}

std::string Char16::printString() const
{
    return "Char16(" + castToString(value) + ")";
}

SExpression Char16::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"char16"}},
        LargeInteger{value}
    }};
}

MethodCategories Char32::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<Char32>::instanceMethods();
}

MethodCategories Char32::__typeMacroMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<Char32>::typeMacroMethods();
}

bool Char32::__canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveIntegerMethods<Char32>::canBeInstantiatedWithLiteralValue(value);
}

AnyValuePtr Char32::__instantiateWithLiteralValue__(const AnyValuePtr &value)
{
    return IntrinsicPrimitiveIntegerMethods<Char32>::instantiateWithLiteralValue(value);
}

char32_t Char32::unwrapAsChar32() const
{
    return value;
}

std::string Char32::asString() const
{
    return castToString(value);
}

std::string Char32::printString() const
{
    return "Char32(" + castToString(value) + ")";
}

SExpression Char32::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"char32"}},
        LargeInteger{value}
    }};
}
} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius