#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ENUM_CONSTANT_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ENUM_CONSTANT_HPP
#pragma once

#include "ModuleDefinedProgramEntity.hpp"
#include "TypeInferenceMode.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(EnumConstant)
SYSMEL_DECLARE_BOOTSTRAP_CLASS(DeferredCompileTimeCodeFragment)

/**
 * I am a compile time constant program entity.
 */
class EnumConstant : public SubtypeOf<ModuleDefinedProgramEntity, EnumConstant>
{
public:
    static constexpr char const __typeName__[] = "EnumConstant";

    virtual bool isEnumConstant() const override;

    virtual ASTNodePtr analyzeIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    virtual AnyValuePtr getName() const override;
    void setName(const AnyValuePtr &newName);

    const AnyValuePtr &getValue() const;
    void setValue(const AnyValuePtr &newValue);

    void setDefinitionNode(const ASTNodePtr &node);

protected:
    friend class ASTSemanticAnalyzer;

    ASTSourcePositionPtr definitionPosition;
    ASTNodePtr definitionNode;

    AnyValuePtr name;
    AnyValuePtr value;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ENUM_CONSTANT_HPP