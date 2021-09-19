#include "sysmel/BootstrapEnvironment/LiteralVoid.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

SExpression LiteralVoid::asSExpression() const
{
    return SExpressionVoid{};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius