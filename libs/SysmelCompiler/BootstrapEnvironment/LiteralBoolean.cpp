#include "sysmel/BootstrapEnvironment/LiteralBoolean.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<LiteralBoolean> literalBooleanRegistration;

TypePtr WrapperTypeFor<bool>::apply()
{
    return LiteralBoolean::__staticType__();
}

bool LiteralBoolean::isLiteralBoolean() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius