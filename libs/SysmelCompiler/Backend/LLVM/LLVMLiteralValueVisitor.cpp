#include "Backend/LLVM/LLVMLiteralValueVisitor.hpp"
#include "Backend/LLVM/SSALLVMValueVisitor.hpp"

#include "Environment/SSAConstantLiteralValue.hpp"
#include "Environment/Type.hpp"
#include "Environment/PrimitiveBooleanType.hpp"
#include "Environment/PrimitiveCharacterType.hpp"
#include "Environment/PrimitiveIntegerType.hpp"
#include "Environment/PrimitiveFloatType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace LLVM
{

static BootstrapTypeRegistration<LLVMLiteralValueVisitor> LLVMValueVisitorTypeRegistration;


llvm::Value *LLVMLiteralValueVisitor::translateValueWithExpectedType(const AnyValuePtr &value, const TypePtr &valueExpectedType)
{
    expectedType = valueExpectedType;
    translatedExpectedType = backend->translateType(expectedType);

    if(expectedType->isVoidType())
    {
        return llvm::UndefValue::get(translatedExpectedType);
    }

    return validAnyValue(value)->acceptLiteralValueVisitor(selfFromThis()).staticAs<SSALLVMValue>()->value;
}

llvm::Value *LLVMLiteralValueVisitor::translateConstantLiteralValue(const SSAConstantLiteralValuePtr &constantValue)
{
    return translateValueWithExpectedType(constantValue->getValue(), constantValue->getValueType());
}


AnyValuePtr LLVMLiteralValueVisitor::visitPrimitiveBooleanType(const PrimitiveBooleanTypePtr &value)
{
    auto data = value->unwrapAsBoolean();
    return wrapLLVMValue(llvm::ConstantInt::get(static_cast<llvm::IntegerType*> (translatedExpectedType), data ? 1 : 0, false));
}

AnyValuePtr LLVMLiteralValueVisitor::visitPrimitiveCharacterType(const PrimitiveCharacterTypePtr &value)
{
    auto data = value->unwrapAsChar32();
    return wrapLLVMValue(llvm::ConstantInt::get(static_cast<llvm::IntegerType*> (translatedExpectedType), data, false));
}

AnyValuePtr LLVMLiteralValueVisitor::visitPrimitiveIntegerType(const PrimitiveIntegerTypePtr &value)
{
    auto largeInteger = value->unwrapAsLargeInteger();
    return wrapLLVMValue(llvm::ConstantInt::get(static_cast<llvm::IntegerType*> (translatedExpectedType), largeInteger.asString(), 10));
}

AnyValuePtr LLVMLiteralValueVisitor::visitPrimitiveFloatType(const PrimitiveFloatTypePtr &value)
{
    auto data = value->unwrapAsFloat64();
    return wrapLLVMValue(llvm::ConstantFP::get(translatedExpectedType, data));
}


} // End of namespace Environment
} // End of namespace Sysmel
