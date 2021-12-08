#include "Environment/LiteralBoolean.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel