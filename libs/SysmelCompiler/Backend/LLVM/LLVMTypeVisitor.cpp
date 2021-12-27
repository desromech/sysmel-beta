#include "Backend/LLVM/LLVMTypeVisitor.hpp"

#include "Environment/DecoratedType.hpp"
#include "Environment/EnumType.hpp"
#include "Environment/FunctionalType.hpp"
#include "Environment/PointerLikeType.hpp"

#include "Environment/AggregateTypeSequentialLayout.hpp"

#include "Environment/StructureType.hpp"
#include "Environment/ClassType.hpp"
#include "Environment/UnionType.hpp"
#include "Environment/TupleType.hpp"

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
    std::vector<llvm::Type*> argumentType;
    argumentType.reserve((hasResultReturnedByReference ? 1 : 0) +(hasValidReceiverType ? 1 : 0) + type->getArgumentCount());

    if(hasResultReturnedByReference)
    {
        argumentType.push_back(llvm::PointerType::getUnqual(resultType));
        actualResultType = llvm::Type::getVoidTy(*backend->getContext());
    }
    
    if(hasValidReceiverType)
        argumentType.push_back(translateArgumentType(receiverType));

    for(size_t i = 0; i < argumentCount; ++i)
        argumentType.push_back(translateArgumentType(type->getArgument(i)));

    return wrapLLVMType(llvm::FunctionType::get(actualResultType, argumentType, hasCVarArgs));
}

AnyValuePtr LLVMTypeVisitor::visitPointerLikeType(const PointerLikeTypePtr &type)
{
    auto baseType = type->getBaseType();
    auto translatedBaseType = backend->translateType(baseType);

    return wrapLLVMType(llvm::PointerType::getUnqual(translatedBaseType));
}

AnyValuePtr LLVMTypeVisitor::translateAggregateTypeWithSequentialLayout(const AggregateTypePtr &type, const std::string &prefix)
{
    auto translatedType = llvm::StructType::create(*backend->getContext(), prefix + type->getQualifiedName());
    backend->setTypeTranslation(type, translatedType);

    auto layout = type->getLayout().staticAs<AggregateTypeSequentialLayout> ();
    const auto &slotTypes = layout->getSlotTypes();
    std::vector<llvm::Type*> translatedSlotTypes;
    translatedSlotTypes.reserve(slotTypes.size());
    for(auto &slotType : slotTypes)
        translatedSlotTypes.push_back(backend->translateType(slotType));

    translatedType->setBody(translatedSlotTypes);
    return wrapLLVMType(translatedType);
}

AnyValuePtr LLVMTypeVisitor::visitClassType(const ClassTypePtr &type)
{
    return translateAggregateTypeWithSequentialLayout(type, "class.");
}

AnyValuePtr LLVMTypeVisitor::visitStructureType(const StructureTypePtr &type)
{
    return translateAggregateTypeWithSequentialLayout(type, "struct.");
}

AnyValuePtr LLVMTypeVisitor::visitTupleType(const TupleTypePtr &type)
{
    return translateAggregateTypeWithSequentialLayout(type, "union.");
}

} // End of namespace Environment
} // End of namespace Sysmel
