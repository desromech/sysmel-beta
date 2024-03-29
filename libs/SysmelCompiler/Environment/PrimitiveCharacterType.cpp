#include "Environment/PrimitiveCharacterType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/StringUtilities.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include "IntrinsicPrimitiveIntegerMethods.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<PrimitiveCharacterType> PrimitiveCharacterTypeRegistration;
static BootstrapTypeRegistration<Char8> Char8TypeRegistration;
static BootstrapTypeRegistration<Char16> Char16TypeRegistration;
static BootstrapTypeRegistration<Char32> Char32TypeRegistration;

bool PrimitiveCharacterType::isPrimitiveCharacterTypeValue() const
{
    return true;
}

AnyValuePtr PrimitiveCharacterType::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitPrimitiveCharacterType(selfFromThis());
}

MethodCategories Char8::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<Char8>::instanceMethods();
}

MethodCategories Char8::__typeMacroMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<Char8>::typeMacroMethods();
}

AnyValuePtrList Char8::__constructors__()
{
    return IntrinsicPrimitiveIntegerMethods<Char8>::constructors();
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

char32_t Char8::unwrapAsChar32() const
{
    return value;
}

LargeInteger Char8::unwrapAsLargeInteger() const
{
    return LargeInteger{value};
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

AnyValuePtrList Char16::__constructors__()
{
    return IntrinsicPrimitiveIntegerMethods<Char16>::constructors();
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

char32_t Char16::unwrapAsChar32() const
{
    return value;
}

LargeInteger Char16::unwrapAsLargeInteger() const
{
    return LargeInteger{value};
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

AnyValuePtrList Char32::__constructors__()
{
    return IntrinsicPrimitiveIntegerMethods<Char32>::constructors();
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

LargeInteger Char32::unwrapAsLargeInteger() const
{
    return LargeInteger{value};
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
} // End of namespace Environment
} // End of namespace Sysmel