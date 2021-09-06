#include "sysmel/ObjectModel/LiteralInteger.hpp"
#include "sysmel/ObjectModel/LiteralNegativeInteger.hpp"
#include "sysmel/ObjectModel/LiteralPositiveInteger.hpp"
#include "sysmel/ObjectModel/LiteralCharacter.hpp"

#include "sysmel/ObjectModel/LiteralSymbol.hpp"
#include "sysmel/ObjectModel/LiteralInteger.hpp"
#include "sysmel/ObjectModel/Error.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace ObjectModel
{
static BootstrapTypeRegistration<LiteralInteger> literalIntegerTypeRegistration;

MethodCategories LiteralInteger::__instanceMethods__()
{
    return MethodCategories{
        {"arithmetic", {
            makeMethodBinding<LargeInteger (LargeInteger)> ("negated", +[](const LargeInteger &value) {
                return -value;
            })
        }},

        {"math functions", {
            makeMethodBinding<LargeInteger (LargeInteger)> ("factorial", +[](const LargeInteger &value) {
                return value.factorial();
            })
        }}
    };
}

LiteralIntegerPtr LiteralInteger::makeFor(const LargeInteger &value)
{
    LiteralIntegerPtr result;
    if(value.isNegative())
        result = std::make_shared<LiteralNegativeInteger> ();
    else
        result = std::make_shared<LiteralPositiveInteger> ();
    result->value = value;
    return result;
}

LiteralIntegerPtr LiteralInteger::makeFor(LargeInteger &&value)
{
    LiteralIntegerPtr result;
    if(value.isNegative())
        result = std::make_shared<LiteralNegativeInteger> ();
    else
        result = std::make_shared<LiteralPositiveInteger> ();
    result->value = std::move(value);
    return result;
}

LiteralIntegerPtr LiteralInteger::makeForCharacter(char32_t value)
{
    LiteralIntegerPtr result = std::make_shared<LiteralCharacter> ();
    result->value = LargeInteger{value};
    return result;
}

bool LiteralInteger::isLiteralInteger() const
{
    return true;
}

std::string LiteralInteger::printString() const
{
    return value.asString();
}

uint8_t LiteralInteger::unwrapAsUInt8() const
{
    throw CannotUnwrap();
}

int8_t LiteralInteger::unwrapAsInt8() const
{
    throw CannotUnwrap();
}

uint16_t LiteralInteger::unwrapAsUInt16() const
{
    throw CannotUnwrap();
}

int16_t LiteralInteger::unwrapAsInt16() const
{
    throw CannotUnwrap();
}

uint32_t LiteralInteger::unwrapAsUInt32() const
{
    throw CannotUnwrap();
}

int32_t LiteralInteger::unwrapAsInt32() const
{
    throw CannotUnwrap();
}

uint64_t LiteralInteger::unwrapAsUInt64() const
{
    throw CannotUnwrap();
}

int64_t LiteralInteger::unwrapAsInt64() const
{
    throw CannotUnwrap();
}

LargeInteger LiteralInteger::unwrapAsLargeInteger() const
{
    return value;
}

char LiteralInteger::unwrapAsChar8() const
{
    throw CannotUnwrap();
}

char16_t LiteralInteger::unwrapAsChar16() const
{
    throw CannotUnwrap();
}

char32_t LiteralInteger::unwrapAsChar32() const
{
    throw CannotUnwrap();
}

float LiteralInteger::unwrapAsFloat32() const
{
    throw CannotUnwrap();
}

double LiteralInteger::unwrapAsFloat64() const
{
    throw CannotUnwrap();
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius