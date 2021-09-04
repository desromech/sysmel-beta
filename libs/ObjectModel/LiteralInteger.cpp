#include "sysmel/ObjectModel/LiteralInteger.hpp"
#include "sysmel/ObjectModel/LiteralNegativeInteger.hpp"
#include "sysmel/ObjectModel/LiteralPositiveInteger.hpp"
#include "sysmel/ObjectModel/LiteralCharacter.hpp"

#include "sysmel/ObjectModel/LiteralSymbol.hpp"
#include "sysmel/ObjectModel/LiteralInteger.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace ObjectModel
{

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

ObjectPtr LiteralInteger::performWithArguments(const ObjectPtr &selector, const std::vector<ObjectPtr> &arguments)
{
    static auto factorialSymbol = LiteralSymbol::intern("factorial");
    if(selector == factorialSymbol)
    {
        return makeFor(value.factorial());
    }
    return LiteralNumber::performWithArguments(selector, arguments);
}
} // End of namespace ObjectModel
} // End of namespace SysmelMoebius