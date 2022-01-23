#include "Environment/ASTPatternNode.hpp"
#include "Environment/ASTNeverPatternNode.hpp"
#include "Environment/ASTLocalVariableNode.hpp"
#include "Environment/ASTIdentifierReferenceNode.hpp"
#include "Environment/PatternMatchingValueName.hpp"

#include "Environment/PatternMatchingValueName.hpp"

#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTPatternNode> ASTPatternNodeTypeRegistration;

bool ASTPatternNode::isASTPatternNode() const
{
    return true;
}

ASTNodePtr ASTPatternNode::parseAsPatternNode()
{
    return selfFromThis();
}

ASTNodePtr ASTPatternNode::optimizePatternNodeForExpectedTypeWith(const TypePtr &type, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    return selfFromThis();
}

ASTNeverPatternNodePtr ASTPatternNode::asNeverPatternNode()
{
    auto result = basicMakeObject<ASTNeverPatternNode> ();
    result->sourcePosition = sourcePosition;
    return result;
}

std::pair<ASTNodePtr, ASTNodePtr> ASTPatternNode::expandHiddenBindingWithExpandedType(const ASTNodePtr &valueNode, const TypePtr &valueType, const TypePtr &expectedType, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    (void)valueType;
    (void)expectedType;
    auto patternValueName = basicMakeObject<PatternMatchingValueName> ();
    patternValueName->sourcePosition = sourcePosition;

    auto patternValueVariable = basicMakeObject<ASTLocalVariableNode> ();
    patternValueVariable->sourcePosition = sourcePosition;
    patternValueVariable->name = patternValueName->asASTNodeRequiredInPosition(sourcePosition);
    patternValueVariable->initialValue = valueNode;
    patternValueVariable->typeInferenceMode = TypeInferenceMode::TemporaryReference;

    auto patternValueIdentifier = basicMakeObject<ASTIdentifierReferenceNode> ();
    patternValueIdentifier->sourcePosition = sourcePosition;
    patternValueIdentifier->identifier = patternValueName;

    return {patternValueVariable, patternValueIdentifier};
}

} // End of namespace Environment
} // End of namespace Sysmel