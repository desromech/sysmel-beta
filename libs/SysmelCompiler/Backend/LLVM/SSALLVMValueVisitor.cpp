#include "Backend/LLVM/SSALLVMValueVisitor.hpp"

#include "Environment/Type.hpp"

#include "Environment/SSAConstantLiteralValue.hpp"
#include "Environment/SSAFunction.hpp"
#include "Environment/SSACodeRegion.hpp"
#include "Environment/SSACodeRegionArgument.hpp"
#include "Environment/SSABasicBlock.hpp"
#include "Environment/SSAGlobalVariable.hpp"

#include "Environment/SSACallInstruction.hpp"
#include "Environment/SSALocalVariableInstruction.hpp"
#include "Environment/SSADoWithCleanupInstruction.hpp"
#include "Environment/SSAReturnFromFunctionInstruction.hpp"
#include "Environment/SSAReturnFromRegionInstruction.hpp"
#include "Environment/SSAStoreInstruction.hpp"
#include "Environment/SSAUnreachableInstruction.hpp"
#include "Environment/SSAWhileInstruction.hpp"

#include "Environment/BootstrapTypeRegistration.hpp"

#include "Environment/ControlFlowUtilities.hpp"

namespace Sysmel
{
namespace LLVM
{

/**
 * I am the base interface for a SSA based code generation backend
 */
class SSALLVMValue : public SubtypeOf<CompilerObject, SSALLVMValue>
{
public:
    static constexpr char const __typeName__[] = "SSALLVMValue";

