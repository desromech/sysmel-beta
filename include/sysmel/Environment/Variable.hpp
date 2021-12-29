#ifndef SYSMEL_ENVIRONMENT_VARIABLE_HPP
#define SYSMEL_ENVIRONMENT_VARIABLE_HPP
#pragma once

#include "ModuleDefinedProgramEntity.hpp"
#include "TypeInferenceMode.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(CompileTimeCleanUpScope);

/**
 * I am a variable program entity.
 */
class SYSMEL_COMPILER_LIB_EXPORT Variable : public SubtypeOf<ModuleDefinedProgramEntity, Variable>
{
public:
    static constexpr char const __typeName__[] = "Variable";

    Variable();

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

    const ASTSourcePositionPtr &getDeclarationPosition() const;
    const ASTSourcePositionPtr &getDefinitionPosition() const;

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

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_VARIABLE_HPP