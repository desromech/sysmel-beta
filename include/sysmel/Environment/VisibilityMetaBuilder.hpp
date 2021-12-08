#ifndef SYSMEL_ENVIRONMENT_VISIBILITY_METABUILDER_HPP
#define SYSMEL_ENVIRONMENT_VISIBILITY_METABUILDER_HPP
#pragma once

#include "MetaBuilder.hpp"
#include "ProgramEntityVisibility.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(VisibilityMetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class VisibilityMetaBuilder : public SubtypeOf<MetaBuilder, VisibilityMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "VisibilityMetaBuilder";

    virtual void setMetaBuilderInstanceContext(const MetaBuilderInstanceContextPtr &context) override;

    virtual ProgramEntityVisibility getVisibility() const;

protected:
    virtual ASTNodePtr analyzeMessageSendNodeWithSelector(const std::string &selectorValue, const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_VISIBILITY_METABUILDER_HPP
