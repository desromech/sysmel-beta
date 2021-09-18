#include "sysmel/ObjectModel/LiteralUndefined.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace ObjectModel
{
static BootstrapTypeRegistration<LiteralUndefined> literalUndefinedRegistration;

AnyValuePtr getNilConstant()
{
    return LiteralUndefined::uniqueInstance();
}

LiteralUndefinedPtr LiteralUndefined::uniqueInstance()
{
    auto singleton = std::make_shared<LiteralUndefined> ();
    return singleton;
}

bool LiteralUndefined::isLiteralUndefined() const
{
    return true;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius