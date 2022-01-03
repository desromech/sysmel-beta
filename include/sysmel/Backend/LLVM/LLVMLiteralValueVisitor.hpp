#ifndef SYSMEL_BACKEND_LLVM_LITERAL_VALUE_VISITOR_HPP
#define SYSMEL_BACKEND_LLVM_LITERAL_VALUE_VISITOR_HPP
#pragma once

#include "../../Environment/LiteralValueVisitor.hpp"
#include "../../Environment/SSAConstantLiteralValue.hpp"
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
class SYSMEL_COMPILER_LIB_EXPORT LLVMLiteralValueVisitor : public SubtypeOf<LiteralValueVisitor, LLVMLiteralValueVisitor>
{
public:
    static constexpr char const __typeName__[] = "LLVMLiteralValueVisitor";

    llvm::Constant *translateValueWithExpectedType(const AnyValuePtr &value, const TypePtr &valueExpectedType);
    llvm::Constant *translateConstantLiteralValue(const SSAConstantLiteralValuePtr &constantValue);

    virtual AnyValuePtr visitValueBox(const ValueBoxPtr &value) override;
    virtual AnyValuePtr visitUndefined(const UndefinedPtr &value) override;
    virtual AnyValuePtr visitVoid(const VoidPtr &value) override;
    virtual AnyValuePtr visitVirtualTable(const VirtualTablePtr &value) override;

    virtual AnyValuePtr visitLiteralInteger(const LiteralIntegerPtr &value) override;
    
    virtual AnyValuePtr visitPrimitiveBooleanType(const PrimitiveBooleanTypePtr &value) override;
    virtual AnyValuePtr visitPrimitiveCharacterType(const PrimitiveCharacterTypePtr &value) override;
    virtual AnyValuePtr visitPrimitiveIntegerType(const PrimitiveIntegerTypePtr &value) override;
    virtual AnyValuePtr visitPrimitiveFloatType(const PrimitiveFloatTypePtr &value) override;
    virtual AnyValuePtr visitPrimitiveVectorTypeValue(const PrimitiveVectorTypeValuePtr &value) override;

    virtual AnyValuePtr visitPointerLikeTypeValue(const PointerLikeTypeValuePtr &value) override;

    llvm::Constant *translateSequentialStructAggregateTypeValue(const AggregateTypeValuePtr &value);
    virtual AnyValuePtr visitTupleTypeValue(const TupleTypeValuePtr &value) override;
    virtual AnyValuePtr visitClassTypeValue(const ClassTypeValuePtr &value) override;
    virtual AnyValuePtr visitStructureTypeValue(const StructureTypeValuePtr &value) override;

    SSALLVMCodeGenerationBackend *backend = nullptr;
    llvm::Type *translatedExpectedType = nullptr;
    TypePtr expectedType;
};

} // End of namespace LLVM
} // End of namespace Sysmel

#endif //SYSMEL_BACKEND_LLVM_LITERAL_VALUE_VISITOR_HPP