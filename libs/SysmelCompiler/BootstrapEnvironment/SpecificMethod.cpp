#include "sysmel/BootstrapEnvironment/SpecificMethod.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTIdentifierReferenceNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticAnalyzer.hpp"
#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/MacroInvocationContext.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <iostream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SpecificMethod> specificMethodTypeRegistration;

bool SpecificMethod::isSpecificMethod() const
{
    return true;
}

MethodPatternMatchingResult SpecificMethod::matchPatternForRunWithIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver)
{
    (void)selector;
    // Make sure the argument count matches.
    if(arguments.size() != signature.argumentTypes.size())
        return MethodPatternMatchingResult{};

    PatternMatchingRank totalRank = 0;

    // TODO: Handle the void receiver type special case.
    if(signature.receiverType->isVoidType())
    {
        // Ignore the receiver that we got.
    }
    else
    {
        totalRank = signature.receiverType->rankToMatchValue(receiver);
        if(totalRank < 0)
            return MethodPatternMatchingResult{};
    }

    for(size_t i = 0; i < arguments.size(); ++i)
    {
        auto argumentRank = signature.argumentTypes[i]->rankToMatchValue(arguments[i]);
        if(argumentRank < 0)
            return MethodPatternMatchingResult{};
        totalRank += argumentRank;
    }

    return MethodPatternMatchingResult{shared_from_this(), totalRank};
}

TypePtr SpecificMethod::getExpectedTypeForAnalyzingArgumentWithIndex(size_t argumentIndex)
{
    return argumentIndex < signature.argumentTypes.size() ? signature.argumentTypes[argumentIndex] : nullptr;
}

MethodPatternMatchingResult SpecificMethod::matchPatternForAnalyzingMessageSendNode(const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    // Make sure the argument count matches.
    if(node->arguments.size() != signature.argumentTypes.size())
        return MethodPatternMatchingResult{};

    PatternMatchingRank totalRank = 0;

    // TODO: Handle the void receiver type special case.
    if(signature.receiverType->isVoidType())
    {
        // Ignore the receiver that we got.
    }
    else
    {
        // A receiver is required in this case.
        if(!node->receiver)
            return MethodPatternMatchingResult{};

        totalRank = semanticAnalyzer->rankForMatchingTypeWithNode(signature.receiverType, node->receiver);
        if(totalRank < 0)
            return MethodPatternMatchingResult{};
    }

    for(size_t i = 0; i < node->arguments.size(); ++i)
    {
        auto expectedArgumentType = signature.argumentTypes[i];
        auto argumentRank = semanticAnalyzer->rankForMatchingTypeWithNode(expectedArgumentType, node->arguments[i]);
        if(argumentRank < 0)
            return MethodPatternMatchingResult{};
        totalRank += argumentRank;
    }

    return MethodPatternMatchingResult{shared_from_this(), totalRank};
}

bool SpecificMethod::isMacroMethod() const
{
    return signature.receiverType && signature.receiverType->isSubtypeOf(MacroInvocationContext::__staticType__());
}

ASTNodePtr SpecificMethod::analyzeMessageSendNode(const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(signature.argumentTypes.size() != node->arguments.size())
        return semanticAnalyzer->recordSemanticErrorInNode(node, "Message argument count mismatch.");

    ASTNodePtr errorNode;

    // Is this a macro method?
    if(isMacroMethod())
    {
        AnyValuePtr macroSelector;
        if(node->selector && node->selector->isASTLiteralSymbolValue())
            macroSelector = std::static_pointer_cast<ASTLiteralValueNode> (node->selector)->value;

        auto macroInvocationContext = semanticAnalyzer->makeMacroInvocationContextFor(node);

        AnyValuePtrList macroArguments;
        macroArguments.reserve(node->arguments.size());

        // Analyze each one of the arguments
        for(size_t i = 0; i < node->arguments.size(); ++i)
        {
            auto expectedType = signature.argumentTypes[i];
            auto argument = node->arguments[i];
            auto adaptedArgument = semanticAnalyzer->adaptNodeAsMacroArgumentOfType(argument, expectedType);
            if(adaptedArgument->isASTErrorNode())
                errorNode = std::static_pointer_cast<ASTErrorNode> (adaptedArgument);

            macroArguments.push_back(adaptedArgument);
        }

        // Propagate error nodes.
        if(errorNode)
            return errorNode;

        // Evaluate the macro macro method.
        auto macroResultNode = semanticAnalyzer->guardCompileTimeEvaluationForNode(node, [&]() {
            auto macroDirectResult = runWithArgumentsIn(macroSelector, macroArguments, macroInvocationContext);
            return validAnyValue(macroDirectResult)->asASTNodeRequiredInPosition(node->sourcePosition);
        });

        // Analyze the macro expanded node.
        return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(macroResultNode);
    }

    // Analyze the receiver.
    if(node->receiver)
    {
        node->receiver = semanticAnalyzer->analyzeNodeIfNeededWithExpectedType(node->receiver, signature.receiverType);
        if(node->receiver->isASTErrorNode())
            errorNode = node->receiver;
    }
        

    // Analyze each one of the arguments
    for(size_t i = 0; i < node->arguments.size(); ++i)
    {
        auto expectedType = signature.argumentTypes[i];
        auto &argument = node->arguments[i];
        argument = semanticAnalyzer->analyzeNodeIfNeededWithExpectedType(argument, expectedType);
        if(argument->isASTErrorNode())
            errorNode = argument;
    }

    // Propagate error nodes.
    if(errorNode)
        return errorNode;

    node->analyzedBoundMessage = shared_from_this();
    node->analyzedType = signature.resultType;
    return semanticAnalyzer->optimizeAnalyzedMessageSend(node);
}

ASTNodePtr SpecificMethod::analyzeIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(isMacroMethod() && signature.argumentTypes.empty())
    {
        auto macroInvocationContext = semanticAnalyzer->makeMacroInvocationContextFor(node);

        // Evaluate the macro macro method.
        auto macroResultNode = semanticAnalyzer->guardCompileTimeEvaluationForNode(node, [&]() {
            auto macroDirectResult = runWithArgumentsIn(node->identifier, {}, macroInvocationContext);
            return validAnyValue(macroDirectResult)->asASTNodeRequiredInPosition(node->sourcePosition);
        });

        // Analyze the macro expanded node.
        return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(macroResultNode);
    }

    return SuperType::analyzeIdentifierReferenceNode(node, semanticAnalyzer);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius