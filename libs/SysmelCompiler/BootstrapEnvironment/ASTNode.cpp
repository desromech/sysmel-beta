#include "sysmel/BootstrapEnvironment/ASTNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticAnalyzer.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/SubclassResponsibility.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTNode> ASTNodeTypeRegistration;

ASTNode::ASTNode()
{
    sourcePosition = ASTSourcePosition::empty();
}

AnyValuePtr ASTNode::accept(const ASTVisitorPtr &)
{
    SysmelSelfSubclassResponsibility();
}

bool ASTNode::isASTNode() const
{
    return true;
}

bool ASTNode::isASTLiteralSymbolValue() const
{
    return false;
}

bool ASTNode::isPureCompileTimeLiteralValueNode() const
{
    return false;
}

bool ASTNode::isPureCompileTimeEvaluableNode() const
{
    return false;
}

ASTNodePtr ASTNode::asASTNodeRequiredInPosition(const ASTSourcePositionPtr &requiredSourcePosition)
{
    (void)requiredSourcePosition;
    return selfFromThis();
}

ASTNodePtr ASTNode::asInlinedBlockBodyNode()
{
    return selfFromThis();
}

bool ASTNode::isASTIdentifierSymbolValue() const
{
    return false;
}

bool ASTNode::isASTLiteralTypeNode() const
{
    return false;
}

ASTNodePtr ASTNode::parseAsArgumentNodeWith(const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    return semanticAnalyzer->recordSemanticErrorInNode(selfFromThis(), "This is not a valid argument argument specification.");
}

std::string ASTNode::printString() const
{
    return sexpressionToPrettyString(asSExpression());
}

ASTNodePtrList ASTNode::children()
{
    ASTNodePtrList result;
    childrenDo([&](const ASTNodePtr &child) {
        if(child)
            result.push_back(child);
    });

    return result;
}

void ASTNode::childrenDo(const ASTIterationBlock &aBlock)
{
    (void)aBlock;
}

void ASTNode::allChildrenDo(const ASTIterationBlock &aBlock)
{
    ASTIterationBlock recursiveBlock = [&](const ASTNodePtr &child) {
        aBlock(child);
        child->childrenDo(recursiveBlock);
    };

    childrenDo(recursiveBlock);
}

void ASTNode::withAllChildrenDo(const ASTIterationBlock &aBlock)
{
    aBlock(selfFromThis());
    allChildrenDo(aBlock);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius