#include "sysmel/BootstrapEnvironment/CleanUpScope.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<CleanUpScope> CleanUpScopeRegistration;

CleanUpScopePtr CleanUpScope::makeEmpty()
{
    return std::make_shared<CleanUpScope> ();
}

CleanUpScopePtr CleanUpScope::makeWithParent(CleanUpScopePtr newParentScope)
{
    auto result = makeEmpty();
    result->parentScope = newParentScope;
    return result;
}

bool CleanUpScope::isCleanUpScope() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius