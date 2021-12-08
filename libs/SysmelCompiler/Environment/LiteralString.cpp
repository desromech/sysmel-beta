#include "Environment/LiteralString.hpp"
#include "Environment/StringUtilities.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<LiteralString> literalStringTypeRegistration;

TypePtr WrapperTypeFor<std::string>::apply()
{
    return LiteralString::__staticType__();
}

LiteralStringPtr LiteralString::makeFor(const std::string &value)
{
    auto result = basicMakeObject<LiteralString> ();
    result->value = value;
    return result;
}

bool LiteralString::isLiteralString() const
{
    return true;
}

std::string LiteralString::asString() const
{
    return value;
}

std::string LiteralString::printString() const
{
    return formatStringLiteral(value);
}

std::string LiteralString::unwrapAsString() const
{
    return value;
}

SExpression LiteralString::asSExpression() const
{
    return value;
}
} // End of namespace Environment
} // End of namespace Sysmel