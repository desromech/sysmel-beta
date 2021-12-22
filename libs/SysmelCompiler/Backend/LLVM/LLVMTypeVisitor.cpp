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

AnyValuePtr LLVMTypeVisitor::visitFunctionalType(const FunctionalTypePtr &type)
{
    auto resultType = backend->translateType(type->getResultType());
    auto receiverType = type->getReceiverType();
    auto hasValidReceiverType = !receiverType->isVoidType();
    std::vector<llvm::Type*> argumentType;
    argumentType.reserve((hasValidReceiverType ? 1 : 0) + type->getArgumentCount());
    if(hasValidReceiverType)
        argumentType.push_back(backend->translateType(receiverType));

    for(size_t i = 0; i < type->getArgumentCount(); ++i)
        argumentType.push_back(backend->translateType(type->getArgument(i)));

    return wrapLLVMType(llvm::FunctionType::get(resultType, argumentType, false));
}

AnyValuePtr LLVMTypeVisitor::visitPointerLikeType(const PointerLikeTypePtr &type)
{
    auto baseType = type->getBaseType();
    auto translatedBaseType = backend->translateType(baseType);

    return wrapLLVMType(llvm::PointerType::getUnqual(translatedBaseType));
}

AnyValuePtr LLVMTypeVisitor::translateAggregateTypeWithSequentialLayout(const AggregateTypePtr &type)
{
    auto translatedType = llvm::StructType::create(*backend->getContext(), validAnyValue(type->getName())->asString());
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
    return translateAggregateTypeWithSequentialLayout(type);
}

AnyValuePtr LLVMTypeVisitor::visitStructureType(const StructureTypePtr &type)
{
    return translateAggregateTypeWithSequentialLayout(type);
}

AnyValuePtr LLVMTypeVisitor::visitTupleType(const TupleTypePtr &type)
{
    return translateAggregateTypeWithSequentialLayout(type);
}

} // End of namespace Environment
} // End of namespace Sysmel
