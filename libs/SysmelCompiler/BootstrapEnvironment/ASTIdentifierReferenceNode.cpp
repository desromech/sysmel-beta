#include "sysmel/BootstrapEnvironment/ASTIdentifierReferenceNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTArgumentDefinitionNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTIdentifierReferenceNode> ASTIdentifierReferenceNodeTypeRegistration;

bool ASTIdentifierReferenceNode::isASTIdentifierReferenceNode() const
{
    return true;
}

AnyValuePtr ASTIdentifierReferenceNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitIdentifierReferenceNode(selfFromThis());
}

SExpression ASTIdentifierReferenceNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"identifier"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        identifier->asSExpression(),
        binding ? binding->asSExpression() : nullptr}};
}

ASTNodePtr ASTIdentifierReferenceNode::parseAsArgumentNodeWith(const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    (void)semanticAnalyzer;
    auto nameNode = basicMakeObject<ASTLiteralValueNode> ();
    nameNode->sourcePosition = sourcePosition;
    nameNode->setValueAndType(identifier);

    auto result = basicMakeObject<ASTArgumentDefinitionNode> ();
    result->sourcePosition = sourcePosition;
    result->identifier = nameNode;

    return result;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius