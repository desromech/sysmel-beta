#include "sysmel/ObjectModel/LiteralInteger.hpp"
#include "sysmel/ObjectModel/LiteralNegativeInteger.hpp"
#include "sysmel/ObjectModel/LiteralPositiveInteger.hpp"
#include "sysmel/ObjectModel/LiteralCharacter.hpp"

#include "sysmel/ObjectModel/LiteralSymbol.hpp"
#include "sysmel/ObjectModel/LiteralInteger.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace ObjectModel
{
static BootstrapTypeRegistration<LiteralInteger> literalIntegerTypeRegistration;

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

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius