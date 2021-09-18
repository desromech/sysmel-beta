#include "sysmel/ObjectModel/LiteralBooleanTrue.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace ObjectModel
{
static BootstrapTypeRegistration<LiteralBooleanTrue> literalBooleanTrueRegistration;

AnyValuePtr getTrueConstant()
{
    return LiteralBooleanTrue::uniqueInstance();
}

LiteralBooleanTruePtr LiteralBooleanTrue::uniqueInstance()
{
    auto singleton = std::make_shared<LiteralBooleanTrue> ();
    return singleton;
}

bool LiteralBooleanTrue::isLiteralBooleanTrue() const
{
    return true;
}

bool LiteralBooleanTrue::unwrapAsBoolean() const
{
    return true;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius