#include "Backend/LLVM/SSALLVMModuleVisitor.hpp"
#include "Backend/LLVM/SSALLVMCodeGenerationBackend.hpp"

#include "Environment/SSAModule.hpp"
#include "Environment/SSAProgramEntityWithChildren.hpp"
#include "Environment/SSANamespace.hpp"
#include "Environment/SSAGlobalVariable.hpp"
#include "Environment/SSAFunction.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace LLVM
{

static BootstrapTypeRegistration<SSALLVMModuleVisitor> SSALLVMModuleVisitorTypeRegistration;

AnyValuePtr SSALLVMModuleVisitor::visitModule(const SSAModulePtr &value)
{
    visitValue(value->getGlobalNamespace());
    return nullptr;
}

AnyValuePtr SSALLVMModuleVisitor::visitProgramEntityWithChildren(const SSAProgramEntityWithChildrenPtr &value)
{
    for(auto &child : value->getChildren())
        visitValue(child);
    return nullptr;
}

AnyValuePtr SSALLVMModuleVisitor::visitGlobalVariable(const SSAGlobalVariablePtr &value)
{
    backend->translateGlobalValue(value);
    return nullptr;
}

AnyValuePtr SSALLVMModuleVisitor::visitFunction(const SSAFunctionPtr &value)
{
    backend->translateGlobalValue(value);
    return nullptr;
}

} // End of namespace Environment
} // End of namespace Sysmel