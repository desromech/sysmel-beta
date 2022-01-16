#include "Environment/LiteralString.hpp"
#include "Environment/StringUtilities.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/LiteralValueVisitor.hpp"

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<LiteralString> literalStringTypeRegistration;

TypePtr WrapperTypeFor<std::string>::apply()
{
    return LiteralString::__staticType__();
}

MethodCategories LiteralString::__instanceMethods__()
{
    return MethodCategories{
        {"accessing", {
            makeMethodBinding<size_t (std::string)> ("size", [](const std::string &self) {
                return self.size();
            }, MethodFlags::Pure),
        }},

        {"string operations", {
            makeMethodBinding<std::string (std::string, std::string)> ("--", [](const std::string &a, const std::string &b) {
                return a + b;
            }, MethodFlags::Pure),

            makeMethodBinding<std::string (std::string, uint64_t)> ("first:", [](const std::string &s, uint64_t count) {
                return s.substr(0, count);
            }, MethodFlags::Pure),

            makeMethodBinding<std::string (std::string, uint64_t)> ("allButFirst:", [](const std::string &s, uint64_t count) {
                return s.substr(count);
            }, MethodFlags::Pure),
        }}
    };
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

AnyValuePtr LiteralString::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitLiteralString(selfFromThis());
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