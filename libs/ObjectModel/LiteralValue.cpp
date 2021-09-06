#include "sysmel/ObjectModel/LiteralValue.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace ObjectModel
{
static BootstrapTypeRegistration<LiteralValue> literalValueTypeRegistration;

bool LiteralValue::isLiteralValue() const
{
    return true;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius