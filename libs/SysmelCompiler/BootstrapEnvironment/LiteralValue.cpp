#include "sysmel/BootstrapEnvironment/LiteralValue.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<LiteralValue> literalValueTypeRegistration;

TypePtr Type::getLiteralValueType()
{
    return LiteralValue::__staticType__();
}

bool LiteralValue::isLiteralValue() const
{
    return true;
}

bool LiteralValue::isPureCompileTimeLiteralValue() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius