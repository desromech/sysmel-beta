#ifndef SYSMEL_ENVIRONMENT_COMPILE_TIME_CLEAN_UP_SCOPE_HPP
#define SYSMEL_ENVIRONMENT_COMPILE_TIME_CLEAN_UP_SCOPE_HPP
#pragma once

#include "CompilerObject.hpp"
#include <unordered_map>

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(CompileTimeCleanUpScope)

/**
 * I am a clean-up scope. I am used for binding the clean-up for deferred actions,
 * and the clean-up of local variables.
 */
class CompileTimeCleanUpScope : public SubtypeOf<CompilerObject, CompileTimeCleanUpScope>
{
public:
    static constexpr char const __typeName__[] = "CompileTimeCleanUpScope";

    static CompileTimeCleanUpScopePtr makeEmpty();
    static CompileTimeCleanUpScopePtr makeWithParent(CompileTimeCleanUpScopePtr newParentScope);

    virtual bool isCompileTimeCleanUpScope() const override;

    void setStoreBinding(const AnyValuePtr &key, const AnyValuePtr &binding);
    AnyValuePtr lookupStoreBindingRecursively(const AnyValuePtr &key);

    CompileTimeCleanUpScopePtr parentScope;

protected:
    std::unordered_map<AnyValuePtr, AnyValuePtr> localStore;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_COMPILE_TIME_CLEAN_UP_SCOPE_HPP