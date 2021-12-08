#ifndef SYSMEL_ENVIRONMENT_TYPE_METABUILDER_HPP
#define SYSMEL_ENVIRONMENT_TYPE_METABUILDER_HPP
#pragma once

#include "NamedProgramEntityMetaBuilder.hpp"
#include "TypeInferenceMode.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(TypeMetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class TypeMetaBuilder : public SubtypeOf<NamedProgramEntityMetaBuilder, TypeMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "TypeMetaBuilder";

protected:
    virtual ASTNodePtr analyzeMessageSendNodeWithSelector(const std::string &selectorValue, const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    ASTNodePtr bodyNode;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_TYPE_METABUILDER_HPP
