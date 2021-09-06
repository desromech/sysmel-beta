#include "sysmel/ObjectModel/LiteralPositiveInteger.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace ObjectModel
{
static BootstrapTypeRegistration<LiteralPositiveInteger> literalPositiveIntegerRegistration;

bool LiteralPositiveInteger::isLiteralPositiveInteger() const
{
    return true;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius