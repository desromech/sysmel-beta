#include "Environment/LiteralNegativeInteger.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<LiteralNegativeInteger> literalNegativeIntegerTypeRegistration;

bool LiteralNegativeInteger::isLiteralNegativeInteger() const
{
    return true;
}

} // End of namespace Environment
} // End of namespace Sysmel