#include "Backend/LLVM/LLVMLiteralValueVisitor.hpp"
#include "Backend/LLVM/SSALLVMValueVisitor.hpp"

#include "Environment/SSAConstantLiteralValue.hpp"
#include "Environment/Type.hpp"
#include "Environment/ValueBox.hpp"
#include "Environment/PointerLikeType.hpp"
#include "Environment/PrimitiveBooleanType.hpp"
#include "Environment/PrimitiveCharacterType.hpp"
#include "Environment/PrimitiveIntegerType.hpp"
#include "Environment/PrimitiveFloatType.hpp"
#include "Environment/ClassType.hpp"
#include "Environment/StructureType.hpp"
#include "Environment/TupleType.hpp"

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

AnyValuePtr LLVMLiteralValueVisitor::visitPointerLikeTypeValue(const PointerLikeTypeValuePtr &value)
{
    sysmelAssert(translatedExpectedType->isPointerTy());
    auto resultType = llvm::cast<llvm::PointerType> (translatedExpectedType);
    auto baseValue = value->baseValue;
    if(!baseValue || baseValue->isUndefined())
    {
        return wrapLLVMConstant(llvm::ConstantPointerNull::get(resultType));
    }
    else if(baseValue->isLiteralString())
    {
        auto elementType = value->getType().staticAs<PointerLikeType> ()->getBaseType()->asUndecoratedType();
        auto result = backend->internStringConstantPointer(elementType, baseValue->asString(), true);
        sysmelAssert(result->getType()->isPointerTy());

        return wrapLLVMConstant(llvm::ConstantExpr::getBitCast(result, translatedExpectedType));
    }
    else if(baseValue->isLiteralInteger())
    {
        sysmelAssert("TODO: translate integer address into constant pointer" && false);
    }
    else if(baseValue->isProgramEntity())
    {
        sysmelAssert("TODO: translate program entity address into constant pointer" && false);
    }

    sysmelAssert("unsupported pointer type value constant" && false);
}

llvm::Constant *LLVMLiteralValueVisitor::translateSequentialStructAggregateTypeValue(const AggregateTypeValuePtr &value)
{
    sysmelAssert(value->getType());
    auto aggregateType = value->getType().staticAs<AggregateType> ();
    auto resultType = llvm::cast<llvm::StructType> (backend->translateType(value->getType()));
    
    const auto &slotTypes = aggregateType->getLayout().staticAs<AggregateTypeSequentialLayout> ()->getSlotTypes();
    auto slotCount = value->slots.size();
    sysmelAssert(slotCount == slotTypes.size());

    std::vector<llvm::Constant*> slots;
    slots.reserve(slotCount);
    for(size_t i = 0; i < slotCount; ++i)
        slots.push_back(backend->translateLiteralValueWithExpectedType(value->slots[i], slotTypes[i]));

    return llvm::ConstantStruct::get(resultType, slots);
}

AnyValuePtr LLVMLiteralValueVisitor::visitTupleTypeValue(const TupleTypeValuePtr &value)
{
    return wrapLLVMConstant(translateSequentialStructAggregateTypeValue(value));
}

AnyValuePtr LLVMLiteralValueVisitor::visitClassTypeValue(const ClassTypeValuePtr &value)
{
    return wrapLLVMConstant(translateSequentialStructAggregateTypeValue(value));
}

AnyValuePtr LLVMLiteralValueVisitor::visitStructureTypeValue(const StructureTypeValuePtr &value)
{
    return wrapLLVMConstant(translateSequentialStructAggregateTypeValue(value));
}

} // End of namespace Environment
} // End of namespace Sysmel
