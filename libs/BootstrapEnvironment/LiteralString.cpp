#include "sysmel/BootstrapEnvironment/LiteralString.hpp"
#include "sysmel/BootstrapEnvironment/StringUtilities.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<LiteralString> literalStringTypeRegistration;

TypePtr WrapperTypeFor<std::string>::apply()
{
    return LiteralString::__staticType__();
}

LiteralStringPtr LiteralString::makeFor(const std::string &value)
{
    auto result = std::make_shared<LiteralString> ();
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
    std::ostringstream out;
    out << '"';
    for(auto c : value)
    {
        formatUtf8Character(c, out);
    }
    out << '"';
    return out.str();
}

std::string LiteralString::unwrapAsString() const
{
    return value;
}

SExpression LiteralString::asSExpression() const
{
    return value;
}
} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius