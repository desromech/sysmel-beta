#include "Environment/FunctionVariable.hpp"
#include "Environment/ASTArgumentDefinitionNode.hpp"
#include "Environment/CompileTimeCleanUpScope.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel