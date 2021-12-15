#ifndef SYSMEL_BACKEND_LLVM_LITERAL_VALUE_VISITOR_HPP
#define SYSMEL_BACKEND_LLVM_LITERAL_VALUE_VISITOR_HPP
#pragma once

#include "../../Environment/LiteralValueVisitor.hpp"
#include "SSALLVMCodeGenerationBackend.hpp"

namespace Sysmel
{
namespace LLVM
{

using namespace Environment;

SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSALLVMValueVisitor)

/**
 * I am the base interface for a SSA based code generation backend
 */
class LLVMLiteralValueVisitor : public SubtypeOf<LiteralValueVisitor, LLVMLiteralValueVisitor>
{
public:
    static constexpr char const __typeName__[] = "LLVMLiteralValueVisitor";

    llvm::Value *visitValue(const AnyValuePtr &value);

    SSALLVMCodeGenerationBackend *backend = nullptr;
};

} // End of namespace LLVM
} // End of namespace Sysmel

#endif //SYSMEL_BACKEND_LLVM_LITERAL_VALUE_VISITOR_HPP