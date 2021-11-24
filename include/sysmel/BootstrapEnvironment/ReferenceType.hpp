#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_REFERENCE_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_REFERENCE_TYPE_HPP
#pragma once

#include "PointerLikeType.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ReferenceType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ReferenceTypeValue);

/**
 * I am an instance of a function type object.
 */
class ReferenceType : public SubMetaTypeOf<PointerLikeType, ReferenceType>
{
public:
    static ReferenceTypePtr make(const TypePtr &baseType);
    static ReferenceTypePtr makeWithAddressSpace(const TypePtr &baseType, const AnyValuePtr &addressSpace);

    virtual bool isReferenceType() const override;
    virtual bool isReferenceLikeType() const override;

    // Is this a pointer like type value?
    virtual ReferenceTypePtr ref() override;
    virtual ReferenceTypePtr refFor(const AnyValuePtr &addressSpace) override;
    virtual PointerLikeTypePtr tempRef() override;
    virtual PointerLikeTypePtr tempRefFor(const AnyValuePtr &addressSpace) override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;
    
    virtual PointerLikeTypeValuePtr makeWithValue(const AnyValuePtr &value) override;
    virtual TypePtr asInferredTypeWithMode(TypeInferenceMode mode, bool isMutable) override;

    virtual ASTNodePtr analyzeUnboundMessageSendNode(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    void addSpecializedInstanceMethods();
};

/**
 * I am an instance of a function type object.
 */
class ReferenceTypeValue : public SubtypeOf<PointerLikeTypeValue, ReferenceTypeValue>
{
public:
    static constexpr char const __typeName__[] = "ReferenceType";
    static constexpr char const __sysmelTypeName__[] = "_ReferenceType";
    
    virtual bool isReferenceTypeValue() const override;
    virtual bool isReferenceLikeTypeValue() const override;
    virtual TypePtr getType() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_REFERENCE_TYPE_HPP