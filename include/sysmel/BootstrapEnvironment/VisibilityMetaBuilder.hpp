#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_VISIBILITY_METABUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_VISIBILITY_METABUILDER_HPP
#pragma once

#include "MetaBuilder.hpp"
#include "ProgramEntityVisibility.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LetMetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class VisibilityMetaBuilder : public SubtypeOf<MetaBuilder, VisibilityMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "VisibilityMetaBuilder";

    virtual void setMetaBuilderInstanceContext(const MetaBuilderInstanceContextPtr &context);

    virtual ProgramEntityVisibility getVisibility() const;

protected:
    virtual ASTNodePtr analyzeMessageSendNodeWithSelector(const std::string &selectorValue, const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_VISIBILITY_METABUILDER_HPP