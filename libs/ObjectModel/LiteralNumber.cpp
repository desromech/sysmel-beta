#include "sysmel/ObjectModel/LiteralNumber.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace ObjectModel
{
static BootstrapTypeRegistration<LiteralNumber> literalNumberTypeRegistration;

bool LiteralNumber::isLiteralNumber() const
{
    return true;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius