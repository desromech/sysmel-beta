#include "sysmel/ObjectModel/LiteralNegativeInteger.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace ObjectModel
{
static BootstrapTypeRegistration<LiteralNegativeInteger> literalNegativeIntegerTypeRegistration;

bool LiteralNegativeInteger::isLiteralNegativeInteger() const
{
    return true;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius