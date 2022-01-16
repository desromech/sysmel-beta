#ifndef SYSMEL_ENVIRONMENT_SSA_LLVM_DEBUG_VALUE_VISITOR_HPP
#define SYSMEL_ENVIRONMENT_SSA_LLVM_DEBUG_VALUE_VISITOR_HPP
#pragma once

#include "../../Environment/SSAValueVisitor.hpp"
#include "SSALLVMCodeGenerationBackend.hpp"

namespace Sysmel
{
namespace LLVM
{

using namespace Environment;

SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSALLVMDebugValueVisitor)

/**
 * I wrap a llvm value in an AnyValue
 */
class SYSMEL_COMPILER_LIB_EXPORT SSADIScope : public SubtypeOf<CompilerObject, SSADIScope>
{
public:
    static constexpr char const __typeName__[] = "SSADIScope";

    llvm::DIScope *value = nullptr;
};

SYSMEL_COMPILER_LIB_EXPORT AnyValuePtr wrapLLVMDIScope(llvm::DIScope *value);

/**
 * I visit SSA values for converting them into llvm SSA values.
 */
class SYSMEL_COMPILER_LIB_EXPORT SSALLVMDebugValueVisitor : public SubtypeOf<SSAValueVisitor, SSALLVMDebugValueVisitor>
{
public:
    static constexpr char const __typeName__[] = "SSALLVMDebugValueVisitor";

    llvm::DIScope *translateDIScope(const SSAValuePtr &value);
    virtual AnyValuePtr visitFunction(const SSAFunctionPtr &value) override;
    virtual AnyValuePtr visitGlobalVariable(const SSAGlobalVariablePtr &value) override;
    virtual AnyValuePtr visitNamespace(const SSANamespacePtr &value) override;
    virtual AnyValuePtr visitTypeProgramEntity(const SSATypeProgramEntityPtr &value) override;
    virtual AnyValuePtr visitTemplate(const SSATemplatePtr &value) override;
    virtual AnyValuePtr visitTemplateInstance(const SSATemplateInstancePtr &value) override;

    SSALLVMCodeGenerationBackend *backend = nullptr;
};

} // End of namespace LLVM
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_LLVM_VALUE_VISITOR_HPP