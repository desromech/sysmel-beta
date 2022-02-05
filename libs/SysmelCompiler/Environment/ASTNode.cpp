#include "Environment/ASTNode.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/ASTValuePatternNode.hpp"
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

MethodCategories ASTNode::__instanceMethods__()
{
    return MethodCategories{
        {"accessing", {
            makeMethodBinding<ASTSourcePositionPtr (ASTNodePtr)> ("sourcePosition", [](const ASTNodePtr &self) {
                return self->sourcePosition;
            }, MethodFlags::Pure),
        }},

        {"converting", {
            makeMethodBinding<ASTSourcePositionPtr (ASTNodePtr)> ("parseAsPatternNode", [](const ASTNodePtr &self) {
                return self->parseAsPatternNode();
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

ASTNodePtr ASTNode::parseAsPatternNode()
{
    auto result = basicMakeObject<ASTValuePatternNode> ();
    result->sourcePosition = sourcePosition;
    result->expectedValue = selfFromThis();
    return result;
}

ASTNodePtr ASTNode::parseAsBindingPatternNode()
{
    return parseAsPatternNode();
}

ASTNodePtr ASTNode::optimizePatternNodeForExpectedTypeWith(const TypePtr &, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    return semanticAnalyzer->recordSemanticErrorInNode(selfFromThis(), "Not a pattern node that can be optimized.");
}

ASTNodePtr ASTNode::expandPatternNodeForExpectedTypeWith(const TypePtr &, const ASTNodePtr &, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    return semanticAnalyzer->recordSemanticErrorInNode(selfFromThis(), "Not a pattern node that can be expanded.");
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

bool ASTNode::isAlwaysMatchingPattern() const
{
    return false;
}

bool ASTNode::isNeverMatchingPattern() const
{
    return false;
}

bool ASTNode::isValidForCachingTypeConversionRules() const
{
    return true;
}

bool ASTNode::isSuperReference() const
{
    return false;
}

ASTNodePtr ASTNode::parseAsArgumentNodeWith(const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    return semanticAnalyzer->recordSemanticErrorInNode(selfFromThis(), "This is not a valid argument argument specification.");
}

ASTNodePtr ASTNode::parseAsPatternMatchingCaseWith(const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    return semanticAnalyzer->recordSemanticErrorInNode(selfFromThis(), "Not a valid pattern matching case.");
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