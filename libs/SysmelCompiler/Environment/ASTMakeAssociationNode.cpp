#include "Environment/ASTMakeAssociationNode.hpp"
#include "Environment/ASTPatternMatchingCaseNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTMakeAssociationNode> ASTMakeAssociationNodeTypeRegistration;

bool ASTMakeAssociationNode::isASTMakeAssociationNode() const
{
    return true;
}

AnyValuePtr ASTMakeAssociationNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitMakeAssociationNode(selfFromThis());
}

SExpression ASTMakeAssociationNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"association"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        key ? key->asSExpression() : nullptr,
        value ? value->asSExpression() : nullptr,
    }};
}

void ASTMakeAssociationNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(key) aBlock(key);
    if(value) aBlock(value);
}

ASTNodePtr ASTMakeAssociationNode::parseAsPatternMatchingCaseWith(const ASTSemanticAnalyzerPtr &)
{
    auto result = basicMakeObject<ASTPatternMatchingCaseNode> ();
    result->sourcePosition = sourcePosition;
    result->patternNode = key ? key->parseAsPatternNode() : nullptr;
    result->actionNode = value;
    return result;
}

} // End of namespace Environment
} // End of namespace Sysmel