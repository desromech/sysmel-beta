#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SPECIFIC_METHOD_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SPECIFIC_METHOD_HPP
#pragma once

#include "Method.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(FunctionalTypeValue)

/**
 * I am the base interface for most of the methods that are defined in the system.
 */
class SpecificMethod : public SubtypeOf<Method, SpecificMethod>
{
public:
    static constexpr char const __typeName__[] = "SpecificMethod";

    virtual bool isSpecificMethod() const override;

    const FunctionalTypePtr &getFunctionalType() const
    {
        return functionalType;
    }

    virtual void setMethodSignature(const TypePtr &receiverType, const TypePtr &resultType, const TypePtrList &argumentTypes);
    virtual void setFunctionSignature(const TypePtr &resultType, const TypePtrList &argumentTypes);
    virtual void setClosureSignature(const TypePtr &resultType, const TypePtrList &argumentTypes);

    virtual TypePtr getExpectedTypeForAnalyzingArgumentWithIndex(size_t argumentIndex);
    virtual MethodPatternMatchingResult matchPatternForRunWithIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver) override;
    virtual MethodPatternMatchingResult matchPatternForAnalyzingMessageSendNode(const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer);

    virtual bool isMacroMethod() const override;

    virtual ASTNodePtr analyzeMessageSendNode(const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;
    virtual ASTNodePtr analyzeIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    virtual FunctionalTypeValuePtr asFunctionalValue();

    void setIntrinsicName(const AnyValuePtr &theIntrinsicName);
    const AnyValuePtr &getIntrinsicName() const;

    void concretizeAutoResultTypeWith(const TypePtr &newResultType);

    bool isExplicit() const;
    void makeExplicit();
    
protected:
    FunctionalTypePtr functionalType;
    FunctionalTypeValuePtr functionalValue;
    AnyValuePtr intrinsicName;

    bool isExplicit_ = false;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SPECIFIC_METHOD_HPP