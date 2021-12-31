#include "Backend/LLVM/SSALLVMValueVisitor.hpp"
#include "Backend/LLVM/LLVMLiteralValueVisitor.hpp"

#include "Environment/Type.hpp"
#include "Environment/FunctionalType.hpp"
#include "Environment/FieldVariable.hpp"
#include "Environment/NameMangler.hpp"
#include "Environment/ASTSourcePosition.hpp"

#include "Environment/ArgumentVariable.hpp"

#include "Environment/SSAConstantLiteralValue.hpp"
#include "Environment/SSAFunction.hpp"
#include "Environment/SSACodeRegion.hpp"
#include "Environment/SSACodeRegionArgument.hpp"
#include "Environment/SSACodeRegionCapture.hpp"
#include "Environment/SSABasicBlock.hpp"
#include "Environment/SSAGlobalVariable.hpp"

#include "Environment/SSACallInstruction.hpp"
#include "Environment/SSAConditionalJumpInstruction.hpp"
#include "Environment/SSADeclareLocalVariableInstruction.hpp"
#include "Environment/SSAIfInstruction.hpp"
#include "Environment/SSAGetAggregateFieldReferenceInstruction.hpp"
#include "Environment/SSAGetAggregateSlotReferenceInstruction.hpp"
#include "Environment/SSAJumpInstruction.hpp"
#include "Environment/SSALoadInstruction.hpp"
#include "Environment/SSALocalVariableInstruction.hpp"
#include "Environment/SSADoWithCleanupInstruction.hpp"
#include "Environment/SSADoWhileInstruction.hpp"
#include "Environment/SSAMakeClosureInstruction.hpp"
#include "Environment/SSAReturnFromFunctionInstruction.hpp"
#include "Environment/SSAReturnFromRegionInstruction.hpp"
#include "Environment/SSAStoreInstruction.hpp"
#include "Environment/SSAUnreachableInstruction.hpp"
#include "Environment/SSAWhileInstruction.hpp"

#include "Environment/SSAUpcastInstruction.hpp"
#include "Environment/SSADowncastInstruction.hpp"
#include "Environment/SSABitcastInstruction.hpp"

#include "Environment/SSAEnableLocalFinalization.hpp"
#include "Environment/SSALocalFinalization.hpp"

#include "Environment/BootstrapTypeRegistration.hpp"

#include "Environment/ControlFlowUtilities.hpp"

namespace Sysmel
{
namespace LLVM
{

static BootstrapTypeRegistration<SSALLVMValueVisitor> SSALLVMValueVisitorTypeRegistration;
static BootstrapTypeRegistration<SSALLVMValue> SSALLVMValueTypeRegistration;
static BootstrapTypeRegistration<SSALLVMConstant> SSALLVMConstantTypeRegistration;

std::unordered_map<std::string, std::function<llvm::Value* (const IntrinsicGenerationContext &)>> SSALLVMValueVisitor::intrinsicGenerators = {
    // Boolean
    {"boolean.not", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 1);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateNot(
            context.self->coerceBooleanIntoI1(context.arguments.back())
            ));
    }},
    {"boolean.xor", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(
                context.builder->CreateXor(
                    context.self->coerceBooleanIntoI1(context.arguments[0]),
                    context.self->coerceBooleanIntoI1(context.arguments[1]))
        );
    }},

    {"boolean.equals", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(
            context.builder->CreateICmpEQ(
                context.self->coerceBooleanIntoI1(context.arguments[0]),
                context.self->coerceBooleanIntoI1(context.arguments[1])
            ));
    }},

    {"boolean.not-equals", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(
            context.builder->CreateICmpNE(
                context.self->coerceBooleanIntoI1(context.arguments[0]),
                context.self->coerceBooleanIntoI1(context.arguments[1])
            ));
    }},

    {"boolean.less-than", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(
            context.builder->CreateICmpULT(
                context.self->coerceBooleanIntoI1(context.arguments[0]),
                context.self->coerceBooleanIntoI1(context.arguments[1])
            ));
    }},

    {"boolean.less-equals", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(
            context.builder->CreateICmpULE(
                context.self->coerceBooleanIntoI1(context.arguments[0]),
                context.self->coerceBooleanIntoI1(context.arguments[1])
            ));
    }},

    {"boolean.greater-than", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(
            context.builder->CreateICmpUGT(
                context.self->coerceBooleanIntoI1(context.arguments[0]),
                context.self->coerceBooleanIntoI1(context.arguments[1])
            ));
    }},

    {"boolean.greater-equals", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(
            context.builder->CreateICmpUGE(
                context.self->coerceBooleanIntoI1(context.arguments[0]),
                context.self->coerceBooleanIntoI1(context.arguments[1])
            ));
    }},

    // Integer
    {"integer.conversion.sign-extend", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() <= 2);
        return context.builder->CreateSExt(context.arguments.back(), context.resultType);
    }},
    {"integer.conversion.zero-extend", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() <= 2);
        return context.builder->CreateZExt(context.arguments.back(), context.resultType);
    }},
    {"integer.conversion.bitcast", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() <= 2);
        return context.builder->CreateBitCast(context.arguments.back(), context.resultType);
    }},
    {"integer.conversion.truncate", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() <= 2);
        return context.builder->CreateTrunc(context.arguments.back(), context.resultType);
    }},

    {"integer.equals", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateICmpEQ(context.arguments[0], context.arguments[1]));
    }},

    {"integer.not-equals", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateICmpNE(context.arguments[0], context.arguments[1]));
    }},

    {"integer.less-than.signed", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateICmpSLT(context.arguments[0], context.arguments[1]));
    }},

    {"integer.less-than.unsigned", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateICmpULT(context.arguments[0], context.arguments[1]));
    }},

    {"integer.less-equals.signed", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateICmpSLE(context.arguments[0], context.arguments[1]));
    }},

    {"integer.less-equals.unsigned", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateICmpULE(context.arguments[0], context.arguments[1]));
    }},

    {"integer.greater-than.signed", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateICmpSGT(context.arguments[0], context.arguments[1]));
    }},

    {"integer.greater-than.unsigned", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateICmpUGT(context.arguments[0], context.arguments[1]));
    }},

    {"integer.greater-equals.signed", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateICmpSGE(context.arguments[0], context.arguments[1]));
    }},

    {"integer.greater-equals.unsigned", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateICmpUGE(context.arguments[0], context.arguments[1]));
    }},

    {"integer.neg", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 1);
        return context.builder->CreateNeg(context.arguments[0]);
    }},

    {"integer.add", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateAdd(context.arguments[0], context.arguments[1]);
    }},
    {"integer.sub", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateSub(context.arguments[0], context.arguments[1]);
    }},
    {"integer.mul.signed", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateMul(context.arguments[0], context.arguments[1]);
    }},
    {"integer.mul.unsigned", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateMul(context.arguments[0], context.arguments[1]);
    }},
    {"integer.div.signed", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateSDiv(context.arguments[0], context.arguments[1]);
    }},
    {"integer.div.unsigned", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateUDiv(context.arguments[0], context.arguments[1]);
    }},
    {"integer.mod.signed", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateSRem(context.arguments[0], context.arguments[1]);
    }},
    {"integer.mod.unsigned", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateURem(context.arguments[0], context.arguments[1]);
    }},

    {"integer.bitnot", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 1);
        return context.builder->CreateNot(context.arguments[0]);
    }},
    {"integer.bitor", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateOr(context.arguments[0], context.arguments[1]);
    }},
    {"integer.bitand", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateAnd(context.arguments[0], context.arguments[1]);
    }},
    {"integer.bitxor", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateXor(context.arguments[0], context.arguments[1]);
    }},
    {"integer.shift-left", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateShl(context.arguments[0], context.arguments[1]);
    }},
    {"integer.shift-right.unsigned", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateLShr(context.arguments[0], context.arguments[1]);
    }},
    {"integer.shift-right.signed", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateAShr(context.arguments[0], context.arguments[1]);
    }},

    // Floats
    {"float.equals", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateFCmpUEQ(context.arguments[0], context.arguments[1]));
    }},

    {"float.not-equals", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateFCmpUNE(context.arguments[0], context.arguments[1]));
    }},

    {"float.less-than", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateFCmpULT(context.arguments[0], context.arguments[1]));
    }},

    {"float.less-equals", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateFCmpULE(context.arguments[0], context.arguments[1]));
    }},

    {"float.greater-than", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateFCmpUGT(context.arguments[0], context.arguments[1]));
    }},

    {"float.greater-equals", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateFCmpUGE(context.arguments[0], context.arguments[1]));
    }},

    {"float.neg", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 1);
        return context.builder->CreateFNeg(context.arguments[0]);
    }},

    {"float.add", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateFAdd(context.arguments[0], context.arguments[1]);
    }},
    {"float.sub", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateFSub(context.arguments[0], context.arguments[1]);
    }},
    {"float.mul", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateFMul(context.arguments[0], context.arguments[1]);
    }},
    {"float.div", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateFDiv(context.arguments[0], context.arguments[1]);
    }},
    {"float.idiv", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        auto rem = context.builder->CreateFRem(context.arguments[0], context.arguments[1]);
        auto dividend = context.builder->CreateFSub(context.arguments[0], rem);
        return context.builder->CreateFDiv(dividend, context.arguments[1]);
    }},
    {"float.mod", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateFRem(context.arguments[0], context.arguments[1]);
    }},

    // Array
    {"array.element", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateGEP(context.arguments[0], {
            llvm::ConstantInt::get(context.self->backend->translateType(Type::getIntPointerType()), 0),
            context.arguments[1]
        });
    }},

    // Pointer
    {"pointer.equals", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateICmpEQ(context.arguments[0], context.arguments[1]));
    }},

    {"pointer.not-equals", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateICmpNE(context.arguments[0], context.arguments[1]));
    }},

    {"pointer.less-than", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateICmpULT(context.arguments[0], context.arguments[1]));
    }},

    {"pointer.less-equals", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateICmpULE(context.arguments[0], context.arguments[1]));
    }},

    {"pointer.greater-than", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateICmpUGT(context.arguments[0], context.arguments[1]));
    }},

    {"pointer.greater-equals", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateICmpUGE(context.arguments[0], context.arguments[1]));
    }},

    {"pointer.to.reference", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 1);
        return context.arguments[0];
    }},

    {"pointer.element", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateGEP(context.arguments[0], context.arguments[1]);
    }},

    // Reference
    {"reference.load", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 1);
        return context.builder->CreateLoad(context.arguments[0]);
    }},
    {"reference.to.pointer", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 1);
        return context.arguments[0];
    }},
    {"reference.reinterpret", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() <= 2);
        return context.arguments.back();
    }},
    {"reference.copy.assignment.trivial", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateStore(context.arguments[1], context.arguments[0]);
    }},

    // CVarArg
    {"cvar.arg.identity", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() <= 2);
        return context.arguments.back();
    }},
    {"cvar.arg.zeroext32", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() <= 2);
        return context.builder->CreateZExt(context.arguments.back(), context.builder->getInt32Ty());
    }},
    {"cvar.arg.signext32", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() <= 2);
        return context.builder->CreateSExt(context.arguments.back(), context.builder->getInt32Ty());
    }},
    {"cvar.arg.float64", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() <= 2);
        return context.builder->CreateFPExt(context.arguments.back(), context.builder->getDoubleTy());
    }},
};

static llvm::GlobalValue::DLLStorageClassTypes convertDLLStorageClass(DllLinkageMode mode)
{
    switch(mode)
    {
    case DllLinkageMode::Import: return llvm::GlobalValue::DLLImportStorageClass;
    case DllLinkageMode::Export: return llvm::GlobalValue::DLLExportStorageClass;
    default: return llvm::GlobalValue::DefaultStorageClass;
    }
}

AnyValuePtr wrapLLVMValue(llvm::Value *value)
{
    auto wrapper = basicMakeObject<SSALLVMValue> ();
    wrapper->value = value;
    return wrapper;
}

AnyValuePtr wrapLLVMConstant(llvm::Constant *value)
{
    auto wrapper = basicMakeObject<SSALLVMConstant> ();
    wrapper->value = value;
    return wrapper;
}

llvm::Value *SSALLVMValueVisitor::visitValueForLLVM(const SSAValuePtr &value)
{
    return visitValue(value).staticAs<SSALLVMValue>()->value;
}

llvm::Value *SSALLVMValueVisitor::translateValue(const SSAValuePtr &value)
{
    auto it = localTranslatedValueMap.find(value);
    if(it != localTranslatedValueMap.end())
        return it->second;
    
    sysmelAssert(!value->isSSAInstruction());
    auto result = visitValueForLLVM(value);
    if(value->isSSACodeRegionLocalValue())
        localTranslatedValueMap.insert({value, result});
    return result;
}

AnyValuePtr SSALLVMValueVisitor::visitFunction(const SSAFunctionPtr &function)
{
    if(currentFunction)
        return wrapLLVMValue(backend->translateGlobalValue(function));

    // Guard for recursive definitions.
    {
        auto existent = backend->findGlobalValueTranslation(function);
        if(existent)
            return wrapLLVMValue(existent);
    }

    auto mainCodeRegion = function->getMainCodeRegion();
    auto resultType = backend->translateType(mainCodeRegion->getResultType());
    auto argumentCount = mainCodeRegion->getArgumentCount();
    auto hasCVarArgs = mainCodeRegion->hasCVarArgs();
    auto isClosure = function->getFunctionalType()->isClosureType();
    if(hasCVarArgs)
        --argumentCount;
    std::vector<llvm::Type*> argumentTypes;
    argumentTypes.reserve(argumentCount);

    // Add the result argument first.
    size_t firstArgumentIndex = 0;
    if(mainCodeRegion->isReturningByReference())
    {
        argumentTypes.push_back(backend->translateType(mainCodeRegion->getArgument(0)->getValueType()));
        ++firstArgumentIndex;
    }

    // Pass the closure context pointer.
    if(isClosure)
        argumentTypes.push_back(llvm::Type::getInt8PtrTy(*backend->getContext()));

    // Add the remaining arguments.
    for(size_t i = firstArgumentIndex; i < argumentCount; ++i)
        argumentTypes.push_back(backend->translateType(mainCodeRegion->getArgument(i)->getValueType()));

    auto functionType = llvm::FunctionType::get(resultType, argumentTypes, hasCVarArgs);

    // Guard for recursive definitions.
    {
        auto existent = backend->findGlobalValueTranslation(function);
        if(existent)
            return wrapLLVMValue(existent);
    }

    // TODO: Compute a proper linkage.
    auto parentFunction = function->getParentFunction();
    auto linkage = parentFunction ? llvm::GlobalValue::PrivateLinkage : llvm::GlobalValue::ExternalLinkage;
    auto name = backend->getNameMangler()->mangleSSAProgramEntity(function);
    
    currentFunction = llvm::Function::Create(functionType, linkage, name, *backend->getTargetModule());
    backend->setGlobalValueTranslation(function, currentFunction);
    currentFunctionBeingTranslated = function;

    if(!parentFunction)
        currentFunction->setDLLStorageClass(convertDLLStorageClass(function->getDllLinkageMode()));

    // Make the argument used for returning by reference with sret.
    if(mainCodeRegion->isReturningByReference())
    {
        auto structureResultType = backend->translateType(function->getValueType().staticAs<FunctionalType> ()->getResultType());
        currentFunction->getArg(0)->addAttr(llvm::Attribute::getWithStructRetType(*backend->getContext(), structureResultType));
    }
    
    // Zeroext/signext
    if(backend->isSignedIntegerType(mainCodeRegion->getResultType()))
        currentFunction->addAttribute(0, llvm::Attribute::SExt);
    else if(backend->isUnsignedIntegerType(mainCodeRegion->getResultType()))
        currentFunction->addAttribute(0, llvm::Attribute::ZExt);

    for(size_t i = firstArgumentIndex; i < argumentCount; ++i)
    {
        auto argType = mainCodeRegion->getArgument(i)->getValueType();
        auto paramIndex = isClosure ? i + 1 : i;
        if(backend->isSignedIntegerType(argType))
            currentFunction->addParamAttr(uint32_t(paramIndex), llvm::Attribute::SExt);
        else if(backend->isUnsignedIntegerType(argType))
            currentFunction->addParamAttr(uint32_t(paramIndex), llvm::Attribute::ZExt);
    }

    // Make the debug information
    llvm::DISubprogram *subprogram = nullptr;
    if(backend->getDIBuilder())
    {
        auto sourceLocation = function->getDefinitionSourcePosition();
        auto file = backend->getOrCreateDIFileForSourcePosition(sourceLocation);
        auto scope = file;
        int scopeLine = sourceLocation->getLine();

        auto flags = llvm::DINode::FlagPrototyped;
        auto spFlags = llvm::DISubprogram::SPFlagZero;
        if(!mainCodeRegion->isEmpty())
            spFlags = llvm::DISubprogram::SPFlagDefinition;

        subprogram = backend->getDIBuilder()->createFunction(
            scope, function->getValidNameString(), llvm::StringRef(),
            file, sourceLocation->getLine(),
            backend->getOrCreateDIFunctionType(function->getFunctionalType()), scopeLine,
            flags, spFlags
        );
        currentFunction->setSubprogram(subprogram);
    }

    // Translate the function body.
    translateMainCodeRegion(mainCodeRegion);

    // Finalize the debug info.
    if(subprogram)
        backend->getDIBuilder()->finalizeSubprogram(subprogram);

    // Verify the function.
    if(llvm::verifyFunction(*currentFunction, &llvm::errs()))
    {
        currentFunction->print(llvm::errs());
        abort();
    }

    return wrapLLVMValue(currentFunction);
}

AnyValuePtr SSALLVMValueVisitor::visitGlobalVariable(const SSAGlobalVariablePtr &globalVariable)
{
    // Guard for recursive definitions.
    {
        auto existent = backend->findGlobalValueTranslation(globalVariable);
        if(existent)
            return wrapLLVMValue(existent);
    }

    auto valueType = backend->translateType(globalVariable->getValueType());

    // Guard for recursive definitions.
    {
        auto existent = backend->findGlobalValueTranslation(globalVariable);
        if(existent)
            return wrapLLVMValue(existent);
    }

    auto parentFunction = globalVariable->getParentFunction();
    auto linkage = parentFunction ? llvm::GlobalValue::PrivateLinkage : llvm::GlobalValue::ExternalLinkage;
    auto name = backend->getNameMangler()->mangleSSAProgramEntity(globalVariable);

    if(!parentFunction)
        currentFunction->setDLLStorageClass(convertDLLStorageClass(globalVariable->getDllLinkageMode()));

    auto translatedGlobalVariable = new llvm::GlobalVariable(valueType, false, linkage, nullptr, name);
    backend->setGlobalValueTranslation(globalVariable, translatedGlobalVariable);

    // TODO: Set the global variable initializer.
    return wrapLLVMValue(translatedGlobalVariable);
}

AnyValuePtr SSALLVMValueVisitor::visitConstantLiteralValue(const SSAConstantLiteralValuePtr &constantValue)
{
    auto visitor = basicMakeObject<LLVMLiteralValueVisitor> ();
    visitor->backend = backend;
    return wrapLLVMValue(visitor->translateConstantLiteralValue(constantValue));
}

AnyValuePtr SSALLVMValueVisitor::visitTypeProgramEntity(const SSATypeProgramEntityPtr &value)
{
    (void)value;
    return wrapLLVMValue(llvm::Constant::getNullValue(llvm::Type::getInt8PtrTy(*backend->getContext())));
}

void SSALLVMValueVisitor::translateMainCodeRegion(const SSACodeRegionPtr &codeRegion)
{
    if(codeRegion->isEmpty())
        return;
    
    auto allocaBlock = llvm::BasicBlock::Create(*backend->getContext(), "", currentFunction);
    allocaBuilder = std::make_unique<llvm::IRBuilder<>>(allocaBlock);

    builder = std::make_unique<llvm::IRBuilder<>>(*backend->getContext());
    currentCodeRegion = codeRegion;

    // Map the source arguments.
    auto argumentCount = codeRegion->getArgumentCount();
    if(codeRegion->hasCVarArgs())
        --argumentCount;

    size_t firstArgumentIndex = 0;
    if(codeRegion->isReturningByReference())
    {
        auto sourceArgument = codeRegion->getArgument(0);
        auto targetArgument = currentFunction->getArg(0);
        localTranslatedValueMap.insert({sourceArgument, targetArgument});
        ++firstArgumentIndex;
    }

    bool isClosure = currentFunctionBeingTranslated->getFunctionalType()->isClosureType();
    for(size_t i = firstArgumentIndex; i < argumentCount; ++i)
    {
        auto sourceArgument = codeRegion->getArgument(i);
        auto targetArgument = currentFunction->getArg(uint32_t(i + (isClosure ? 1 : 0)));
        localTranslatedValueMap.insert({sourceArgument, targetArgument});
    }

    // Create the code region basic blocks.
    const auto &sourceBasicBlocks = codeRegion->getBasicBlocks();
    const auto basicBlockCount = codeRegion->getBasicBlocks().size();
    std::vector<llvm::BasicBlock*> translatedBlocks;
    translatedBlocks.reserve(basicBlockCount);
    for(auto &sourceBB : sourceBasicBlocks)
    {
        auto targetBasicBlock = llvm::BasicBlock::Create(*backend->getContext());
        localTranslatedValueMap.insert({sourceBB, targetBasicBlock});
        translatedBlocks.push_back(targetBasicBlock);
    }

    // Translate the blocks in-order
    for(size_t i = 0; i < basicBlockCount; ++i)
        translateBasicBlockInto(i, sourceBasicBlocks[i], translatedBlocks[i]);

    // Add the final entry.
    allocaBuilder->CreateBr(translatedBlocks.front());
}

llvm::Value *SSALLVMValueVisitor::translateCodeRegionWithArguments(const SSACodeRegionPtr &codeRegion, const std::vector<llvm::Value*> &arguments)
{
    sysmelAssert(arguments.size() == codeRegion->getArgumentCount());
    sysmelAssert(!codeRegion->isEmpty());

    // Map the code region arguments.
    for(size_t i = 0; i < arguments.size(); ++ i)
        localTranslatedValueMap.insert({codeRegion->getArgument(i), arguments[i]});

    // Get the result type.
    auto resultType = codeRegion->getResultType();
    auto resultTranslatedType = backend->translateType(resultType);

    // Create the merge block and the phi node if needed.
    auto mergeBlock = llvm::BasicBlock::Create(*backend->getContext());
    llvm::PHINode *resultPhi = nullptr;
    if(!resultType->isVoidType())
        resultPhi = llvm::PHINode::Create(resultTranslatedType, 0, "", mergeBlock);

    // Create the code region basic blocks.
    const auto &sourceBasicBlocks = codeRegion->getBasicBlocks();
    const auto basicBlockCount = sourceBasicBlocks.size();
    std::vector<llvm::BasicBlock*> translatedBlocks;
    translatedBlocks.reserve(basicBlockCount);
    for(auto &sourceBB : sourceBasicBlocks)
    {
        auto targetBasicBlock = llvm::BasicBlock::Create(*backend->getContext());
        localTranslatedValueMap.insert({sourceBB, targetBasicBlock});
        translatedBlocks.push_back(targetBasicBlock);
    }

    // Enter into the code region.
    builder->CreateBr(translatedBlocks.front());

    // Keep track of the code region context.
    auto oldCodeRegion = currentCodeRegion;
    auto oldCodeRegionReturnBlock = currentCodeRegionReturnBlock;
    auto oldCodeRegionReturnPhi = currentCodeRegionResultPHI;
    doWithEnsure([&](){
        currentCodeRegion = codeRegion;
        currentCodeRegionReturnBlock = mergeBlock;
        currentCodeRegionResultPHI = resultPhi;

        // Translate the blocks in-order
        for(size_t i = 0; i < basicBlockCount; ++i)
            translateBasicBlockInto(i, sourceBasicBlocks[i], translatedBlocks[i]);
    }, [&]{
        currentCodeRegion = oldCodeRegion;
        currentCodeRegionReturnBlock = oldCodeRegionReturnBlock;
        currentCodeRegionResultPHI = oldCodeRegionReturnPhi;
    });

    mergeBlock->insertInto(currentFunction);
    builder->SetInsertPoint(mergeBlock);

    llvm::Value *result = resultPhi;
    if(!result)
        result = llvm::UndefValue::get(resultTranslatedType);
    else
        result = simplifyDegeneratePhi(resultPhi);
    return result;
}

void SSALLVMValueVisitor::translateBasicBlockInto(size_t index, const SSABasicBlockPtr &sourceBasicBlock, llvm::BasicBlock *targetBasicBlock)
{
    // Insert the block into the target function.
    targetBasicBlock->insertInto(currentFunction);
    builder->SetInsertPoint(targetBasicBlock);

    // Create the code region result finalization flag if it is required.
    if(index == 0)
    {
        auto captureCount = currentCodeRegion->getCaptureCount();
        if(captureCount > 0)
        {
            auto contextArgumentIndex = currentCodeRegion->isReturningByReference() ? 1 : 0;

            std::vector<llvm::Type*> contextElementTypes;
            contextElementTypes.reserve(1 + captureCount);
            contextElementTypes.push_back(llvm::Type::getInt8PtrTy(*backend->getContext()));

            for(size_t i = 0; i < captureCount; ++i)
            {
                auto captureType = currentCodeRegion->getCapture(i)->getValueType();
                auto translatedCaptureType = backend->translateType(captureType);
                if(captureType->isPassedByReference())
                    contextElementTypes.push_back(llvm::PointerType::getUnqual(translatedCaptureType));
                else
                    contextElementTypes.push_back(translatedCaptureType);
            }

            auto contextStructType = llvm::StructType::get(*backend->getContext(), contextElementTypes);
            auto contextPointerType = llvm::PointerType::getUnqual(contextStructType);

            auto contextPointer = builder->CreatePointerCast(currentFunction->getArg(contextArgumentIndex), contextPointerType);
            for(size_t i = 0; i < captureCount; ++i)
            {
                const auto &capture = currentCodeRegion->getCapture(i);
                auto captureType = capture->getValueType();
                auto capturePointer = builder->CreateConstInBoundsGEP2_32(nullptr, contextPointer, 0, uint32_t(i + 1));
                auto captureValue = capturePointer;
                if(!captureType->isPassedByReference())
                    captureValue = builder->CreateLoad(capturePointer);

                localTranslatedValueMap.insert({capture, captureValue});
                declareDebugCapture(capture, capturePointer);
            }
        }

        auto argumentCount = currentCodeRegion->getArgumentCount();
        for(size_t i = 0; i < argumentCount; ++i)
        {
            const auto &arg = currentCodeRegion->getArgument(i);
            
            // Argument local finalization.
            if(i == 0 && arg->isLocalFinalizationRequired())
                createLocalFinalizationFlagFor(arg);

            // Declare the local argument.
            declareDebugArgument(arg);
        }
    }

    sourceBasicBlock->instructionsDo([&](const SSAInstructionPtr &instruction){
        translateInstruction(instruction);
    });
    sysmelAssert(isLastTerminator());
}

void SSALLVMValueVisitor::declareDebugArgument(const SSACodeRegionArgumentPtr &argument)
{
    auto diBuilder = backend->getDIBuilder();
    if(!diBuilder)
        return;

    const auto &variable = argument->getVariable();
    if(!variable)
        return;

    auto debugVariable = translateDebugLocalVariable(variable);   
    if(!debugVariable)
        return;

    auto location = backend->getDILocationFor(argument->getDefinitionPosition(), currentFunction->getSubprogram());
    auto value = translateValue(argument);
    
    auto address = value;

    // If the argument is not in a reference, make a copy for exposing it in the debugger.
    if(!variable->getValueType()->isPassedByReference())
    {
        address = allocaBuilder->CreateAlloca(address->getType());
        builder->CreateStore(value, address);
    }

    diBuilder->insertDeclare(address, debugVariable, diBuilder->createExpression(), location, builder->GetInsertBlock());
}

void SSALLVMValueVisitor::declareDebugCapture(const SSACodeRegionCapturePtr &capture, llvm::Value *capturePointer)
{
    (void)capture;
    (void)capturePointer;
}

llvm::DILocalVariable *SSALLVMValueVisitor::translateDebugLocalVariable(const VariablePtr &variable)
{
    auto sourcePosition = variable->getDefinitionPosition();
    auto file = backend->getOrCreateDIFileForSourcePosition(sourcePosition);
    auto line = sourcePosition->getLine();
    auto debugType = backend->translateDIType(variable->getValueType());

    // Arguments
    auto scope = currentFunction->getSubprogram();
    if(variable->isArgumentVariable())
    {
        auto argumentIndex = variable.staticAs<ArgumentVariable> ()->argumentIndex;
        return backend->getDIBuilder()->createParameterVariable(
            scope, variable->getValidNameString(), 1 + argumentIndex,
            file, line, debugType
        );
    }

    if(validAnyValue(variable->getName())->isHiddenNameSymbol())
        return nullptr;

    return backend->getDIBuilder()->createAutoVariable(
        scope, variable->getValidNameString(),
        file, line, debugType
    );
}

void SSALLVMValueVisitor::translateInstruction(const SSAInstructionPtr &instruction)
{
    withSourcePositionDo(instruction->getSourcePosition(), [&]{
        auto instructionValue = visitValueForLLVM(instruction);
        localTranslatedValueMap.insert({instruction, instructionValue});
    });
}

AnyValuePtr SSALLVMValueVisitor::visitCallInstruction(const SSACallInstructionPtr &instruction)
{
    {
        auto calledFunction = instruction->getFunction();
        if(calledFunction->isSSAFunction())
        {
            auto intrinsicName = calledFunction.staticAs<SSAFunction>()->getIntrinsicName();
            if(intrinsicName && intrinsicName->isLiteralSymbol())
                return wrapLLVMValue(translateIntrinsicCall(intrinsicName->unwrapAsString(), instruction));
        }
    }

    return wrapLLVMValue(translateCall(instruction));
}

llvm::Value *SSALLVMValueVisitor::translateIntrinsicCall(const std::string &intrinsicName, const SSACallInstructionPtr &instruction)
{
    auto it = intrinsicGenerators.find(intrinsicName);
    if(it != intrinsicGenerators.end())
    {
        IntrinsicGenerationContext context;
        context.self = this;
        context.builder = builder.get();
        context.intrinsicName = intrinsicName;
        context.originalResultType = instruction->getValueType();
        if(!context.originalResultType->isCVarArgType())
            context.resultType = backend->translateType(context.originalResultType);
        context.originalArguments = instruction->getArguments();
        context.arguments.reserve(instruction->getArguments().size());
        for(auto &arg : instruction->getArguments())
            context.arguments.push_back(translateValue(arg));
        return it->second(context);
    }

    return translateCall(instruction);
}

llvm::Value *SSALLVMValueVisitor::translateCall(const SSACallInstructionPtr &instruction)
{
    const auto &calledFunction = instruction->getFunction();
    auto functionType = calledFunction->getValueType();
    sysmelAssert(functionType->isFunctionalType());

    bool isClosure = functionType->isClosureType();
    bool isReturningByReference = functionType.staticAs<FunctionalType> ()->getResultType()->isReturnedByReference();
    
    auto translatedFunctionType = backend->translateType(functionType);
    auto translatedCalledFunction = translateValue(calledFunction);
    sysmelAssert(translatedCalledFunction->getType()->isPointerTy());

    std::vector<llvm::Value*> arguments;
    auto argumentCount = instruction->getArguments().size();
    arguments.reserve((isClosure ? 1 : 0) + argumentCount);

    size_t firstArgumentIndex = 0;

    // Translate the result argument first.
    if(isReturningByReference)
    {
        arguments.push_back(translateValue(instruction->getArguments()[0]));
        ++firstArgumentIndex;
    }

    llvm::FunctionType *calledFunctionType = nullptr;
    auto calledFunctionValue = translatedCalledFunction;

    // Add the closure argument first or second.
    if(isClosure)
    {
        auto unwrappedType = llvm::cast<llvm::PointerType> (translatedFunctionType)->getElementType();
        sysmelAssert(unwrappedType->isPointerTy());
        unwrappedType = llvm::cast<llvm::PointerType> (unwrappedType)->getElementType();

        sysmelAssert(unwrappedType->isFunctionTy());
        calledFunctionType = llvm::cast<llvm::FunctionType> (unwrappedType);

        calledFunctionValue = builder->CreateLoad(translatedCalledFunction);
        arguments.push_back(builder->CreatePointerCast(translatedCalledFunction, llvm::Type::getInt8PtrTy(*backend->getContext())));
    }
    else
    {
        sysmelAssert(translatedFunctionType->isFunctionTy());
        calledFunctionType = llvm::cast<llvm::FunctionType> (translatedFunctionType);
    }

    // Pass the remaining arguments.
    for(size_t i = firstArgumentIndex; i < instruction->getArguments().size(); ++i)
        arguments.push_back(translateValue(instruction->getArguments()[i]));

    return builder->CreateCall(calledFunctionType, calledFunctionValue, arguments);
}

AnyValuePtr SSALLVMValueVisitor::visitDeclareLocalVariableInstruction(const SSADeclareLocalVariableInstructionPtr &instruction)
{
    auto diBuilder = backend->getDIBuilder();
    auto value = translateValue(instruction->getValue());
    auto variable = instruction->getVariable();
    if(diBuilder && variable && !validAnyValue(variable->getName())->isHiddenNameSymbol())
    {
        auto diVariable = translateDebugLocalVariable(variable);
        if(diVariable)
        {
            auto address = value;
            auto refType = variable->getReferenceType()->asUndecoratedType();
            auto scope = currentFunction->getSubprogram();
            auto location = backend->getDILocationFor(instruction->getSourcePosition(), scope);
            if(!address->getType()->isPointerTy() || (!refType->isReferenceLikeType() && !refType->isPassedByReference()))
            {
                address = allocaBuilder->CreateAlloca(value->getType());
                builder->CreateStore(value, address);
            }

            diBuilder->insertDeclare(address, diVariable, diBuilder->createExpression(), location, builder->GetInsertBlock());
        }
    }

    return wrapLLVMValue(makeVoidValue());
}

AnyValuePtr SSALLVMValueVisitor::visitDoWithCleanupInstruction(const SSADoWithCleanupInstructionPtr &instruction)
{
    auto cleanUpRegion = instruction->getCleanUpRegion();
    cleanUpRegionStack.push_back(cleanUpRegion);
    auto result = doWithEnsure([&]{
        return translateCodeRegionWithArguments(instruction->getBodyRegion(), {});
    }, [&]{
        cleanUpRegionStack.pop_back();
    });

    if(!llvm::pred_empty(builder->GetInsertBlock()))
        translateCodeRegionWithArguments(cleanUpRegion, {});
    return wrapLLVMValue(result);
}

AnyValuePtr SSALLVMValueVisitor::visitDoWhileInstruction(const SSADoWhileInstructionPtr &instruction)
{
    auto body = llvm::BasicBlock::Create(*backend->getContext(), "doWhileBody");
    auto conditionBlock = llvm::BasicBlock::Create(*backend->getContext(), "doWhileCondition");
    auto continueBlock = llvm::BasicBlock::Create(*backend->getContext(), "doWhileContinue");
    auto merge = llvm::BasicBlock::Create(*backend->getContext(), "doWhileMerge");

    // Emit the body.
    builder->CreateBr(body);
    builder->SetInsertPoint(body);
    body->insertInto(currentFunction);

    auto bodyRegion = instruction->getBodyRegion();
    if(bodyRegion)
    {
        // TODO: Store the state required for continue/break.
        translateCodeRegionWithArguments(bodyRegion, {});
    }

    if(!isLastTerminator())
        builder->CreateBr(conditionBlock);
    
    // Emit the condition.
    builder->SetInsertPoint(conditionBlock);
    conditionBlock->insertInto(currentFunction);

    auto conditionRegion = instruction->getConditionRegion();
    if(conditionRegion)
    {
        auto condition = translateCodeRegionWithArguments(conditionRegion, {});
        if(!isLastTerminator())
            builder->CreateCondBr(coerceBooleanIntoI1(condition), continueBlock, merge);
    }
    else
    {
        builder->CreateBr(continueBlock);
    }

    // Emit the continue block.
    builder->SetInsertPoint(continueBlock);
    continueBlock->insertInto(currentFunction);

    auto continueRegion = instruction->getContinueRegion();
    if(continueRegion)
    {
        // TODO: Store the state required for continue/break.
        translateCodeRegionWithArguments(continueRegion, {});
    }

    if(!isLastTerminator())
        builder->CreateBr(body);

    // Merge the control flow.
    builder->SetInsertPoint(merge);
    merge->insertInto(currentFunction);

    return wrapLLVMValue(makeVoidValue());
}

AnyValuePtr SSALLVMValueVisitor::visitGetAggregateFieldReferenceInstruction(const SSAGetAggregateFieldReferenceInstructionPtr &instruction)
{
    sysmelAssert(instruction->getAggregate()->getValueType()->isPointerLikeType());
    auto aggregate = translateValue(instruction->getAggregate());
    return wrapLLVMValue(builder->CreateConstInBoundsGEP2_32(nullptr, aggregate, 0, instruction->getFieldVariable()->getSlotIndex()));
}

AnyValuePtr SSALLVMValueVisitor::visitGetAggregateSlotReferenceInstruction(const SSAGetAggregateSlotReferenceInstructionPtr &instruction)
{
    sysmelAssert(instruction->getAggregate()->getValueType()->isPointerLikeType());
    auto aggregate = translateValue(instruction->getAggregate());
    auto index = instruction->getSlotIndex();
    if(index->isSSAConstantLiteralValue())
    {
        auto constantValue = validAnyValue(index.staticAs<SSAConstantLiteralValue> ()->getValue());
        if(constantValue->isLiteralInteger())
            return wrapLLVMValue(builder->CreateConstInBoundsGEP2_32(nullptr, aggregate, 0, constantValue->unwrapAsInt32()));
    }

    auto translatedIndex = translateValue(index);
    auto indexType = backend->translateType(Type::getUIntPointerType());
    return wrapLLVMValue(builder->CreateGEP(nullptr, aggregate, {llvm::ConstantInt::get(indexType, 0), translatedIndex}));
}

AnyValuePtr SSALLVMValueVisitor::visitIfInstruction(const SSAIfInstructionPtr &instruction)
{
    auto condition = translateValue(instruction->getCondition());
    auto thenRegion = instruction->getTrueRegion();
    auto elseRegion = instruction->getFalseRegion();
    auto resultType = instruction->getValueType();

    auto thenBlock = llvm::BasicBlock::Create(*backend->getContext(), "ifThen");
    auto elseBlock = llvm::BasicBlock::Create(*backend->getContext(), "ifElse");
    auto mergeBlock = llvm::BasicBlock::Create(*backend->getContext(), "ifMerge");

    llvm::PHINode *phiNode = nullptr;
    if(!resultType->isVoidType())
        phiNode = llvm::PHINode::Create(backend->translateType(resultType), 0, "ifResult", mergeBlock);

    // Condition.
    builder->CreateCondBr(coerceBooleanIntoI1(condition), thenBlock, elseBlock);
    builder->SetInsertPoint(thenBlock);

    // Then block
    thenBlock->insertInto(currentFunction);
    
    llvm::Value *thenResult = nullptr;
    if(thenRegion)
        thenResult = translateCodeRegionWithArguments(thenRegion, {});

    if(!isLastTerminator())
    {
        builder->CreateBr(mergeBlock);
        if(phiNode && thenResult)
            phiNode->addIncoming(thenResult, builder->GetInsertBlock());
    }

    // Else block
    builder->SetInsertPoint(elseBlock);
    elseBlock->insertInto(currentFunction);

    llvm::Value *elseResult = nullptr;
    if(elseRegion)
        elseResult = translateCodeRegionWithArguments(elseRegion, {});

    if(!isLastTerminator())
    {
        builder->CreateBr(mergeBlock);
        if(phiNode && elseResult)
            phiNode->addIncoming(elseResult, builder->GetInsertBlock());
    }

    // Merge block.
    builder->SetInsertPoint(mergeBlock);
    mergeBlock->insertInto(currentFunction);

    if(phiNode)
        return wrapLLVMValue(simplifyDegeneratePhi(phiNode));
    else
        return wrapLLVMValue(makeVoidValue());
}


AnyValuePtr SSALLVMValueVisitor::visitLocalVariableInstruction(const SSALocalVariableInstructionPtr &instruction)
{
    auto variableValueType = instruction->getVariableValueType();
    auto valueType = backend->translateType(variableValueType);
    auto alloca = allocaBuilder->CreateAlloca(valueType);
    alloca->setAlignment(llvm::Align(std::max(uint64_t(1), variableValueType->getMemoryAlignment())));
    if(instruction->isLocalFinalizationRequired())
        createLocalFinalizationFlagFor(instruction);
    return wrapLLVMValue(alloca);
}

void SSALLVMValueVisitor::returnFromFunction(const SSAValuePtr &resultValue)
{
    if(resultValue)
    {
        auto translatedResultValue = translateValue(resultValue);
        emitCleanUpsForReturning();
        if(!resultValue->getValueType()->isVoidType())
            builder->CreateRet(translatedResultValue);
        else
            builder->CreateRetVoid();
    }
    else
    {
        emitCleanUpsForReturning();
        builder->CreateRetVoid();
    }
}

void SSALLVMValueVisitor::emitCleanUpsForReturning()
{
    for(size_t i = 0; i < cleanUpRegionStack.size(); ++i)
    {
        auto &region = cleanUpRegionStack[cleanUpRegionStack.size() - i - 1];
        translateCodeRegionWithArguments(region, {});
    }
}

AnyValuePtr SSALLVMValueVisitor::visitMakeClosureInstruction(const SSAMakeClosureInstructionPtr &instruction)
{
    auto closureImplementation = translateValue(instruction->getClosureImplementation());
    sysmelAssert(closureImplementation->getType()->isPointerTy());

    auto rawCapturedValues = instruction->getCapturedValues();
    auto resultType = backend->translateType(instruction->getValueType());
    sysmelAssert(resultType->isPointerTy());

    std::vector<llvm::Type*> closureStructTypes;
    closureStructTypes.reserve(1 + rawCapturedValues.size());

    // Extract the closure struct types,
    closureStructTypes.push_back(closureImplementation->getType());
    for(auto &capture : rawCapturedValues)
    {
        auto captureType = capture->getValueType()->asUndecoratedType();
        auto translatedCaptureType = backend->translateType(captureType);
        if(captureType->isPassedByReference())
            closureStructTypes.push_back(llvm::PointerType::getUnqual(translatedCaptureType));
        else
            closureStructTypes.push_back(translatedCaptureType);
    }

    auto closureStructType = llvm::StructType::get(*backend->getContext(), closureStructTypes);

    // Allocate the closure.
    auto closureStruct = allocaBuilder->CreateAlloca(closureStructType);

    // Set the closure data.
    builder->CreateStore(closureImplementation, builder->CreateConstInBoundsGEP2_32(nullptr, closureStruct, 0, 0));
    for(size_t i = 0; i < rawCapturedValues.size(); ++i)
    {
        auto capture = translateValue(rawCapturedValues[i]);
        builder->CreateStore(capture, builder->CreateConstInBoundsGEP2_32(nullptr, closureStruct, 0, uint32_t(i + 1)));
    }

    // Cast and return the closure.
    return wrapLLVMValue(builder->CreatePointerCast(closureStruct, resultType));
}

AnyValuePtr SSALLVMValueVisitor::visitReturnFromFunctionInstruction(const SSAReturnFromFunctionInstructionPtr &instruction)
{
    returnFromFunction(instruction->getValue());
    return wrapLLVMValue(makeVoidValue());
}

AnyValuePtr SSALLVMValueVisitor::visitReturnFromRegionInstruction(const SSAReturnFromRegionInstructionPtr &instruction)
{
    if(!currentCodeRegionReturnBlock)
    {
        returnFromFunction(instruction->getValue());
    }
    else
    {
        auto resultValue = instruction->getValue();
        if(resultValue)
        {
            auto translatedResultValue = translateValue(resultValue);
            if(currentCodeRegionResultPHI)
                currentCodeRegionResultPHI->addIncoming(translatedResultValue, builder->GetInsertBlock());
        }
        builder->CreateBr(currentCodeRegionReturnBlock);
    }
        
    return wrapLLVMValue(makeVoidValue());
}

AnyValuePtr SSALLVMValueVisitor::visitLoadInstruction(const SSALoadInstructionPtr &instruction)
{
    auto reference = translateValue(instruction->getReference());
    return wrapLLVMValue(builder->CreateLoad(reference));
}

AnyValuePtr SSALLVMValueVisitor::visitStoreInstruction(const SSAStoreInstructionPtr &instruction)
{
    auto value = translateValue(instruction->getValue());
    auto reference = translateValue(instruction->getReference());
    builder->CreateStore(value, reference);
    return wrapLLVMValue(makeVoidValue());
}

AnyValuePtr SSALLVMValueVisitor::visitUnreachableInstruction(const SSAUnreachableInstructionPtr &)
{
    builder->CreateUnreachable();
    return wrapLLVMValue(makeVoidValue());
}

AnyValuePtr SSALLVMValueVisitor::visitWhileInstruction(const SSAWhileInstructionPtr &instruction)
{
    auto header = llvm::BasicBlock::Create(*backend->getContext(), "whileHeader");
    auto body = llvm::BasicBlock::Create(*backend->getContext(), "whileBody");
    auto continueBlock = llvm::BasicBlock::Create(*backend->getContext(), "whileContinue");
    auto merge = llvm::BasicBlock::Create(*backend->getContext(), "whileMerge");

    builder->CreateBr(header);
    builder->SetInsertPoint(header);
    header->insertInto(currentFunction);
    
    // Emit the condition.
    auto conditionRegion = instruction->getConditionRegion();
    if(conditionRegion)
    {
        auto condition = translateCodeRegionWithArguments(conditionRegion, {});
        builder->CreateCondBr(coerceBooleanIntoI1(condition), body, merge);
    }
    else
    {
        builder->CreateBr(body);
    }

    // Emit the body.
    builder->SetInsertPoint(body);
    body->insertInto(currentFunction);

    auto bodyRegion = instruction->getBodyRegion();
    if(bodyRegion)
    {
        // TODO: Store the state required for continue/break.
        translateCodeRegionWithArguments(bodyRegion, {});
    }

    if(!isLastTerminator())
        builder->CreateBr(continueBlock);

    // Emit the continue block.
    builder->SetInsertPoint(continueBlock);
    continueBlock->insertInto(currentFunction);

    auto continueRegion = instruction->getContinueRegion();
    if(continueRegion)
    {
        // TODO: Store the state required for continue/break.
        translateCodeRegionWithArguments(continueRegion, {});
    }

    if(!isLastTerminator())
        builder->CreateBr(header);

    // Merge the control flow.
    builder->SetInsertPoint(merge);
    merge->insertInto(currentFunction);

    return wrapLLVMValue(makeVoidValue());
}

AnyValuePtr SSALLVMValueVisitor::visitUpcastInstruction(const SSAUpcastInstructionPtr &instruction)
{
    return wrapLLVMValue(builder->CreateBitCast(translateValue(instruction->getValue()), backend->translateType(instruction->getTargetType())));
}

AnyValuePtr SSALLVMValueVisitor::visitDowncastInstruction(const SSADowncastInstructionPtr &instruction)
{
    return wrapLLVMValue(builder->CreateBitCast(translateValue(instruction->getValue()), backend->translateType(instruction->getTargetType())));
}

AnyValuePtr SSALLVMValueVisitor::visitBitcastInstruction(const SSABitcastInstructionPtr &instruction)
{
    return wrapLLVMValue(builder->CreateBitCast(translateValue(instruction->getValue()), backend->translateType(instruction->getTargetType())));
}

AnyValuePtr SSALLVMValueVisitor::visitEnableLocalFinalization(const SSAEnableLocalFinalizationPtr &instruction)
{
    auto flagType = llvm::Type::getInt1Ty(*backend->getContext());
    builder->CreateStore(llvm::ConstantInt::get(flagType, 1), findLocalFinalizationFlagFor(instruction->getLocalVariable()));
    return wrapLLVMValue(makeVoidValue());
}

AnyValuePtr SSALLVMValueVisitor::visitLocalFinalization(const SSALocalFinalizationPtr &instruction)
{
    auto flag = builder->CreateLoad(findLocalFinalizationFlagFor(instruction->getLocalVariable()));

    auto thenRegion = instruction->getFinalizationRegion();

    auto thenBlock = llvm::BasicBlock::Create(*backend->getContext(), "localFinalizationCondition");
    auto mergeBlock = llvm::BasicBlock::Create(*backend->getContext(), "localFinalizationMerge");

    // Condition.
    builder->CreateCondBr(flag, thenBlock, mergeBlock);
    builder->SetInsertPoint(thenBlock);

    // Then block
    thenBlock->insertInto(currentFunction);
    
    translateCodeRegionWithArguments(thenRegion, {});

    if(!isLastTerminator())
        builder->CreateBr(mergeBlock);

    // Merge block.
    builder->SetInsertPoint(mergeBlock);
    mergeBlock->insertInto(currentFunction);

    return wrapLLVMValue(makeVoidValue());
}

llvm::Value *SSALLVMValueVisitor::makeVoidValue()
{
    return llvm::UndefValue::get(backend->translateType(Type::getVoidType()));
}

llvm::Value *SSALLVMValueVisitor::coerceBooleanIntoI1(llvm::Value *value)
{
    return builder->CreateTruncOrBitCast(value, llvm::Type::getInt1Ty(*backend->getContext()));
}

llvm::Value *SSALLVMValueVisitor::coerceI1IntoBoolean8(llvm::Value *value)
{
    return builder->CreateZExtOrBitCast(value, llvm::Type::getInt8Ty(*backend->getContext()));
}

llvm::Value *SSALLVMValueVisitor::simplifyDegeneratePhi(llvm::PHINode *phi)
{
    auto incomingCount = phi->getNumIncomingValues();
    if(incomingCount == 0)
    {
        auto replacement = llvm::UndefValue::get(phi->getType());
        phi->replaceAllUsesWith(replacement);
        phi->eraseFromParent();
        return replacement;
    }
    else if(incomingCount == 1)
    {
        auto replacement = phi->getIncomingValue(0);
        phi->replaceAllUsesWith(replacement);
        phi->eraseFromParent();
        return replacement;
    }

    return phi;
}

bool SSALLVMValueVisitor::isLastTerminator() const
{
    auto block = builder->GetInsertBlock();
    return !block->empty() && block->back().isTerminator();
}

llvm::Value *SSALLVMValueVisitor::createLocalFinalizationFlagFor(const SSAValuePtr &localVariable)
{
    auto flagType = llvm::Type::getInt1Ty(*backend->getContext());
    auto flag = allocaBuilder->CreateAlloca(flagType);
    builder->CreateStore(llvm::ConstantInt::get(flagType, 0), flag);
    localTranslatedFinalizationEnabledFlagMap.insert({localVariable, flag});
    return flag;
}

llvm::Value *SSALLVMValueVisitor::findLocalFinalizationFlagFor(const SSAValuePtr &localVariable)
{
    return localTranslatedFinalizationEnabledFlagMap.find(localVariable)->second;
}

void SSALLVMValueVisitor::withSourcePositionDo(const ASTSourcePositionPtr &sourcePosition, const std::function<void()> &aBlock)
{
    if(!backend->getDIBuilder() || !builder)
        return aBlock();

    auto oldLocation = builder->getCurrentDebugLocation();
    auto newLocation = backend->getDILocationFor(sourcePosition, currentFunction->getSubprogram());
    builder->SetCurrentDebugLocation(newLocation);

    doWithEnsure([&]{
        return aBlock();
    }, [&]{
        builder->SetCurrentDebugLocation(oldLocation);
    });
}
} // End of namespace Environment
} // End of namespace Sysmel