#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CLEAN_UP_SCOPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CLEAN_UP_SCOPE_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(CleanUpScope)

/**
 * I am a clean-up scope. I am used for binding the clean-up for deferred actions,
 * and the clean-up of local variables.
 */
class CleanUpScope : public SubtypeOf<CompilerObject, CleanUpScope>
{
public:
    static constexpr char const __typeName__[] = "CleanUpScope";

    virtual bool isCleanUpScope() const override;

    static CleanUpScopePtr makeEmpty();
    static CleanUpScopePtr makeWithParent(CleanUpScopePtr newParentScope);

    CleanUpScopePtr parentScope;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CLEAN_UP_SCOPE_HPP