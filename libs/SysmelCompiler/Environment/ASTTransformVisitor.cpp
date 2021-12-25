#include "Environment/ASTTransformVisitor.hpp"

#include "Environment/ASTArgumentDefinitionNode.hpp"
#include "Environment/ASTCleanUpScopeNode.hpp"
#include "Environment/ASTClosureNode.hpp"
#include "Environment/ASTCompileTimeEvaluationErrorNode.hpp"
#include "Environment/ASTErrorNode.hpp"
#include "Environment/ASTIdentifierReferenceNode.hpp"
#include "Environment/ASTCallNode.hpp"
#include "Environment/ASTLexicalScopeNode.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/ASTMakeAssociationNode.hpp"
#include "Environment/ASTMakeDictionaryNode.hpp"
#include "Environment/ASTMakeLiteralArrayNode.hpp"
#include "Environment/ASTMakeTupleNode.hpp"
#include "Environment/ASTMessageChainNode.hpp"
#include "Environment/ASTMessageChainMessageNode.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTParseErrorNode.hpp"
#include "Environment/ASTPragmaNode.hpp"
#include "Environment/ASTQuasiQuoteNode.hpp"
#include "Environment/ASTQuasiUnquoteNode.hpp"
#include "Environment/ASTQuoteNode.hpp"
#include "Environment/ASTSequenceNode.hpp"
#include "Environment/ASTSemanticErrorNode.hpp"
#include "Environment/ASTSpliceNode.hpp"

#include "Environment/ASTQuasiQuotePatternExpansionNode.hpp"
#include "Environment/ASTQuasiQuotePatternExpansionArgumentNode.hpp"

#include "Environment/ASTProgramEntityNode.hpp"
#include "Environment/ASTVariableNode.hpp"
#include "Environment/ASTLocalVariableNode.hpp"
#include "Environment/ASTGlobalVariableNode.hpp"
#include "Environment/ASTFieldVariableNode.hpp"
#include "Environment/ASTCompileTimeConstantNode.hpp"

#include "Environment/ASTFieldVariableAccessNode.hpp"
#include "Environment/ASTVariableAccessNode.hpp"
#include "Environment/ASTLocalImmutableAccessNode.hpp"

#include "Environment/ASTFunctionalNode.hpp"
#include "Environment/ASTFunctionNode.hpp"
#include "Environment/ASTMethodNode.hpp"

#include "Environment/ASTNamespaceNode.hpp"

#include "Environment/ASTTypeNode.hpp"
#include "Environment/ASTEnumNode.hpp"
#include "Environment/ASTStructNode.hpp"
#include "Environment/ASTUnionNode.hpp"
#include "Environment/ASTClassNode.hpp"
#include "Environment/ASTTemplateNode.hpp"
#include "Environment/ASTProgramEntityExtensionNode.hpp"

#include "Environment/ASTTypeConversionNode.hpp"
#include "Environment/ASTValueAsVoidTypeConversionNode.hpp"
#include "Environment/ASTUpcastTypeConversionNode.hpp"
#include "Environment/ASTDowncastTypeConversionNode.hpp"
#include "Environment/ASTValueAsReferenceReinterpretConversionNode.hpp"

#include "Environment/ASTIfNode.hpp"
#include "Environment/ASTWhileNode.hpp"
#include "Environment/ASTDoWhileNode.hpp"
#include "Environment/ASTReturnNode.hpp"
#include "Environment/ASTContinueNode.hpp"
#include "Environment/ASTBreakNode.hpp"

#include "Environment/ASTCastNode.hpp"
#include "Environment/ASTExplicitCastNode.hpp"
#include "Environment/ASTImplicitCastNode.hpp"
#include "Environment/ASTReinterpretCastNode.hpp"

#include "Environment/SubclassResponsibility.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTTransformVisitor> ASTTransformVisitorTypeRegistration;

void ASTTransformVisitor::transformChildNode(ASTNodePtr& childNode)
{
    if(childNode)
    {
        auto transformedNode = visitNode(childNode);
        sysmelAssert(transformedNode->isASTNode());
        childNode = staticObjectCast<ASTNode> (transformedNode);
    }
}

void ASTTransformVisitor::transformChildNodeList(ASTNodePtrList& childNodeList)
{
    for(auto &childNode : childNodeList)
        transformChildNode(childNode);
}

AnyValuePtr ASTTransformVisitor::visitArgumentDefinitionNode(const ASTArgumentDefinitionNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->identifier);
    transformChildNode(result->type);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitCleanUpScopeNode(const ASTCleanUpScopeNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->body);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitClosureNode(const ASTClosureNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNodeList(result->arguments);
    transformChildNode(result->resultType);
    transformChildNode(result->body);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitCompileTimeEvaluationErrorNode(const ASTCompileTimeEvaluationErrorNodePtr &node)
{
    return visitErrorNode(node);
}

AnyValuePtr ASTTransformVisitor::visitErrorNode(const ASTErrorNodePtr &node)
{
    return node;
}

AnyValuePtr ASTTransformVisitor::visitIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &node)
{
    return node;
}

AnyValuePtr ASTTransformVisitor::visitCallNode(const ASTCallNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->function);
    transformChildNodeList(result->arguments);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitLexicalScopeNode(const ASTLexicalScopeNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->body);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitLiteralValueNode(const ASTLiteralValueNodePtr &node)
{
    return node;
}

AnyValuePtr ASTTransformVisitor::visitMakeAssociationNode(const ASTMakeAssociationNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->key);
    transformChildNode(result->value);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitMakeDictionaryNode(const ASTMakeDictionaryNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNodeList(result->elements);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitMakeLiteralArrayNode(const ASTMakeLiteralArrayNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNodeList(result->elements);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitMakeTupleNode(const ASTMakeTupleNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNodeList(result->elements);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitMessageChainNode(const ASTMessageChainNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->receiver);
    transformChildNodeList(result->messages);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitMessageChainMessageNode(const ASTMessageChainMessageNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->selector);
    transformChildNodeList(result->arguments);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitMessageSendNode(const ASTMessageSendNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->receiver);
    transformChildNode(result->selector);
    transformChildNodeList(result->arguments);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitParseErrorNode(const ASTParseErrorNodePtr &node)
{
    return visitErrorNode(node);
}

AnyValuePtr ASTTransformVisitor::visitPragmaNode(const ASTPragmaNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->selector);
    transformChildNodeList(result->arguments);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitQuasiQuoteNode(const ASTQuasiQuoteNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->expression);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitQuasiUnquoteNode(const ASTQuasiUnquoteNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->expression);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitQuoteNode(const ASTQuoteNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->expression);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitSemanticErrorNode(const ASTSemanticErrorNodePtr &node)
{
    return visitErrorNode(node);
}

AnyValuePtr ASTTransformVisitor::visitSequenceNode(const ASTSequenceNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNodeList(result->pragmas);
    transformChildNodeList(result->expressions);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitSpliceNode(const ASTSpliceNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->expression);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitQuasiQuotePatternExpansionNode(const ASTQuasiQuotePatternExpansionNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->pattern);
    transformChildNodeList(result->arguments);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitQuasiQuotePatternExpansionArgumentNode(const ASTQuasiQuotePatternExpansionArgumentNodePtr &node)
{
    return node;
}

AnyValuePtr ASTTransformVisitor::visitProgramEntityNode(const ASTProgramEntityNodePtr &node)
{
    return node;
}

AnyValuePtr ASTTransformVisitor::visitVariableNode(const ASTVariableNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->name);
    transformChildNode(result->type);
    transformChildNode(result->alignment);
    transformChildNode(result->initialValue);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitLocalVariableNode(const ASTLocalVariableNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->name);
    transformChildNode(result->type);
    transformChildNode(result->alignment);
    transformChildNode(result->initialValue);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitGlobalVariableNode(const ASTGlobalVariableNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->name);
    transformChildNode(result->type);
    transformChildNode(result->alignment);
    transformChildNode(result->initialValue);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitFieldVariableNode(const ASTFieldVariableNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->name);
    transformChildNode(result->type);
    transformChildNode(result->alignment);
    transformChildNode(result->initialValue);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitCompileTimeConstantNode(const ASTCompileTimeConstantNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->name);
    transformChildNode(result->type);
    transformChildNode(result->value);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitFieldVariableAccessNode(const ASTFieldVariableAccessNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->aggregate);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitVariableAccessNode(const ASTVariableAccessNodePtr &node)
{
    return node;
}

AnyValuePtr ASTTransformVisitor::visitLocalImmutableAccessNode(const ASTLocalImmutableAccessNodePtr &node)
{
    return node;
}

AnyValuePtr ASTTransformVisitor::visitFunctionalNode(const ASTFunctionalNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->name);
    transformChildNodeList(result->arguments);
    transformChildNode(result->resultType);
    transformChildNode(result->body);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitFunctionNode(const ASTFunctionNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->name);
    transformChildNodeList(result->arguments);
    transformChildNode(result->resultType);
    transformChildNode(result->body);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitMethodNode(const ASTMethodNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->name);
    transformChildNodeList(result->arguments);
    transformChildNode(result->resultType);
    transformChildNode(result->body);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitNamespaceNode(const ASTNamespaceNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->name);
    transformChildNode(result->body);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitTypeNode(const ASTTypeNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->name);
    transformChildNode(result->body);
    transformChildNode(result->alignment);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitEnumNode(const ASTEnumNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->name);
    transformChildNode(result->body);
    transformChildNode(result->alignment);
    transformChildNode(result->valueType);
    transformChildNode(result->values);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitStructNode(const ASTStructNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->name);
    transformChildNode(result->body);
    transformChildNode(result->alignment);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitUnionNode(const ASTUnionNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->name);
    transformChildNode(result->body);
    transformChildNode(result->alignment);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitClassNode(const ASTClassNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->name);
    transformChildNode(result->superclass);
    transformChildNode(result->body);
    transformChildNode(result->alignment);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitTemplateNode(const ASTTemplateNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->name);
    transformChildNodeList(result->arguments);
    transformChildNode(result->body);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitProgramEntityExtensionNode(const ASTProgramEntityExtensionNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->programEntity);
    transformChildNode(result->body);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitCastNode(const ASTCastNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->expression);
    transformChildNode(result->targetType);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitExplicitCastNode(const ASTExplicitCastNodePtr &node)
{
    return visitCastNode(node);
}

AnyValuePtr ASTTransformVisitor::visitImplicitCastNode(const ASTImplicitCastNodePtr &node)
{
    return visitCastNode(node);
}

AnyValuePtr ASTTransformVisitor::visitReinterpretCastNode(const ASTReinterpretCastNodePtr &node)
{
    return visitCastNode(node);
}

AnyValuePtr ASTTransformVisitor::visitTypeConversionNode(const ASTTypeConversionNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->expression);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitValueAsVoidTypeConversionNode(const ASTValueAsVoidTypeConversionNodePtr &node)
{
    return visitTypeConversionNode(node);
}

AnyValuePtr ASTTransformVisitor::visitUpcastTypeConversionNode(const ASTUpcastTypeConversionNodePtr &node)
{
    return visitTypeConversionNode(node);
}

AnyValuePtr ASTTransformVisitor::visitDowncastTypeConversionNode(const ASTDowncastTypeConversionNodePtr &node)
{
    return visitTypeConversionNode(node);
}

AnyValuePtr ASTTransformVisitor::visitValueAsReferenceReinterpretConversionNode(const ASTValueAsReferenceReinterpretConversionNodePtr &node)
{
    return visitTypeConversionNode(node);
}

AnyValuePtr ASTTransformVisitor::visitIfNode(const ASTIfNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->condition);
    transformChildNode(result->trueExpression);
    transformChildNode(result->falseExpression);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitWhileNode(const ASTWhileNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->condition);
    transformChildNode(result->bodyExpression);
    transformChildNode(result->continueExpression);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitDoWhileNode(const ASTDoWhileNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->bodyExpression);
    transformChildNode(result->condition);
    transformChildNode(result->continueExpression);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitReturnNode(const ASTReturnNodePtr &node)
{
    auto result = shallowCloneObject(node);
    transformChildNode(result->expression);
    return result;
}

AnyValuePtr ASTTransformVisitor::visitContinueNode(const ASTContinueNodePtr &node)
{
    return node;
}

AnyValuePtr ASTTransformVisitor::visitBreakNode(const ASTBreakNodePtr &node)
{
    return node;
}

} // End of namespace Environment
} // End of namespace Sysmel