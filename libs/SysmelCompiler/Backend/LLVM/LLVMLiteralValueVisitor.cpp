#include "Backend/LLVM/LLVMLiteralValueVisitor.hpp"
#include "Backend/LLVM/SSALLVMValueVisitor.hpp"

#include "Environment/SSAConstantLiteralValue.hpp"
#include "Environment/Type.hpp"
#include "Environment/ValueBox.hpp"
#include "Environment/PrimitiveBooleanType.hpp"
#include "Environment/PrimitiveCharacterType.hpp"
#include "Environment/PrimitiveIntegerType.hpp"
#include "Environment/PrimitiveFloatType.hpp"
#include "Environment/StructureType.hpp"

#include "Environment/AggregateTypeSequentialLayout.hpp"

#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace LLVM
{

static BootstrapTypeRegistration<LLVMLiteralValueVisitor> LLVMValueVisitorTypeRegistration;


llvm::Constant *LLVMLiteralValueVisitor::translateValueWithExpectedType(const AnyValuePtr &value, const TypePtr &valueExpectedType)
{
    expectedType = valueExpectedType;
    translatedExpectedType = backend->translateType(expectedType);

    if(expectedType->isVoidType())
    {
        return llvm::UndefValue::get(translatedExpectedType);
    }

    return validAnyValue(value)->acceptLiteralValueVisitor(selfFromThis()).staticAs<SSALLVMConstant>()->value;
}

llvm::Constant *LLVMLiteralValueVisitor::translateConstantLiteralValue(const SSAConstantLiteralValuePtr &constantValue)
{
    return translateValueWithExpectedType(constantValue->getValue(), constantValue->getValueType());
}

AnyValuePtr LLVMLiteralValueVisitor::visitValueBox(const ValueBoxPtr &value)
{
    return value->value->acceptLiteralValueVisitor(selfFromThis());
}

AnyValuePtr LLVMLiteralValueVisitor::visitPrimitiveBooleanType(const PrimitiveBooleanTypePtr &value)
{
    auto data = value->unwrapAsBoolean();
    return wrapLLVMConstant(llvm::ConstantInt::get(llvm::cast<llvm::IntegerType> (translatedExpectedType), data ? 1 : 0, false));
}

AnyValuePtr LLVMLiteralValueVisitor::visitPrimitiveCharacterType(const PrimitiveCharacterTypePtr &value)
{
    auto data = value->unwrapAsChar32();
    return wrapLLVMConstant(llvm::ConstantInt::get(llvm::cast<llvm::IntegerType> (translatedExpectedType), data, false));
}

AnyValuePtr LLVMLiteralValueVisitor::visitPrimitiveIntegerType(const PrimitiveIntegerTypePtr &value)
{
    auto largeInteger = value->unwrapAsLargeInteger();
    return wrapLLVMConstant(llvm::ConstantInt::get(llvm::cast<llvm::IntegerType> (translatedExpectedType), largeInteger.asString(), 10));
}

AnyValuePtr LLVMLiteralValueVisitor::visitPrimitiveFloatType(const PrimitiveFloatTypePtr &value)
{
    auto data = value->unwrapAsFloat64();
    return wrapLLVMConstant(llvm::ConstantFP::get(translatedExpectedType, data));
}

AnyValuePtr LLVMLiteralValueVisitor::visitStructureTypeValue(const StructureTypeValuePtr &value)
{
    assert(value->getType());
    auto structureType = value->getType().staticAs<StructureType> ();
    auto resultType = llvm::cast<llvm::StructType> (backend->translateType(value->getType()));
    
    const auto &slotTypes = structureType->getLayout().staticAs<AggregateTypeSequentialLayout> ()->getSlotTypes();
    auto slotCount = value->slots.size();
    assert(slotCount == slotTypes.size());

    std::vector<llvm::Constant*> slots;
    slots.reserve(slotCount);
    for(size_t i = 0; i < slotCount; ++i)
        slots.push_back(backend->translateLiteralValueWithExpectedType(value->slots[i], slotTypes[i]));

    return wrapLLVMConstant(llvm::ConstantStruct::get(resultType, slots));
}

} // End of namespace Environment
} // End of namespace Sysmel