    llvm::Value *value = nullptr;
};

static BootstrapTypeRegistration<SSALLVMValueVisitor> SSALLVMValueVisitorTypeRegistration;
static BootstrapTypeRegistration<SSALLVMValue> SSALLVMValueTypeRegistration;

llvm::Value *SSALLVMValueVisitor::visitValueForLLVM(const SSAValuePtr &value)
{
    return visitValue(value).staticAs<SSALLVMValue>()->value;
}

llvm::Value *SSALLVMValueVisitor::translateValue(const SSAValuePtr &value)
{
    auto it = localTranslatedValueMap.find(value);
    if(it != localTranslatedValueMap.end())
        return it->second;
    
    assert(!value->isSSAInstruction());
    auto result = visitValueForLLVM(value);
    if(value->isSSACodeRegionLocalValue())
        localTranslatedValueMap.insert({value, result});
    return result;
}

AnyValuePtr SSALLVMValueVisitor::wrapLLVMValue(llvm::Value *value)
{
    auto wrapper = basicMakeObject<SSALLVMValue> ();
    wrapper->value = value;
    return wrapper;
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
    std::vector<llvm::Type*> argumentTypes;
    argumentTypes.reserve(mainCodeRegion->getArgumentCount());
    for(auto &arg : mainCodeRegion->getArguments())
        argumentTypes.push_back(backend->translateType(arg->getValueType()));

    auto functionType = llvm::FunctionType::get(resultType, argumentTypes, false);

    // Guard for recursive definitions.
    {
        auto existent = backend->findGlobalValueTranslation(function);
        if(existent)
            return wrapLLVMValue(existent);
    }

    // TODO: Compute a proper linkage.
    auto linkage = llvm::GlobalValue::LinkageTypes::ExternalLinkage;
    // TODO: Compute a proper name.
    auto name = validAnyValue(function->getName())->asString();
    
    currentFunction = llvm::Function::Create(functionType, linkage, name, *backend->getTargetModule());
    backend->setGlobalValueTranslation(function, currentFunction);

    // Translate the function body.
    translateMainCodeRegion(mainCodeRegion);

    // Verify the function.
    llvm::verifyFunction(*currentFunction);
    
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

    // TODO: Compute a proper linkage.
    auto linkage = llvm::GlobalValue::LinkageTypes::ExternalLinkage;
    // TODO: Compute a proper name.
    auto name = validAnyValue(globalVariable->getName())->asString();

    auto translatedGlobalVariable = new llvm::GlobalVariable(valueType, false, linkage, nullptr, name);
    backend->setGlobalValueTranslation(globalVariable, translatedGlobalVariable);

    // TODO: Set the global variable initializer.
    return wrapLLVMValue(translatedGlobalVariable);
}

AnyValuePtr SSALLVMValueVisitor::visitConstantLiteralValue(const SSAConstantLiteralValuePtr &constantValue)
{
    auto value = validAnyValue(constantValue->getValue());
    auto valueType = constantValue->getValueType();
    auto translatedValueType = backend->translateType(valueType);
    // FIXME: Use here a proper visitor pattern.
    if(valueType->isVoidType())
    {
        return wrapLLVMValue(llvm::UndefValue::get(translatedValueType));
    }
    else if(value->isPrimitiveIntegerTypeValue())
    {
        auto largeInteger = value->unwrapAsLargeInteger();
        return wrapLLVMValue(llvm::ConstantInt::get(static_cast<llvm::IntegerType*> (translatedValueType), largeInteger.asString(), 10));
    }
    else if(value->isPrimitiveCharacterTypeValue())
    {
        auto data = value->unwrapAsChar32();
        return wrapLLVMValue(llvm::ConstantInt::get(static_cast<llvm::IntegerType*> (translatedValueType), data, false));
    }
    else if(value->isPrimitiveFloatTypeValue())
    {
        auto data = value->unwrapAsFloat64();
        return wrapLLVMValue(llvm::ConstantFP::get(translatedValueType, data));
    }
    else if(value->isPrimitiveBooleanTypeValue())
    {
        auto data = value->unwrapAsBoolean();
        return wrapLLVMValue(llvm::ConstantInt::get(static_cast<llvm::IntegerType*> (translatedValueType), data ? 1 : 0, false));
    }

    assert("TODO: visitConstantLiteralValue " && false);
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
    for(size_t i = 0; i < codeRegion->getArgumentCount(); ++i)
    {
        auto sourceArgument = codeRegion->getArgument(i);
        auto targetArgument = currentFunction->getArg(i);
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
        translateBasicBlockInto(sourceBasicBlocks[i], translatedBlocks[i]);

    // Add the final entry.
    allocaBuilder->CreateBr(translatedBlocks.front());
}

llvm::Value *SSALLVMValueVisitor::translateCodeRegionWithArguments(const SSACodeRegionPtr &codeRegion, const std::vector<llvm::Value*> &arguments)
{
    assert(arguments.size() == codeRegion->getArgumentCount());
    assert(!codeRegion->isEmpty());

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
    const auto basicBlockCount = codeRegion->getBasicBlocks().size();
    std::vector<llvm::BasicBlock*> translatedBlocks;
    translatedBlocks.reserve(basicBlockCount);
    for(auto &sourceBB : sourceBasicBlocks)
    {
        auto targetBasicBlock = llvm::BasicBlock::Create(*backend->getContext());
        localTranslatedValueMap.insert({sourceBB, targetBasicBlock});
        translatedBlocks.push_back(targetBasicBlock);
    }

    // Enter into the code region.
    builder->CreateBr(translatedBlocks.back());

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
            translateBasicBlockInto(sourceBasicBlocks[i], translatedBlocks[i]);
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
    return result;
}

void SSALLVMValueVisitor::translateBasicBlockInto(const SSABasicBlockPtr &sourceBasicBlock, llvm::BasicBlock *targetBasicBlock)
{
    // Insert the block into the target function.
    targetBasicBlock->insertInto(currentFunction);
    builder->SetInsertPoint(targetBasicBlock);

    sourceBasicBlock->instructionsDo([&](const SSAInstructionPtr &instruction){
        translateInstruction(instruction);
    });
}

void SSALLVMValueVisitor::translateInstruction(const SSAInstructionPtr &instruction)
{
    auto instructionValue = visitValueForLLVM(instruction);
    localTranslatedValueMap.insert({instruction, instructionValue});
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

std::unordered_map<std::string, std::function<llvm::Value* (SSALLVMValueVisitor*, llvm::IRBuilder<>*, const std::vector<llvm::Value*> &, const SSAValuePtrList&, const std::string&)>> SSALLVMValueVisitor::intrinsicGenerators = {
    {"integer.less-than", +[](SSALLVMValueVisitor* self, llvm::IRBuilder<> *builder, const std::vector<llvm::Value*> &arguments, const SSAValuePtrList &rawArguments, const std::string&) {
        assert(arguments.size() == 2);
        auto isSigned = self->backend->isSignedIntegerType(rawArguments[0]->getValueType());
        return self->coerceI1IntoBoolean8(
            isSigned
                ? builder->CreateICmpSLT(arguments[0], arguments[1])
                : builder->CreateICmpULT(arguments[0], arguments[1])
        );
    }},

    {"integer.add", +[](SSALLVMValueVisitor*, llvm::IRBuilder<> *builder, const std::vector<llvm::Value*> &arguments, const SSAValuePtrList&, const std::string&) {
        assert(arguments.size() == 2);
        return builder->CreateAdd(arguments[0], arguments[0]);
    }},

    {"float.add", +[](SSALLVMValueVisitor*, llvm::IRBuilder<> *builder, const std::vector<llvm::Value*> &arguments, const SSAValuePtrList&, const std::string&) {
        assert(arguments.size() == 2);
        return builder->CreateFAdd(arguments[0], arguments[0]);
    }},
    {"float.mul", +[](SSALLVMValueVisitor*, llvm::IRBuilder<> *builder, const std::vector<llvm::Value*> &arguments, const SSAValuePtrList&, const std::string&) {
        assert(arguments.size() == 2);
        return builder->CreateFMul(arguments[0], arguments[0]);
    }},


    {"reference.load", +[](SSALLVMValueVisitor*, llvm::IRBuilder<> *builder, const std::vector<llvm::Value*> &arguments, const SSAValuePtrList&, const std::string&) {
        assert(arguments.size() == 1);
        return builder->CreateLoad(arguments[0]);
    }},
    {"reference.copy.assignment.trivial", +[](SSALLVMValueVisitor*, llvm::IRBuilder<> *builder, const std::vector<llvm::Value*> &arguments, const SSAValuePtrList&, const std::string&) {
        assert(arguments.size() == 2);
        return builder->CreateStore(arguments[1], arguments[0]);
    }},
};

llvm::Value *SSALLVMValueVisitor::translateIntrinsicCall(const std::string &intrinsicName, const SSACallInstructionPtr &instruction)
{
    auto it = intrinsicGenerators.find(intrinsicName);
    if(it != intrinsicGenerators.end())
    {
        std::vector<llvm::Value*> arguments;
        arguments.reserve(instruction->getArguments().size());
        for(auto &arg : instruction->getArguments())
            arguments.push_back(translateValue(arg));
        return it->second(this, builder.get(), arguments, instruction->getArguments(), intrinsicName);
    }

    return translateCall(instruction);
}

llvm::Value *SSALLVMValueVisitor::translateCall(const SSACallInstructionPtr &instruction)
{
    const auto &calledFunction = instruction->getFunction();
    auto functionType = calledFunction->getValueType();
    assert(functionType->isFunctionalType());
    
    auto translatedFunctionType = backend->translateType(functionType);
    auto translatedCalledFunction = translateValue(calledFunction);
    std::vector<llvm::Value*> arguments;
    arguments.reserve(instruction->getArguments().size());
    for(auto &arg : instruction->getArguments())
        arguments.push_back(translateValue(arg));
    return builder->CreateCall(static_cast<llvm::FunctionType*> (translatedFunctionType), translatedCalledFunction, arguments);
}

AnyValuePtr SSALLVMValueVisitor::visitDoWithCleanupInstruction(const SSADoWithCleanupInstructionPtr &instruction)
{
    auto result = translateCodeRegionWithArguments(instruction->getBodyRegion(), {});
    translateCodeRegionWithArguments(instruction->getCleanUpRegion(), {});
    return wrapLLVMValue(result);
}

AnyValuePtr SSALLVMValueVisitor::visitLocalVariableInstruction(const SSALocalVariableInstructionPtr &instruction)
{
    auto valueType = backend->translateType(instruction->getVariableValueType());
    return wrapLLVMValue(allocaBuilder->CreateAlloca(valueType));
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
    // TODO: Emit the clean-ups here.
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

bool SSALLVMValueVisitor::isLastTerminator() const
{
    return builder->GetInsertBlock()->back().isTerminator();
}
} // End of namespace Environment
} // End of namespace Sysmel