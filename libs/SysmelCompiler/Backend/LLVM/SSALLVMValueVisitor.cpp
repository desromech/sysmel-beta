#include "Backend/LLVM/SSALLVMValueVisitor.hpp"
#include "Backend/LLVM/LLVMLiteralValueVisitor.hpp"

#include "Environment/Type.hpp"
#include "Environment/FunctionalType.hpp"
#include "Environment/FieldVariable.hpp"
#include "Environment/NameMangler.hpp"
#include "Environment/ASTSourcePosition.hpp"

#include "Environment/SSAConstantLiteralValue.hpp"
#include "Environment/SSAFunction.hpp"
#include "Environment/SSACodeRegion.hpp"
#include "Environment/SSACodeRegionArgument.hpp"
#include "Environment/SSABasicBlock.hpp"
#include "Environment/SSAGlobalVariable.hpp"

#include "Environment/SSACallInstruction.hpp"
#include "Environment/SSAConditionalJumpInstruction.hpp"
#include "Environment/SSAIfInstruction.hpp"
#include "Environment/SSAGetAggregateFieldReferenceInstruction.hpp"
#include "Environment/SSAGetAggregateSlotReferenceInstruction.hpp"
#include "Environment/SSAJumpInstruction.hpp"
#include "Environment/SSALoadInstruction.hpp"
#include "Environment/SSALocalVariableInstruction.hpp"
#include "Environment/SSADoWithCleanupInstruction.hpp"
#include "Environment/SSADoWhileInstruction.hpp"
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

    {"integer.less-than.signed", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateICmpSLT(context.arguments[0], context.arguments[1]));
    }},

    {"integer.less-than.unsigned", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.self->coerceI1IntoBoolean8(context.builder->CreateICmpULT(context.arguments[0], context.arguments[1]));
    }},

    {"integer.add", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateAdd(context.arguments[0], context.arguments[1]);
    }},

    {"float.add", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateFAdd(context.arguments[0], context.arguments[1]);
    }},
    {"float.mul", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateFMul(context.arguments[0], context.arguments[1]);
    }},


    {"pointer.to.reference", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 1);
        return context.arguments[0];
    }},

    {"pointer.element", +[](const IntrinsicGenerationContext &context) {
        sysmelAssert(context.arguments.size() == 2);
        return context.builder->CreateGEP(context.arguments[0], context.arguments[1]);
    }},

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
};

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
    auto name = backend->getNameMangler()->mangleSSAProgramEntity(function);
    
    currentFunction = llvm::Function::Create(functionType, linkage, name, *backend->getTargetModule());
    backend->setGlobalValueTranslation(function, currentFunction);

    // Make the argument used for returning by reference with sret.
    size_t firstArgumentIndex = 0;
    if(mainCodeRegion->isReturningByReference())
    {
        auto structureResultType = backend->translateType(function->getValueType().staticAs<FunctionalType> ()->getResultType());
        currentFunction->getArg(0)->addAttr(llvm::Attribute::getWithStructRetType(*backend->getContext(), structureResultType));
        ++firstArgumentIndex;
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
        abort();

    backend->getFunctionPassManager()->run(*currentFunction);
    
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
    auto name = backend->getNameMangler()->mangleSSAProgramEntity(globalVariable);

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
    if(index == 0 && currentCodeRegion->getArgumentCount() > 0)
    {
        auto firstArgument = currentCodeRegion->getArgument(0);
        if(firstArgument->isLocalFinalizationRequired())
            createLocalFinalizationFlagFor(firstArgument);
    }

    sourceBasicBlock->instructionsDo([&](const SSAInstructionPtr &instruction){
        translateInstruction(instruction);
    });
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
        context.resultType = backend->translateType(instruction->getValueType());
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
    return builder->GetInsertBlock()->back().isTerminator();
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