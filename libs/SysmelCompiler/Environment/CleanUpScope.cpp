#include "Environment/CleanUpScope.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<CleanUpScope> CleanUpScopeRegistration;

CleanUpScopePtr CleanUpScope::makeEmpty()
{
    return basicMakeObject<CleanUpScope> ();
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

} // End of namespace Environment
} // End of namespace Sysmel