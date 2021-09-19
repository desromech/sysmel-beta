#include "sysmel/BootstrapEnvironment/LiteralPositiveInteger.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<LiteralPositiveInteger> literalPositiveIntegerRegistration;

bool LiteralPositiveInteger::isLiteralPositiveInteger() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius