#include "Environment/LiteralPositiveInteger.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<LiteralPositiveInteger> literalPositiveIntegerRegistration;

bool LiteralPositiveInteger::isLiteralPositiveInteger() const
{
    return true;
}

} // End of namespace Environment
} // End of namespace Sysmel