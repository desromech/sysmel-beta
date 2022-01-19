#include "Environment/ASTSequencePatternNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTSequencePatternNode> ASTSequencePatternNodeTypeRegistration;

bool ASTSequencePatternNode::isASTSequencePatternNode() const
{
    return true;
}

AnyValuePtr ASTSequencePatternNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitSequencePatternNode(selfFromThis());
}

SExpression ASTSequencePatternNode::asSExpression() const
{
    SExpressionList result;
    result.elements.reserve(3);
    result.elements.push_back(SExpressionIdentifier{{"sequencePattern"}});
    result.elements.push_back(sourcePosition->asSExpression());
    result.elements.push_back(expectedSequenceType ? expectedSequenceType->asSExpression() : nullptr);
    
    for(auto &el : elements)
        result.elements.push_back(el->asSExpression());

    return result;
}

} // End of namespace Environment
} // End of namespace Sysmel