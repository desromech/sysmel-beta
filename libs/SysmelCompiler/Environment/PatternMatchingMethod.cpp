#include "Environment/PatternMatchingMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/AmbiguousMatchingPatternsFound.hpp"
#include "Environment/NotMatchingPatternFound.hpp"

#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include <limits>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<PatternMatchingMethod> patternMatchingMethodTypeRegistration;

PatternMatchingMethod::PatternMatchingMethod(const AnyValuePtr &initialName)
{
    name = initialName;
}

bool PatternMatchingMethod::isPatternMatchingMethod() const
{
    return true;
}

void PatternMatchingMethod::addPattern(const MethodPtr &newPattern)
{
    // TODO: Check for a clash!!
    patterns.push_back(newPattern);
}

AnyValuePtr PatternMatchingMethod::runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver)
{
    std::vector<MethodPtr> matchingCandidates;
    PatternMatchingRank bestRank = std::numeric_limits<PatternMatchingRank>::max();

    for(const auto &pattern : patterns)
    {
        auto result = pattern->matchPatternForRunWithIn(selector, arguments, receiver);
        if(!result.matchingMethod)
            continue;

        if(result.matchingRank < bestRank)
        {
            matchingCandidates.clear();
            matchingCandidates.push_back(result.matchingMethod);
            bestRank = result.matchingRank;
        }
        else if(result.matchingRank == bestRank)
        {
            matchingCandidates.push_back(result.matchingMethod);
        }
    }

    if(matchingCandidates.empty())
    {
        signalNew<NotMatchingPatternFound> ();
    }
    else if(matchingCandidates.size() > 1)
    {
        signalNew<AmbiguousMatchingPatternsFound> ();
    }

    return matchingCandidates.front()->runWithArgumentsIn(selector, arguments, receiver);
}

void PatternMatchingMethod::ensureArgumentsAreAnalyzed(ASTNodePtrList &arguments, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    // Perform the initial analysis of the arguments.
    TypePtrList expectedTypeSet;
    expectedTypeSet.reserve(patterns.size());
    for(size_t i = 0; i < arguments.size(); ++i)
    {
        auto &argument = arguments[i];
        if(argument->analyzedType)
            continue;

        expectedTypeSet.clear();
        bool hasAutoType = false;
        for(const auto &pattern : patterns)
        {
            auto expectedType = pattern->getExpectedTypeForAnalyzingArgumentWithIndex(i);
            if(!expectedType)
            {
                hasAutoType = true;
                break;
            }

            expectedTypeSet.push_back(expectedType);
        }

        if(hasAutoType)
            argument = semanticAnalyzer->analyzeNodeIfNeededWithTemporaryAutoType(argument);
        else
            argument = semanticAnalyzer->analyzeNodeIfNeededWithExpectedTypeSet(argument, expectedTypeSet);
    }
}

ASTNodePtr PatternMatchingMethod::analyzeMessageSendNode(const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    // Analyze the arguments.
    ensureArgumentsAreAnalyzed(node->arguments, semanticAnalyzer);

    std::vector<MethodPtr> matchingCandidates;
    PatternMatchingRank bestRank = std::numeric_limits<PatternMatchingRank>::max();

    for(const auto &pattern : patterns)
    {
        auto result = pattern->matchPatternForAnalyzingMessageSendNode(node, semanticAnalyzer);
        if(!result.matchingMethod)
            continue;

        if(result.matchingRank < bestRank)
        {
            matchingCandidates.clear();
            matchingCandidates.push_back(result.matchingMethod);
            bestRank = result.matchingRank;
        }
        else if(result.matchingRank == bestRank)
        {
            matchingCandidates.push_back(result.matchingMethod);
        }
    }

    if(matchingCandidates.size() != 1)
    {
        // If the receiver supports dynamic compile time message sends,
        // then we might need to solve the actual pattern during runtime.
        if(node->receiver)
        {
            auto receiverType = node->receiver->analyzedType;
            if(receiverType->supportsDynamicCompileTimeMessageSend())
                return semanticAnalyzer->analyzeDynamicCompileTimeMessageSendNode(node);
        }

        if(matchingCandidates.empty())
        {
            return semanticAnalyzer->recordSemanticErrorInNode(node, "Failed to find matching pattern.");
        }
        else if(matchingCandidates.size() > 1)
        {
            return semanticAnalyzer->recordSemanticErrorInNode(node, "Ambiguous matching patterns.");
        }
    }

    return matchingCandidates.front()->analyzeMessageSendNode(node, semanticAnalyzer);
}

AnyValuePtr PatternMatchingMethod::asMethodMatchingSignature(const TypePtr &receiverType, const TypePtrList &argumentTypes, const TypePtr &resultType)
{
    for(const auto &pattern : patterns)
    {
        auto matchedPattern = pattern->asMethodMatchingSignature(receiverType, argumentTypes, resultType);
        if(matchedPattern)
            return matchedPattern;
    }
    
    return nullptr;
}

} // End of namespace Environment
} // End of namespace Sysmel