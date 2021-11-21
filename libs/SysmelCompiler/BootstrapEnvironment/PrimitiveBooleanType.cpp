#include "sysmel/BootstrapEnvironment/PrimitiveBooleanType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<PrimitiveBooleanType> PrimitiveBooleanTypeTypeRegistration;
static BootstrapTypeRegistration<Boolean8> Boolean8TypeRegistration;

bool PrimitiveBooleanType::isPrimitiveBooleanTypeValue() const
{
    return true;
}

Boolean8Ptr Boolean8::make(bool value)
{
    auto result = std::make_shared<Boolean8> ();
    result->value = value;
    return result;
}

bool Boolean8::unwrapAsBoolean() const
{
    return value;
}

std::string Boolean8::asString() const
{
    return value ? "true" : "false";
}

std::string Boolean8::printString() const
{
    return std::string("Boolean8(") + (value ? "true" : "false") + ")";
}

SExpression Boolean8::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"boolean8"}},
        value
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius