#include "Environment/ASTNode.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/LiteralString.hpp"
#include "Environment/SubclassResponsibility.hpp"
#include "Environment/MacroInvocationContext.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTNode> ASTNodeTypeRegistration;

// FIXME: Move onto the ASTLiteralNode
AnyValuePtrList ASTNode::__constructors__()
{
    return AnyValuePtrList{
        makeConstructor<ASTNodePtr (TypePtr, LiteralStringPtr)> (+[](const TypePtr &, const LiteralStringPtr &value){
            return value->asASTNodeRequiredInPosition(ASTSourcePosition::empty());
        }, MethodFlags::Pure),        
    };
}

MethodCategories ASTNode::__instanceMethods__()
{
    return MethodCategories{
        {"accessing", {
            makeMethodBinding<ASTSourcePositionPtr (ASTNodePtr)> ("sourcePosition", [](const ASTNodePtr &self) {
                return self->sourcePosition;
            }, MethodFlags::Pure),
        }},
    };
}

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

} // End of namespace Environment
} // End of namespace Sysmel