#include "sysmel/BootstrapEnvironment/LiteralNumber.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/SubclassResponsibility.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<LiteralNumber> literalNumberTypeRegistration;

MethodCategories LiteralNumber::__instanceMethods__()
{
    return MethodCategories{
        {"converting", {
            makeMethodBinding("asFloat", &LiteralNumber::asFloat),
        }}
    };
}

bool LiteralNumber::isLiteralNumber() const
{
    return true;
}

double LiteralNumber::asFloat() const
{
    SysmelSelfSubclassResponsibility();
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius