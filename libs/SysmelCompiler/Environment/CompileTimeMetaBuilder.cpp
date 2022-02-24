#include "Environment/CompileTimeMetaBuilder.hpp"
#include "Environment/CompileTimeConstantMetaBuilder.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<CompileTimeMetaBuilder> visibilityMetaBuilderTypeRegistration;

void CompileTimeMetaBuilder::setMetaBuilderInstanceContext(const MetaBuilderInstanceContextPtr &context)
{
    SuperType::setMetaBuilderInstanceContext(context);
    context->isCompileTime = true;
}

ASTNodePtr CompileTimeMetaBuilder::analyzeMessageSendNodeWithSelector(const std::string &selector, const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(node->arguments.empty())
    {
        // Functional
        if(selector == "constant")
            return delegateToMetaBuilderAt<CompileTimeConstantMetaBuilder> (node->sourcePosition);
    }

    // If
    if((selector == "if:then:" && node->arguments.size() == 2) ||
        (selector == "if:then:else:" && node->arguments.size() == 3))
    {
        auto conditionNode = semanticAnalyzer->analyzeNodeIfNeededWithBooleanExpectedType(node->arguments[0]);
        if(conditionNode->isASTErrorNode())
            return conditionNode;

        auto conditionValue = validAnyValue(semanticAnalyzer->evaluateInCompileTime(conditionNode))->unwrapAsBoolean();
        if(conditionValue)
        {
            return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(node->arguments[1]);
        }
        else
        {
            ASTNodePtr elseNode;
            if(node->arguments.size() == 3)
                elseNode = node->arguments[2];
            else
                elseNode = getVoidConstant()->asASTNodeRequiredInPosition(node->sourcePosition);
            return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(elseNode);
        }
    }

    return SuperType::analyzeMessageSendNodeWithSelector(selector, node, semanticAnalyzer);
}


} // End of namespace Environment
} // End of namespace Sysmel