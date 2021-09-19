#include "sysmel/BootstrapEnvironment/LiteralNegativeInteger.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<LiteralNegativeInteger> literalNegativeIntegerTypeRegistration;

bool LiteralNegativeInteger::isLiteralNegativeInteger() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius