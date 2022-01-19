#include "Environment/ASTIdentifierReferenceNode.hpp"
#include "Environment/ASTArgumentDefinitionNode.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/ASTAnyValuePatternNode.hpp"
#include "Environment/ASTBindingPatternNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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
        analyzedType ? analyzedType->asSExpression() : nullptr,
        identifier->asSExpression(),
        binding ? binding->asSExpression() : nullptr,
        sourcePosition->asSExpression(),}};
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

ASTNodePtr ASTIdentifierReferenceNode::parseAsPatternNode()
{
    if(validAnyValue(identifier)->isAnonymousNameSymbol())
    {
        auto pattern = basicMakeObject<ASTAnyValuePatternNode> ();
        pattern->sourcePosition = sourcePosition;
        return pattern;
    }
    
    return SuperType::parseAsPatternNode();
}

ASTNodePtr ASTIdentifierReferenceNode::parseAsBindingPatternNode()
{
    auto bindingPattern = basicMakeObject<ASTBindingPatternNode> ();
    bindingPattern->sourcePosition = sourcePosition;
    bindingPattern->identifier = identifier;
    return bindingPattern;
}

} // End of namespace Environment
} // End of namespace Sysmel