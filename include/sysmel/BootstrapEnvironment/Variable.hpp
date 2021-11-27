#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_VARIABLE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_VARIABLE_HPP
#pragma once

#include "ModuleDefinedProgramEntity.hpp"
#include "TypeInferenceMode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(CompileTimeCleanUpScope);

/**
 * I am a variable program entity.
 */
class Variable : public SubtypeOf<ModuleDefinedProgramEntity, Variable>
{
public:
    static constexpr char const __typeName__[] = "Variable";

    virtual bool isVariable() const override;

    virtual void setDefinitionParameters(const AnyValuePtr &definitionName, const TypePtr &definitionValueType, bool definitionMutability, uint64_t definitionMinimalAlignment = 0);
    virtual void setDeclarationNode(const ASTNodePtr &node);
    virtual void setDefinitionNode(const ASTNodePtr &node);

    virtual ASTNodePtr analyzeIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    virtual AnyValuePtr getName() const override;

    virtual AnyValuePtr findStoreBindingInCompileTime(const CompileTimeCleanUpScopePtr &compileTimeCleanUpScope);

    const TypePtr &getValueType() const
    {
        return valueType;
    }

    const TypePtr &getReferenceType() const
    {
        return referenceType;
    }

    bool isMutable() const
    {
        return isMutable_;
    }

protected:
    ASTSourcePositionPtr declarationPosition;
    ASTNodePtr declarationNode;

    ASTSourcePositionPtr definitionPosition;
    ASTNodePtr definitionNode;

    AnyValuePtr name;
    TypePtr valueType;
    TypePtr referenceType;
    bool isMutable_ = false;
    uint64_t minimalAlignment = 0;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_VARIABLE_HPP