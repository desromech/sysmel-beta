#include "Environment/ASTPatternNode.hpp"
#include "Environment/ASTBuilder.hpp"
#include "Environment/ASTNeverPatternNode.hpp"
#include "Environment/ASTLocalVariableNode.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/ASTIdentifierReferenceNode.hpp"
#include "Environment/ASTIfNode.hpp"
#include "Environment/ASTSequenceNode.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTFailPatternNode.hpp"
#include "Environment/VariantType.hpp"
#include "Environment/PatternMatchingValueName.hpp"

#include "Environment/Wrappers.hpp"
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

ASTNodePtr ASTPatternNode::optimizePatternNodeForExpectedTypeWith(const TypePtr &, const ASTSemanticAnalyzerPtr &)
{
    return selfFromThis();
}

ASTNeverPatternNodePtr ASTPatternNode::asNeverPatternNode()
{
    auto result = basicMakeObject<ASTNeverPatternNode> ();
    result->sourcePosition = sourcePosition;
    return result;
}

static std::pair<ASTNodePtr, ASTNodePtr> makeHiddenBindingExpansion(const ASTNodePtr &valueNode, const ASTNodePtr &nameNode, const ASTSourcePositionPtr &sourcePosition)
{
    ASTNodePtr validNameNode = nameNode;
    if(!validNameNode)
    {
        auto patternValueName = basicMakeObject<PatternMatchingValueName> ();
        patternValueName->sourcePosition = sourcePosition;
        validNameNode = patternValueName->asASTNodeRequiredInPosition(sourcePosition);
    }

    sysmelAssert(validNameNode->isASTLiteralValueNode());

    auto patternValueVariable = basicMakeObject<ASTLocalVariableNode> ();
    patternValueVariable->sourcePosition = sourcePosition;
    patternValueVariable->name = validNameNode;
    patternValueVariable->initialValue = valueNode;
    patternValueVariable->typeInferenceMode = TypeInferenceMode::TemporaryReference;

    auto patternValueIdentifier = basicMakeObject<ASTIdentifierReferenceNode> ();
    patternValueIdentifier->sourcePosition = sourcePosition;
    patternValueIdentifier->identifier = validNameNode.staticAs<ASTLiteralValueNode> ()->value;;

    return {patternValueVariable, patternValueIdentifier};
}

std::pair<ASTNodePtr, ASTNodePtr> ASTPatternNode::expandHiddenBindingWithExpandedType(const ASTNodePtr &valueNode, const TypePtr &valueType, const TypePtr &expectedType, const ASTSemanticAnalyzerPtr &semanticAnalyzer, const ASTNodePtr &nameNode)
{
    (void)valueType;
    (void)expectedType;
    (void)semanticAnalyzer;

    auto decayedValueType = valueType->asDecayedType();
    auto decayedExpectedType = expectedType->asDecayedType();
    if(decayedValueType != decayedExpectedType && decayedValueType->isVariantType())
    {
        auto variantType = decayedValueType.staticAs<VariantType> ();
        auto expectedTypeSelector = variantType->findTypeSelectorIndexFor(decayedExpectedType);
        if(expectedTypeSelector.has_value())
        {
            auto [directVariable, directIdentifier] = makeHiddenBindingExpansion(valueNode, nullptr, sourcePosition);
            auto builder = ASTBuilder::withSourcePosition(sourcePosition);

            auto expectedTypeSelectorValue = builder->literal(wrapValue(expectedTypeSelector.value()));
            auto typeSelector = builder->sendToWithArguments(builder->literalSymbol("typeSelector"), directIdentifier, {});

            auto variantTypeCheck = builder->ifElse(
                builder->sendToWithArguments(builder->literalSymbol("="), typeSelector, {expectedTypeSelectorValue}),
                builder->failPattern());

            auto extractedValue = builder->sendToWithArguments(builder->literalSymbol("uncheckedGetWithTypeSelector:"), directIdentifier, {expectedTypeSelectorValue});

            auto checkAndExtractSequence = builder->sequence({}, {
                directVariable,
                variantTypeCheck,
                extractedValue
            });

            return makeHiddenBindingExpansion(checkAndExtractSequence, nameNode, sourcePosition);
        }
    }

    return makeHiddenBindingExpansion(valueNode, nameNode, sourcePosition);
}

} // End of namespace Environment
} // End of namespace Sysmel