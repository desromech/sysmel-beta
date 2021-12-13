#ifndef SYSMEL_ENVIRONMENT_SSA_LLVM_MODULE_VISITOR_HPP
#define SYSMEL_ENVIRONMENT_SSA_LLVM_MODULE_VISITOR_HPP
#pragma once

#include "../../Environment/SSAValueVisitor.hpp"

namespace Sysmel
{
namespace LLVM
{

using namespace Environment;

SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSALLVMModuleVisitor)
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSALLVMCodeGenerationBackend)

/**
 * I am the base interface for a SSA based code generation backend
 */
class SSALLVMModuleVisitor : public SubtypeOf<SSAValueVisitor, SSALLVMModuleVisitor>
{
public:
    static constexpr char const __typeName__[] = "SSALLVMModuleVisitor";

    virtual AnyValuePtr visitModule(const SSAModulePtr &value) override;
    virtual AnyValuePtr visitProgramEntityWithChildren(const SSAProgramEntityWithChildrenPtr &value) override;

    virtual AnyValuePtr visitGlobalVariable(const SSAGlobalVariablePtr &value) override;
    virtual AnyValuePtr visitFunction(const SSAFunctionPtr &value) override;

    SSALLVMCodeGenerationBackend *backend = nullptr;
};

} // End of namespace LLVM
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_LLVM_MODULE_VISITOR_HPP