#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_COMPILED_METHOD_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_COMPILED_METHOD_HPP
#pragma once

#include "SpecificMethod.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTSourcePosition);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTAnalysisEnvironment);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTArgumentDefinitionNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(CompiledMethod);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(CompileTimeCleanUpScope);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(ArgumentVariable);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(FunctionalType);

/**
 * I am the base interface for most of the methods that are defined in the system.
 */
class CompiledMethod : public SubtypeOf<SpecificMethod, CompiledMethod>
{
public:
    static constexpr char const __typeName__[] = "CompiledMethod";

    virtual bool isCompiledMethod() const override;

    bool isDefined() const;
    bool isDefinedForCompileTime() const;

    void setDeclaration(const ASTNodePtr &node);
    void setDefinition(const ASTNodePtr &node, const ASTNodePtr &bodyNode, const ASTAnalysisEnvironmentPtr &environment);

    virtual void setMethodSignature(const TypePtr &receiverType, const TypePtr &resultType, const TypePtrList &argumentTypes) override;
    virtual void setFunctionSignature(const TypePtr &resultType, const TypePtrList &argumentTypes) override;
    virtual void setClosureSignature(const TypePtr &resultType, const TypePtrList &argumentTypes) override;

    void setArgumentDeclarationNode(size_t index, const ASTArgumentDefinitionNodePtr &argumentNode);
    void setArgumentDefinitionNode(size_t index, const ASTArgumentDefinitionNodePtr &argumentNode);

    ASTNodePtr analyzeDefinitionWith(const ASTSemanticAnalyzerPtr &semanticAnalyzer);

    virtual void ensureSemanticAnalysis() override;

    virtual std::string fullPrintString() const override;
    virtual SExpression asFullDefinitionSExpression() const override;

    virtual AnyValuePtr runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver) override;
    virtual AnyValuePtr applyInClosureWithArguments(const AnyValuePtr &closure, const std::vector<AnyValuePtr> &arguments);
    virtual AnyValuePtr applyWithArguments(const std::vector<AnyValuePtr> &arguments) override;

    virtual void recordChildProgramEntityDefinition(const ProgramEntityPtr &newChild) override;
   
protected:
    void createArgumentVariablesWithTypes(const TypePtrList &argumentTypes);
    void validateBeforeCompileTimeEvaluation();
    void setArgumentsInEvaluationEnvironment(const AnyValuePtr &receiver, const AnyValuePtrList &arguments, const CompileTimeCleanUpScopePtr &environment);
    
    ASTAnalysisEnvironmentPtr createSemanticAnalysisEnvironment();

    ASTSourcePositionPtr declarationPosition;
    ASTNodePtr declarationNode;

    ASTSourcePositionPtr definitionPosition;
    ASTNodePtr definitionNode;
    ASTNodePtr definitionBodyNode;

    ASTAnalysisEnvironmentPtr definitionEnvironment;
    ASTNodePtr analyzedBodyNode;

    ProgramEntityPtrList children;

    ArgumentVariablePtrList arguments;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_COMPILED_METHOD_HPP