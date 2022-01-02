#ifndef SYSMEL_BACKEND_LLVM_DEBUG_TYPE_VISITOR_HPP
#define SYSMEL_BACKEND_LLVM_DEBUG_TYPE_VISITOR_HPP
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
class SYSMEL_COMPILER_LIB_EXPORT LLVMDebugTypeVisitor : public SubtypeOf<TypeVisitor, LLVMDebugTypeVisitor>
{
public:
    static constexpr char const __typeName__[] = "LLVMDebugTypeVisitor";

    llvm::DIType *visitType(const TypePtr &type);

    virtual AnyValuePtr visitMetaType(const MetaTypePtr &type) override;
    virtual AnyValuePtr visitDecoratedType(const DecoratedTypePtr &type) override;

    virtual AnyValuePtr visitPointerType(const PointerTypePtr &type) override;
    virtual AnyValuePtr visitReferenceType(const ReferenceTypePtr &type) override;
    virtual AnyValuePtr visitTemporaryReferenceType(const TemporaryReferenceTypePtr &type) override;

    virtual AnyValuePtr visitFunctionalType(const FunctionalTypePtr &type) override;

    virtual AnyValuePtr visitArrayType(const ArrayTypePtr &type) override;
    virtual AnyValuePtr visitPrimitiveVectorType(const PrimitiveVectorTypePtr &type) override;

    llvm::DIType *translateAggregateTypeWithFields(const AggregateTypeWithFieldsPtr &type, unsigned int tag);
    virtual AnyValuePtr visitStructureType(const StructureTypePtr &type) override;
    virtual AnyValuePtr visitClassType(const ClassTypePtr &type) override;
    virtual AnyValuePtr visitUnionType(const UnionTypePtr &type) override;

    virtual AnyValuePtr visitTupleType(const TupleTypePtr &type) override;
    virtual AnyValuePtr visitVariantType(const VariantTypePtr &type) override;

    llvm::DIType *translateFieldOf(const FieldVariablePtr &field, llvm::DIScope *parent);
    SSALLVMCodeGenerationBackend *backend = nullptr;
};

} // End of namespace LLVM
} // End of namespace Sysmel

#endif //SYSMEL_BACKEND_LLVM_TYPE_VISITOR_HPP