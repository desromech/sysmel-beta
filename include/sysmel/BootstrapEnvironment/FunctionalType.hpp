#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTIONAL_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTIONAL_TYPE_HPP
#pragma once

#include "SimpleType.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(FunctionalType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(FunctionalTypeValue);

/**
 * I am an instance of a function type object.
 */
class FunctionalType : public SubMetaTypeOf<SimpleType, FunctionalType>
{
public:
    virtual bool isFunctionalType() const override;
    virtual ASTNodePtr analyzeCallNode(const ASTCallNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    size_t getArgumentCount() const;
    const TypePtr &getArgument(size_t index) const;
    const TypePtr &getResultType() const;

    virtual TypePtr getReceiverType() const;
    virtual FunctionalTypeValuePtr makeValueWithImplementation(const AnyValuePtr &implementation);
    virtual FunctionalTypeValuePtr makeValueWithEnvironmentAndImplementation(const AnyValuePtr &environment, const AnyValuePtr &implementation);
    
    virtual std::string printString() const override;
    
protected:
    TypePtrList arguments;
    TypePtr result;
};

/**
 * I am an instance of a function type object.
 */
class FunctionalTypeValue : public SubtypeOf<AnyValue, FunctionalTypeValue>
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTION_TYPE_HPP