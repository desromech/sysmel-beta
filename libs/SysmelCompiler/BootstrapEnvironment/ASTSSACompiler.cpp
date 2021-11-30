#include "sysmel/BootstrapEnvironment/ASTSSACompiler.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

#include "sysmel/BootstrapEnvironment/ASTNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTCallNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSequenceNode.hpp"

#include "sysmel/BootstrapEnvironment/ASTVariableAccessNode.hpp"

#include "sysmel/BootstrapEnvironment/ASTValueAsVoidTypeConversionNode.hpp"

#include "sysmel/BootstrapEnvironment/CompiledMethod.hpp"
#include "sysmel/BootstrapEnvironment/SSAFunction.hpp"
#include "sysmel/BootstrapEnvironment/SSACodeRegion.hpp"
#include "sysmel/BootstrapEnvironment/SSACodeRegionArgument.hpp"
#include "sysmel/BootstrapEnvironment/SSABasicBlock.hpp"
#include "sysmel/BootstrapEnvironment/SSABuilder.hpp"

#include "sysmel/BootstrapEnvironment/SSAConstantLiteralValue.hpp"

#include "sysmel/BootstrapEnvironment/SSACallInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSASendMessageInstruction.hpp"

#include "sysmel/BootstrapEnvironment/FunctionalType.hpp"
#include "sysmel/BootstrapEnvironment/ArgumentVariable.hpp"
#include "sysmel/BootstrapEnvironment/LocalVariable.hpp"

#include "sysmel/BootstrapEnvironment/ControlFlowUtilities.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTSSACompiler> ASTSSACompilerTypeRegistration;

SSAValuePtr ASTSSACompiler::visitNodeForValue(const ASTNodePtr &node)
{
    auto oldPosition = builder->getSourcePosition();
    builder->setSourcePosition(node->sourcePosition);
    auto value = doWithEnsure([&](){
        return visitNode(node);
    }, [&]{
        builder->setSourcePosition(oldPosition);
    });

    assert(value->isSSAValue());
    return staticObjectCast<SSAValue> (value);
}

void ASTSSACompiler::compileMethodBody(const CompiledMethodPtr &method, const SSAFunctionPtr &ssaFunction, const ASTNodePtr &node)
{
    currentMethod = method;
    currentSSAFunction = ssaFunction;
    currentCodeRegion = ssaFunction->getMainCodeRegion();

    builder = basicMakeObject<SSABuilder> ();
    builder->setSourcePosition(node->sourcePosition);
    builder->setCodeRegion(currentCodeRegion);
    builder->makeBasicBlockHere();

    size_t argumentsOffset = 0;
    auto functionalType = method->getFunctionalType();
    auto receiverType = functionalType->getReceiverType();
    if(!receiverType->isVoidType())
    {
        argumentsOffset = 1;
    }

    const auto &methodArguments = method->getArguments();
    const auto &codeRegionArguments = currentCodeRegion->getArguments();
    for(size_t i = 0; i < methodArguments.size(); ++i)
        localVariableMap.insert({methodArguments[i], codeRegionArguments[argumentsOffset + i]});
    
    auto resultValue = visitNodeForValue(node);
    if(!builder->isLastTerminator())
        builder->returnFromFunction(resultValue);
}

AnyValuePtr ASTSSACompiler::visitLiteralValueNode(const ASTLiteralValueNodePtr &node)
{
    return node->value->asSSAValueRequiredInPosition(builder->getSourcePosition());
}

AnyValuePtr ASTSSACompiler::visitCallNode(const ASTCallNodePtr &node)
{
    auto functionValue = visitNodeForValue(node->function);
    SSAValuePtrList arguments;
    arguments.reserve(node->arguments.size());
    for(auto &arg : node->arguments)
        arguments.push_back(visitNodeForValue(arg));

    return builder->call(node->analyzedType, functionValue, arguments);
}

AnyValuePtr ASTSSACompiler::visitMessageSendNode(const ASTMessageSendNodePtr &node)
{
    SSAValuePtr receiverValue;
    if(node->receiver)
        receiverValue = visitNodeForValue(node->receiver);

    SSAValuePtr selectorValue;
    if(node->selector)
        selectorValue = visitNodeForValue(node->selector);

    SSAValuePtrList arguments;
    arguments.reserve(node->arguments.size());
    for(auto &arg : node->arguments)
        arguments.push_back(visitNodeForValue(arg));

    // Replace the direct message sends by call.
    assert(node->receiver || node->analyzedBoundMessageIsDirect);
    if(node->analyzedBoundMessageIsDirect)
    {
        if(node->receiver && !node->receiver->analyzedType->isVoidType())
            arguments.insert(arguments.begin(), receiverValue);

        return builder->call(node->analyzedType, node->analyzedBoundMessage->asSSAValueRequiredInPosition(node->sourcePosition), arguments);
    }

    return builder->sendMessage(node->analyzedType, selectorValue, receiverValue, arguments);
}

AnyValuePtr ASTSSACompiler::visitSequenceNode(const ASTSequenceNodePtr &node)
{
    SSAValuePtr result;
    for(auto &expression : node->expressions)
    {
        result = visitNodeForValue(expression);
        if(builder->isLastTerminator())
            break;
    }

    if(!result)
        result = builder->literal(getVoidConstant());
    return result;
}

AnyValuePtr ASTSSACompiler::visitVariableAccessNode(const ASTVariableAccessNodePtr &node)
{
    auto it = localVariableMap.find(node->variable);
    auto variableValue = it != localVariableMap.end() ? it->second : node->variable->asSSAValueRequiredInPosition(node->sourcePosition);
    return variableValue;
}

AnyValuePtr ASTSSACompiler::visitValueAsVoidTypeConversionNode(const ASTValueAsVoidTypeConversionNodePtr &node)
{
    visitNodeForValue(node->expression);
    return builder->literal(getVoidConstant());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius