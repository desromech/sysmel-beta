#ifndef SYSMEL_BACKEND_LLVM_TYPE_VISITOR_HPP
#define SYSMEL_BACKEND_LLVM_TYPE_VISITOR_HPP
#pragma once

#include "../../Environment/TypeVisitor.hpp"
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
class LLVMTypeVisitor : public SubtypeOf<TypeVisitor, LLVMTypeVisitor>
{
public:
    static constexpr char const __typeName__[] = "LLVMTypeVisitor";

    llvm::Type *visitType(const TypePtr &type);

    virtual AnyValuePtr visitMetaType(const MetaTypePtr &type) override;

    virtual AnyValuePtr visitDecoratedType(const DecoratedTypePtr &type) override;
    virtual AnyValuePtr visitEnumType(const EnumTypePtr &type) override;

    virtual AnyValuePtr visitFunctionalType(const FunctionalTypePtr &type) override;
    virtual AnyValuePtr visitPointerLikeType(const PointerLikeTypePtr &type) override;

    AnyValuePtr translateAggregateTypeWithSequentialLayout(const AggregateTypePtr &type, const std::string &prefix=std::string());
    virtual AnyValuePtr visitClassType(const ClassTypePtr &type) override;
    virtual AnyValuePtr visitStructureType(const StructureTypePtr &type) override;
    virtual AnyValuePtr visitTupleType(const TupleTypePtr &type) override;

    SSALLVMCodeGenerationBackend *backend = nullptr;
};

} // End of namespace LLVM
} // End of namespace Sysmel

#endif //SYSMEL_BACKEND_LLVM_TYPE_VISITOR_HPP