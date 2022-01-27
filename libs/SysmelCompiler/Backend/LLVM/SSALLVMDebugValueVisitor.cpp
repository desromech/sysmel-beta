#include "Backend/LLVM/SSALLVMDebugValueVisitor.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/SSACodeRegion.hpp"
#include "Environment/SSAFunction.hpp"
#include "Environment/SSAGlobalVariable.hpp"
#include "Environment/SSANamespace.hpp"
#include "Environment/SSATypeProgramEntity.hpp"
#include "Environment/SSATemplate.hpp"
#include "Environment/SSATemplateInstance.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"


namespace Sysmel
{
namespace LLVM
{

static BootstrapTypeRegistration<SSADIScope> SSADIScopeTypeRegistration;
static BootstrapTypeRegistration<SSALLVMDebugValueVisitor> SSALLVMDebugValueVisitorTypeRegistration;

AnyValuePtr wrapLLVMDIScope(llvm::DIScope *value)
{
    auto wrapper = basicMakeObject<SSADIScope> ();
    wrapper->value = value;
    return wrapper;
}

llvm::DIScope *SSALLVMDebugValueVisitor::translateDIScope(const SSAValuePtr &value)
{
    return visitValue(value).staticAs<SSADIScope>()->value;
}

AnyValuePtr SSALLVMDebugValueVisitor::visitFunction(const SSAFunctionPtr &function)
{
    auto sourceLocation = function->getDefinitionSourcePosition();
    auto file = backend->getOrCreateDIFileForSourcePosition(sourceLocation);
    auto scope = backend->getOrCreateDIScopeForSSAProgramEntity(function->getParent());
    if(!scope)
        scope = file;
    int scopeLine = sourceLocation->getLine();

    auto flags = llvm::DINode::FlagPrototyped;
    auto spFlags = llvm::DISubprogram::SPFlagZero;
    auto mainCodeRegion = function->getMainCodeRegion();
    if(!mainCodeRegion->isEmpty())
        spFlags = llvm::DISubprogram::SPFlagDefinition;

    auto subprogram = backend->getDIBuilder()->createFunction(
        scope, function->getValidNameString(), llvm::StringRef(),
        file, sourceLocation->getLine(),
        backend->getOrCreateDIFunctionType(function->getFunctionalType()), scopeLine,
        flags, spFlags
    );

    // Finalize the subprogram if the function is empty.
    if(mainCodeRegion->isEmpty())
        backend->getDIBuilder()->finalizeSubprogram(subprogram);
    return wrapLLVMDIScope(subprogram);
}

AnyValuePtr SSALLVMDebugValueVisitor::visitGlobalVariable(const SSAGlobalVariablePtr &value)
{
    return wrapLLVMDIScope(nullptr);
}

AnyValuePtr SSALLVMDebugValueVisitor::visitNamespace(const SSANamespacePtr &value)
{
    auto parentEntity = value->getParent();

    // Is this the global namespace?
    if(validAnyValue(parentEntity)->isUndefined() || !parentEntity->isSSANamespace())
        return wrapLLVMDIScope(nullptr);

    auto parentScope = backend->getOrCreateDIScopeForSSAProgramEntity(parentEntity);
    return wrapLLVMDIScope(backend->getDIBuilder()->createNameSpace(parentScope, value->getValidNameString(), false));
}

AnyValuePtr SSALLVMDebugValueVisitor::visitTypeProgramEntity(const SSATypeProgramEntityPtr &value)
{
    return wrapLLVMDIScope(backend->translateDIType(value->getValue()));
}

AnyValuePtr SSALLVMDebugValueVisitor::visitTemplate(const SSATemplatePtr &value)
{
    return wrapLLVMDIScope(backend->getOrCreateDIScopeForSSAProgramEntity(value->getParent()));
}

AnyValuePtr SSALLVMDebugValueVisitor::visitTemplateInstance(const SSATemplateInstancePtr &value)
{
    return wrapLLVMDIScope(backend->getOrCreateDIScopeForSSAProgramEntity(value->getParent()));
}

} // End of namespace Environment
} // End of namespace Sysmel