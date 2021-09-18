#include "sysmel/ObjectModel/LiteralBoolean.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace ObjectModel
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

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius