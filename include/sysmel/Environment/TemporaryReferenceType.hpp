#ifndef SYSMEL_ENVIRONMENT_TEMPORARY_REFERENCE_TYPE_HPP
#define SYSMEL_ENVIRONMENT_TEMPORARY_REFERENCE_TYPE_HPP
#pragma once

#include "PointerLikeType.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(TemporaryReferenceType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(TemporaryReferenceTypeValue);

/**
 * I am an instance of a function type object.
 */
class SYSMEL_COMPILER_LIB_EXPORT TemporaryReferenceType : public SubMetaTypeOf<PointerLikeType, TemporaryReferenceType>
{
public:
    static TemporaryReferenceTypePtr make(const TypePtr &baseType);
    static TemporaryReferenceTypePtr makeWithAddressSpace(const TypePtr &baseType, const AnyValuePtr &addressSpace);

    virtual bool isTemporaryReferenceType() const override;
    virtual bool isReferenceLikeType() const override;
    virtual AnyValuePtr acceptTypeVisitor(const TypeVisitorPtr &visitor) override;
    virtual AnyValuePtr lookupLocalSymbolFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope) override;

    virtual ReferenceTypePtr ref() override;
    virtual ReferenceTypePtr refFor(const AnyValuePtr &addressSpace) override;
    virtual PointerLikeTypePtr tempRef() override;
    virtual PointerLikeTypePtr tempRefFor(const AnyValuePtr &addressSpace) override;
    virtual TypePtr withDecorations(TypeDecorationFlags decorations) override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    virtual PointerLikeTypeValuePtr makeWithValue(const AnyValuePtr &value) override;
    virtual TypePtr asInferredTypeForWithModeInEnvironment(const ASTNodePtr &node, TypeInferenceMode mode, bool isMutable, bool concreteLiterals, const ASTAnalysisEnvironmentPtr &environment) override;

    virtual ASTNodePtr analyzeUnboundMessageSendNode(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;
    
    virtual void addDefaultTypeConversionRules() override;
    void addSpecializedInstanceMethods();
};

/**
 * I am an instance of a function type object.
 */
class TemporaryReferenceTypeValue : public SubtypeOf<PointerLikeTypeValue, TemporaryReferenceTypeValue>
{
public:
    static constexpr char const __typeName__[] = "TemporaryReferenceType";
    static constexpr char const __sysmelTypeName__[] = "_TemporaryReferenceType";
    
    virtual bool isTemporaryReferenceTypeValue() const override;
    virtual bool isReferenceLikeTypeValue() const override;
    virtual SExpression asSExpression() const override;
    virtual TypePtr getType() const override;
    virtual AnyValuePtr acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor) override;

    virtual AnyValuePtr getReferenceToFieldWithType(const FieldVariablePtr &field, const TypePtr &referenceType) override;
    virtual AnyValuePtr getReferenceToSlotWithType(int64_t slotIndex, int64_t slotOffset, const TypePtr &referenceType) override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_TEMPORARY_REFERENCE_TYPE_HPP