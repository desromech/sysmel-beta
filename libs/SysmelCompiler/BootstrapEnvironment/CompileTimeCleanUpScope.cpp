#include "sysmel/BootstrapEnvironment/CompileTimeCleanUpScope.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<CompileTimeCleanUpScope> CompileTimeCleanUpScopeRegistration;

CompileTimeCleanUpScopePtr CompileTimeCleanUpScope::makeEmpty()
{
    return basicMakeObject<CompileTimeCleanUpScope> ();
}

CompileTimeCleanUpScopePtr CompileTimeCleanUpScope::makeWithParent(CompileTimeCleanUpScopePtr newParentScope)
{
    auto result = makeEmpty();
    result->parentScope = newParentScope;
    return result;
}

bool CompileTimeCleanUpScope::isCompileTimeCleanUpScope() const
{
    return true;
}

void CompileTimeCleanUpScope::setStoreBinding(const AnyValuePtr &key, const AnyValuePtr &binding)
{
    localStore[key] = binding;
}

AnyValuePtr CompileTimeCleanUpScope::lookupStoreBindingRecursively(const AnyValuePtr &key)
{
    auto it = localStore.find(key);
    if(it != localStore.end())
        return it->second;

    return parentScope ? parentScope->lookupStoreBindingRecursively(key) : nullptr;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius