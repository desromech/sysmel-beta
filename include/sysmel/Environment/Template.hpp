#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TEMPLATE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TEMPLATE_HPP
#pragma once

#include "ModuleDefinedProgramEntity.hpp"
#include <map>

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(Template);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(TemplateInstance);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(ArgumentVariable);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTArgumentDefinitionNode);

struct TemplateDefinitionFragment
{
    ASTSourcePositionPtr position;
    ASTNodePtr node;
    ASTNodePtr bodyNode;
    ASTAnalysisEnvironmentPtr environment;
};

/**
 * I am the base interface for most of the methods that are defined in the system.
 */
class Template : public SubtypeOf<ModuleDefinedProgramEntity, Template>
{
public:
    static constexpr char const __typeName__[] = "Template";

    static constexpr bool __isDynamicCompileTimeType__ = true;
    static constexpr bool __isLiteralValueMessageAnalyzer__ = true;

    virtual bool isTemplate() const override;

    virtual void recordChildProgramEntityDefinition(const ProgramEntityPtr &newChild) override;
    virtual ASTNodePtr analyzeCallNode(const ASTCallNodePtr &callNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    virtual AnyValuePtr getName() const override;
    virtual void setName(const AnyValuePtr &newName);

    void setDeclarationNode(const ASTNodePtr &node);
    void addDefinition(const ASTNodePtr &node, const ASTNodePtr &bodyNode, const ASTAnalysisEnvironmentPtr &bodyEnvironment);

    void setArgumentTypes(const TypePtrList &argumentTypes);
    void setArgumentDeclarationNode(size_t index, const ASTArgumentDefinitionNodePtr &argumentNode);
    void setArgumentDefinitionNode(size_t index, const ASTArgumentDefinitionNodePtr &argumentNode);

    TemplateInstancePtr getOrCreateTemplateInstanceWithArguments(const AnyValuePtrList &instanceArguments);
    AnyValuePtr getOrCreateTemplateInstanceValueWithArguments(const AnyValuePtrList &instanceArguments);

protected:
    ProgramEntityPtrList children;
    AnyValuePtr name;

    ASTSourcePositionPtr declarationPosition;
    ASTNodePtr declarationNode;

    std::vector<TemplateDefinitionFragment> definitionFragments;

    ArgumentVariablePtrList arguments;
    std::map<AnyValuePtrList, TemplateInstancePtr> instanceCache;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TEMPLATE_HPP