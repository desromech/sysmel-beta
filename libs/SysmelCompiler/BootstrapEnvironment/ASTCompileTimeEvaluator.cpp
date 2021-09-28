#include "sysmel/BootstrapEnvironment/ASTCompileTimeEvaluator.hpp"
#include "sysmel/BootstrapEnvironment/ASTArgumentDefinitionNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTCleanUpScopeNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTClosureNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTIntrinsicOperationNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTLexicalScopeNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMakeAssociationNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMakeDictionaryNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMakeLiteralArrayNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMakeTupleNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageChainMessageNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageChainNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTPragmaNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTQuasiQuoteNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTQuasiUnquoteNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTQuoteNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTParseErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSequenceNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSpliceNode.hpp"

#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTCompileTimeEvaluator> ASTCompileTimeEvaluatorTypeRegistration;

AnyValuePtr ASTCompileTimeEvaluator::visitCleanUpScopeNode(const ASTCleanUpScopeNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTCompileTimeEvaluator::visitClosureNode(const ASTClosureNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTCompileTimeEvaluator::visitIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTCompileTimeEvaluator::visitIntrinsicOperationNode(const ASTIntrinsicOperationNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTCompileTimeEvaluator::visitLexicalScopeNode(const ASTLexicalScopeNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTCompileTimeEvaluator::visitLiteralValueNode(const ASTLiteralValueNodePtr &node)
{
    return node->value;
}

AnyValuePtr ASTCompileTimeEvaluator::visitMakeAssociationNode(const ASTMakeAssociationNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTCompileTimeEvaluator::visitMakeDictionaryNode(const ASTMakeDictionaryNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTCompileTimeEvaluator::visitMakeTupleNode(const ASTMakeTupleNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTCompileTimeEvaluator::visitMessageSendNode(const ASTMessageSendNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTCompileTimeEvaluator::visitQuasiQuoteNode(const ASTQuasiQuoteNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTCompileTimeEvaluator::visitQuoteNode(const ASTQuoteNodePtr &node)
{
    return node->expression;
}

AnyValuePtr ASTCompileTimeEvaluator::visitSequenceNode(const ASTSequenceNodePtr &node)
{
    auto result = getVoidConstant();
    for(const auto &expression : node->expressions)
        result = visitNode(expression);
    return result;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius