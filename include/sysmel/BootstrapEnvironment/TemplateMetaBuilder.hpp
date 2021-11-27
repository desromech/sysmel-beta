#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TEMPLATE_METABUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TEMPLATE_METABUILDER_HPP
#pragma once

#include "NamedProgramEntityMetaBuilder.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(TemplateMetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class TemplateMetaBuilder : public SubtypeOf<NamedProgramEntityMetaBuilder, TemplateMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "TemplateMetaBuilder";

    virtual ASTNodePtr analyzeCallNode(const ASTCallNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer);

protected:
    virtual ASTNodePtr concretizeMetaBuilder() override;
    virtual ASTNodePtr analyzeMessageSendNodeWithSelector(const std::string &selectorValue, const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    bool hasParsedCallArguments = false;
    ASTNodePtrList argumentsNode;
    ASTNodePtr bodyNode;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TEMPLATE_METABUILDER_HPP
