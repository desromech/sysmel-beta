#ifndef SYSMEL_ENVIRONMENT_FUNCTIONAL_TYPE_HPP
#define SYSMEL_ENVIRONMENT_FUNCTIONAL_TYPE_HPP
#pragma once

#include "SimpleType.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(FunctionalType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(FunctionalTypeValue);

/**
 * I am an instance of a function type object.
 */
class SYSMEL_COMPILER_LIB_EXPORT FunctionalType : public SubMetaTypeOf<SimpleType, FunctionalType>
{
public:
    virtual bool isFunctionalType() const override;
    virtual ASTNodePtr analyzeCallNode(const ASTCallNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    virtual bool supportsDynamicCompileTimeMessageSend() const override;

    virtual bool isNullableType() const override;
    virtual bool isImmutableType() override;
    virtual bool hasTrivialInitialization() override;
    virtual bool hasTrivialInitializationCopyingFrom() override;
    virtual bool hasTrivialInitializationMovingFrom() override;
    virtual bool hasTrivialFinalization() override;
    virtual bool hasTrivialAssignCopyingFrom() override;
    virtual bool hasTrivialAssignMovingFrom() override;

    virtual uint64_t getMemorySize() override;
    virtual uint64_t getMemoryAlignment() override;

    void addSpecializedInstanceMethods();

    size_t getArgumentCount() const;
    const TypePtr &getArgument(size_t index) const;
    const TypePtr &getResultType() const;

    virtual TypePtr getReceiverType() const;
    virtual FunctionalTypeValuePtr makeValueWithImplementation(const AnyValuePtr &implementation);
    virtual FunctionalTypeValuePtr makeValueWithEnvironmentAndImplementation(const AnyValuePtr &environment, const AnyValuePtr &implementation);
    
    virtual std::string getQualifiedName() const override;

    bool hasAutoResultType() const;
    virtual FunctionTypePtr copyWithResultType(const TypePtr &newResultType);
    
    bool matchesDefinitionSignature(bool hasReceiver, bool hasConstReceiver, const TypePtrList &argumentTypes, const TypePtr &resultType);
    bool matchesSignature(const TypePtr &receiverType, const TypePtrList &argumentTypes, const TypePtr &resultType) const;

    TypePtr getParamsType() const;
    bool hasCVarArgs() const;

protected:
    TypePtrList arguments;
    TypePtr result;
};

/**
 * I am an instance of a function type object.
 */
class SYSMEL_COMPILER_LIB_EXPORT FunctionalTypeValue : public SubtypeOf<AnyValue, FunctionalTypeValue>
{
public:
    static constexpr char const __typeName__[] = "FunctionalType";
    static constexpr char const __sysmelTypeName__[] = "_FunctionalType";

    static constexpr bool __isDynamicCompileTimeType__ = false;

    virtual bool isFunctionalTypeValue() const override;

    virtual SExpression asSExpression() const override;

    FunctionalTypePtr type;
    AnyValuePtr functionalImplementation;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_FUNCTION_TYPE_HPP