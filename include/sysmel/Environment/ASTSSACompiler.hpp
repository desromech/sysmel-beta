#ifndef SYSMEL_ENVIRONMENT_AST_SSA_COMPILER_HPP
#define SYSMEL_ENVIRONMENT_AST_SSA_COMPILER_HPP
#pragma once

#include "ASTVisitor.hpp"
#include <unordered_map>

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(SpecificMethod)
SYSMEL_DECLARE_BOOTSTRAP_CLASS(CompiledMethod)
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAFunction)
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSACodeRegion)
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSABuilder)
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(SSAValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateType)

typedef std::function<void ()> ASTSSACodeRegionBuildingBlock;

/**
 * I am a compile time evaluator of a previously analyzed AST.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTSSACompiler : public SubtypeOf<ASTVisitor, ASTSSACompiler>
{
public:
    static constexpr char const __typeName__[] = "ASTSSACompiler";

    SSAValuePtr visitNodeForValue(const ASTNodePtr &node);

    void compileMethodBody(const CompiledMethodPtr &method, const SSAFunctionPtr &ssaFunction, const ASTNodePtr &node);

    virtual AnyValuePtr visitCallNode(const ASTCallNodePtr &node) override;
    virtual AnyValuePtr visitCleanUpScopeNode(const ASTCleanUpScopeNodePtr &node) override;
    virtual AnyValuePtr visitClosureNode(const ASTClosureNodePtr &node) override;
    virtual AnyValuePtr visitLexicalScopeNode(const ASTLexicalScopeNodePtr &node) override;
    virtual AnyValuePtr visitLiteralValueNode(const ASTLiteralValueNodePtr &node) override;
    virtual AnyValuePtr visitMakeTupleNode(const ASTMakeTupleNodePtr &node) override;
    virtual AnyValuePtr visitMakeVectorNode(const ASTMakeVectorNodePtr &node) override;
    virtual AnyValuePtr visitMessageSendNode(const ASTMessageSendNodePtr &node) override;
    virtual AnyValuePtr visitQuoteNode(const ASTQuoteNodePtr &node) override;
    virtual AnyValuePtr visitSequenceNode(const ASTSequenceNodePtr &node) override;
    virtual AnyValuePtr visitVectorSwizzleNode(const ASTVectorSwizzleNodePtr &node) override;

    virtual AnyValuePtr visitQuasiQuotePatternExpansionNode(const ASTQuasiQuotePatternExpansionNodePtr &node) override;
    
    virtual AnyValuePtr visitLocalVariableNode(const ASTLocalVariableNodePtr &node) override;
    virtual AnyValuePtr visitGlobalVariableNode(const ASTGlobalVariableNodePtr &node) override;
    virtual AnyValuePtr visitFieldVariableAccessNode(const ASTFieldVariableAccessNodePtr &node) override;
    virtual AnyValuePtr visitVariableAccessNode(const ASTVariableAccessNodePtr &node) override;
    virtual AnyValuePtr visitLocalImmutableAccessNode(const ASTLocalImmutableAccessNodePtr &node) override;
    virtual AnyValuePtr visitSlotAccessNode(const ASTSlotAccessNodePtr &node) override;

    virtual AnyValuePtr visitProgramEntityNode(const ASTProgramEntityNodePtr &node) override;
    virtual AnyValuePtr visitFunctionalNode(const ASTFunctionalNodePtr &node) override;
    virtual AnyValuePtr visitNamespaceNode(const ASTNamespaceNodePtr &node) override;
    virtual AnyValuePtr visitTypeNode(const ASTTypeNodePtr &node) override;
    virtual AnyValuePtr visitProgramEntityExtensionNode(const ASTProgramEntityExtensionNodePtr &node) override;

    virtual AnyValuePtr visitReinterpretCastNode(const ASTReinterpretCastNodePtr &node) override;
    virtual AnyValuePtr visitValueAsVoidTypeConversionNode(const ASTValueAsVoidTypeConversionNodePtr &node) override;
    virtual AnyValuePtr visitUpcastTypeConversionNode(const ASTUpcastTypeConversionNodePtr &node) override;
    virtual AnyValuePtr visitDowncastTypeConversionNode(const ASTDowncastTypeConversionNodePtr &node) override;
    virtual AnyValuePtr visitValueAsReferenceReinterpretConversionNode(const ASTValueAsReferenceReinterpretConversionNodePtr &node) override;

    virtual AnyValuePtr visitIfNode(const ASTIfNodePtr &node) override;
    virtual AnyValuePtr visitWhileNode(const ASTWhileNodePtr &node) override;
    virtual AnyValuePtr visitDoWhileNode(const ASTDoWhileNodePtr &node) override;
    virtual AnyValuePtr visitReturnNode(const ASTReturnNodePtr &node) override;
    virtual AnyValuePtr visitContinueNode(const ASTContinueNodePtr &node) override;
    virtual AnyValuePtr visitBreakNode(const ASTBreakNodePtr &node) override;

    void buildRegionForSourcePositionWith(const SSACodeRegionPtr &region, const ASTSourcePositionPtr &sourcePosition, const ASTSSACodeRegionBuildingBlock &aBlock);
    void buildRegionForNodeWith(const SSACodeRegionPtr &region, const ASTNodePtr &node, const ASTSSACodeRegionBuildingBlock &aBlock);
    SSACodeRegionPtr buildRegionForNode(const ASTNodePtr &node);
    void assignInitialValueFrom(const SSAValuePtr &destination, const TypePtr &destinationValueType, const SSAValuePtr &initialValue, bool isAggregateComponent = false);
    void addFinalizationFor(const SSAValuePtr &localVariable, const TypePtr &valueType);
    void mapLocalVariableToValue(const AnyValuePtr &binding, const SSAValuePtr &value);
    SSAValuePtr findLocalVariableMapping(const AnyValuePtr &binding);

    void returnValueFromFunction(const SSAValuePtr &result);
    void returnValueFromRegion(const SSAValuePtr &result);
    SSAValuePtr prepareForReturningValueFromRegion(const SSAValuePtr &result, const SSACodeRegionPtr &returningRegion);

    SSAValuePtr makeAggregateWithElements(const AggregateTypePtr &aggregateType, const SSAValuePtrList &elements);

    SpecificMethodPtr currentMethod;
    SSAFunctionPtr currentSSAFunction;
    SSACodeRegionPtr currentCodeRegion;
    SSACodeRegionPtr currentCleanUpCodeRegion;
    SSABuilderPtr builder;
    SSABuilderPtr cleanUpBuilder;

    std::unordered_map<AnyValuePtr, SSAValuePtr> localVariableMap;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_SSA_COMPILER_HPP