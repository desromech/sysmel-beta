#include "sysmel/BootstrapEnvironment/FunctionVariable.hpp"
#include "sysmel/BootstrapEnvironment/ASTArgumentDefinitionNode.hpp"
#include "sysmel/BootstrapEnvironment/CompileTimeCleanUpScope.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<FunctionVariable> FunctionVariableTypeRegistration;

bool FunctionVariable::isFunctionVariable() const
{
    return true;
}

AnyValuePtr FunctionVariable::findStoreBindingInCompileTime(const CompileTimeCleanUpScopePtr &compileTimeCleanUpScope)
{
    return compileTimeCleanUpScope->lookupStoreBindingRecursively(selfFromThis());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius