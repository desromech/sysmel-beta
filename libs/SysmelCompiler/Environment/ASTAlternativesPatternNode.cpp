#include "Environment/ASTAlternativesPatternNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTAlternativesPatternNode> ASTAlternativesPatternNodeTypeRegistration;

bool ASTAlternativesPatternNode::isASTAlternativesPatternNode() const
{
    return true;
}

AnyValuePtr ASTAlternativesPatternNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitAlternativesPatternNode(selfFromThis());
}

SExpression ASTAlternativesPatternNode::asSExpression() const
{
    SExpressionList result;
    result.elements.reserve(2);
    result.elements.push_back(SExpressionIdentifier{{"alternativesPattern"}});
    result.elements.push_back(sourcePosition->asSExpression());
    
    for(auto &alternative : alternatives)
        result.elements.push_back(alternative->asSExpression());

    return result;
}

} // End of namespace Environment
} // End of namespace Sysmel