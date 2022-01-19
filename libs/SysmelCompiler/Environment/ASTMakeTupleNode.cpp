#include "Environment/ASTMakeTupleNode.hpp"
#include "Environment/ASTSequencePatternNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTMakeTupleNode> ASTMakeTupleNodeTypeRegistration;

bool ASTMakeTupleNode::isASTMakeTupleNode() const
{
    return true;
}

AnyValuePtr ASTMakeTupleNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitMakeTupleNode(selfFromThis());
}

SExpression ASTMakeTupleNode::asSExpression() const
{
    SExpressionList sexpr;
    sexpr.elements.reserve(3 + elements.size());
    sexpr.elements.push_back(SExpressionIdentifier{{"tuple"}});
    sexpr.elements.push_back(sourcePosition->asSExpression());
    sexpr.elements.push_back(analyzedType ? analyzedType->asSExpression() : nullptr);
    for(const auto &element : elements)
        sexpr.elements.push_back(element->asSExpression());

    return sexpr;
}

void ASTMakeTupleNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    for(auto &el : elements)
        aBlock(el);
}

ASTNodePtr ASTMakeTupleNode::parseAsPatternNode()
{
    auto sequencePattern = basicMakeObject<ASTSequencePatternNode> ();
    sequencePattern->sourcePosition = sourcePosition;
    sequencePattern->elements.reserve(elements.size());
    for(auto &el : elements)
        sequencePattern->elements.push_back(el->parseAsPatternNode());
    
    return sequencePattern;
}

} // End of namespace Environment
} // End of namespace Sysmel