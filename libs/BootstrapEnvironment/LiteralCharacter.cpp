#include "sysmel/BootstrapEnvironment/LiteralCharacter.hpp"
#include "sysmel/BootstrapEnvironment/StringUtilities.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include <algorithm>
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<LiteralCharacter> literalCharacterTypeRegistration;

TypePtr WrapperTypeFor<char32_t>::apply()
{
    return LiteralCharacter::__staticType__();
}

bool LiteralCharacter::isLiteralCharacter() const
{
    return true;
}

MethodCategories LiteralCharacter::__instanceMethods__()
{
    return MethodCategories{ };
}

std::string LiteralCharacter::asString() const
{
    std::string result;
    result.reserve(1);
    result[0] = value.wordAt(0);
    return result;
}

std::string LiteralCharacter::printString() const
{
    std::ostringstream out;
    out << '\'';
    formatUtf32Character(value.wordAt(0), out);
    out << '\'';
    return out.str();
}

SExpression LiteralCharacter::asSExpression() const
{
    return unwrapAsChar32();
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius