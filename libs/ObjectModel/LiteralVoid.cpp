#include "sysmel/ObjectModel/LiteralVoid.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace ObjectModel
{
static BootstrapTypeRegistration<LiteralVoid> literalVoidRegistration;

AnyValuePtr getVoidConstant()
{
    return LiteralVoid::uniqueInstance();
}

LiteralVoidPtr LiteralVoid::uniqueInstance()
{
    auto singleton = std::make_shared<LiteralVoid> ();
    return singleton;
}

bool LiteralVoid::isLiteralVoid() const
{
    return true;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius