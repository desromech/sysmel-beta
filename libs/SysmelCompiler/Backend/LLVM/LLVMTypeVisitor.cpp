#include "Backend/LLVM/LLVMTypeVisitor.hpp"

#include "Environment/DecoratedType.hpp"
#include "Environment/EnumType.hpp"
#include "Environment/FunctionalType.hpp"
#include "Environment/PointerLikeType.hpp"

#include "Environment/AggregateTypeSequentialLayout.hpp"
#include "Environment/AggregateTypeVariantLayout.hpp"

#include "Environment/ArrayType.hpp"

#include "Environment/StructureType.hpp"
#include "Environment/ClassType.hpp"
#include "Environment/UnionType.hpp"
#include "Environment/TupleType.hpp"
#include "Environment/VariantType.hpp"

#include "Environment/PrimitiveVectorType.hpp"
#include "Environment/PaddingType.hpp"

#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace LLVM
{

/**
 * I am the base interface for a SSA based code generation backend
 */
class LLVMType : public SubtypeOf<CompilerObject, LLVMType>
{
public:
    static constexpr char const __typeName__[] = "LLVMType";

    llvm::Type *type = nullptr;
};

static BootstrapTypeRegistration<LLVMTypeVisitor> LLVMTypeVisitorTypeRegistration;
static BootstrapTypeRegistration<LLVMType> LLVMTypeTypeRegistration;

static AnyValuePtr wrapLLVMType(llvm::Type *type)
{
    auto wrapper = basicMakeObject<LLVMType> ();
    wrapper->type = type;
    return wrapper;
}

llvm::Type *LLVMTypeVisitor::visitType(const TypePtr &type)
{
    return type->acceptTypeVisitor(selfFromThis()).staticAs<LLVMType> ()->type;
}

AnyValuePtr LLVMTypeVisitor::visitMetaType(const MetaTypePtr &type)
{
    (void)type;
    return wrapLLVMType(llvm::Type::getInt8PtrTy(*backend->getContext()));
}

AnyValuePtr LLVMTypeVisitor::visitDecoratedType(const DecoratedTypePtr &type)
{
    return wrapLLVMType(backend->translateType(type->getBaseType()));
}

AnyValuePtr LLVMTypeVisitor::visitEnumType(const EnumTypePtr &type)
{
    return wrapLLVMType(backend->translateType(type->getBaseType()));
}

llvm::Type *LLVMTypeVisitor::translateArgumentType(const TypePtr &type)
{
    auto result = backend->translateType(type);
    if(type->isPassedByReference())
        return llvm::PointerType::getUnqual(result);
    return result;
}

AnyValuePtr LLVMTypeVisitor::visitFunctionalType(const FunctionalTypePtr &type)
{
    auto declaredResultType = type->getResultType();
    auto resultType = backend->translateType(declaredResultType);
    auto actualResultType = resultType;
    auto receiverType = type->getReceiverType();
    auto hasValidReceiverType = !receiverType->isVoidType();
    auto argumentCount = type->getArgumentCount();
    bool hasCVarArgs = type->hasCVarArgs();
    if(hasCVarArgs)
        --argumentCount;
    bool hasResultReturnedByReference = declaredResultType->isReturnedByReference();
    bool isClosureType = type->isClosureType();
    std::vector<llvm::Type*> argumentType;
    argumentType.reserve(
        (hasResultReturnedByReference ? 1 : 0) +
        (isClosureType ? 1 : 0) +
        (hasValidReceiverType ? 1 : 0) +
        type->getArgumentCount());

    if(hasResultReturnedByReference)
    {
        argumentType.push_back(llvm::PointerType::getUnqual(resultType));
        actualResultType = llvm::Type::getVoidTy(*backend->getContext());
    }

    if(isClosureType)
        argumentType.push_back(llvm::Type::getInt8PtrTy(*backend->getContext()));

    if(hasValidReceiverType)
        argumentType.push_back(translateArgumentType(receiverType));

    for(size_t i = 0; i < argumentCount; ++i)
        argumentType.push_back(translateArgumentType(type->getArgument(i)));

    auto llvmFunctionalType = llvm::FunctionType::get(actualResultType, argumentType, hasCVarArgs);
    llvm::Type *llvmType = llvmFunctionalType;

    // The closure type is actually a pointer type.
    if(isClosureType)
        llvmType = llvm::PointerType::getUnqual(llvm::PointerType::getUnqual(llvmFunctionalType));
    else
        llvmType = llvm::PointerType::getUnqual(llvmFunctionalType);

    return wrapLLVMType(llvmType);
}

AnyValuePtr LLVMTypeVisitor::visitPointerLikeType(const PointerLikeTypePtr &type)
{
    auto baseType = type->getBaseType();
    auto translatedBaseType = backend->translateType(baseType);

    return wrapLLVMType(llvm::PointerType::getUnqual(translatedBaseType));
}

AnyValuePtr LLVMTypeVisitor::visitArrayType(const ArrayTypePtr &type)
{
    auto elementType = backend->translateType(type->elementType);
    return wrapLLVMType(llvm::ArrayType::get(elementType, type->size));
}

AnyValuePtr LLVMTypeVisitor::visitPaddingType(const PaddingTypePtr &type)
{
    return wrapLLVMType(llvm::ArrayType::get(llvm::Type::getInt8Ty(*backend->getContext()), type->size));
}

AnyValuePtr LLVMTypeVisitor::visitPrimitiveVectorType(const PrimitiveVectorTypePtr &type)
{
    auto elementType = backend->translateType(type->elementType);
    return wrapLLVMType(llvm::FixedVectorType::get(elementType, type->elements));
}

AnyValuePtr LLVMTypeVisitor::translateAggregateTypeWithSequentialLayout(const AggregateTypePtr &type, const std::string &prefix)
{
    auto layout = type->getLayout().staticAs<AggregateTypeSequentialLayout> ();
    auto translatedType = llvm::StructType::create(*backend->getContext(), {}, prefix + type->getQualifiedName(), layout->isPacked());
    backend->setTypeTranslation(type, translatedType);

    const auto &slotTypes = layout->getSlotTypes();
    std::vector<llvm::Type*> translatedSlotTypes;
    translatedSlotTypes.reserve(slotTypes.size());
    for(auto &slotType : slotTypes)
        translatedSlotTypes.push_back(sanitizeTypeForAggregate(backend->translateType(slotType)));

    translatedType->setBody(translatedSlotTypes);
    return wrapLLVMType(translatedType);
}

llvm::Type *LLVMTypeVisitor::sanitizeTypeForAggregate(llvm::Type *type)
{
    if(type->isVectorTy())
    {
        auto vectorType = llvm::cast<llvm::FixedVectorType> (type);
        return llvm::ArrayType::get(vectorType->getElementType(), vectorType->getNumElements());
    }

    return type;
}

AnyValuePtr LLVMTypeVisitor::visitClassType(const ClassTypePtr &type)
{
    return translateAggregateTypeWithSequentialLayout(type, "class.");
}

AnyValuePtr LLVMTypeVisitor::visitStructureType(const StructureTypePtr &type)
{
    return translateAggregateTypeWithSequentialLayout(type, "struct.");
}

AnyValuePtr LLVMTypeVisitor::visitUnionType(const UnionTypePtr &type)
{
    llvm::Type *slots = {
        llvm::ArrayType::get(llvm::Type::getInt8Ty(*backend->getContext()), type->getAlignedMemorySize())
    };

    return wrapLLVMType(llvm::StructType::create(*backend->getContext(), slots, "union." + type->getQualifiedName()));
}

AnyValuePtr LLVMTypeVisitor::visitTupleType(const TupleTypePtr &type)
{
    return translateAggregateTypeWithSequentialLayout(type, "tuple.");
}

AnyValuePtr LLVMTypeVisitor::visitVariantType(const VariantTypePtr &type)
{
    auto layout = type->getLayout().staticAs<AggregateTypeVariantLayout> ();
    std::vector<llvm::Type*> slots;
    slots.reserve(4);

    slots.push_back(backend->translateType(layout->getDataTypeIndexType()));
    auto int8Ty = llvm::Type::getInt8Ty(*backend->getContext());

    auto bytes = layout->getPaddingSize();
    if(bytes)
        slots.push_back(llvm::ArrayType::get(int8Ty, bytes));

    bytes = layout->getElementMemorySize();
    if(bytes)
        slots.push_back(llvm::ArrayType::get(int8Ty, bytes));

    bytes = layout->getEndPaddingSize();
    if(bytes)
        slots.push_back(llvm::ArrayType::get(int8Ty, bytes));

    return wrapLLVMType(llvm::StructType::create(*backend->getContext(), slots, "variant." + type->getQualifiedName()));
}

} // End of namespace Environment
} // End of namespace Sysmel
