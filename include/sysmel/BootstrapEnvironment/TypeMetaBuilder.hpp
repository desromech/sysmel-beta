#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TYPE_METABUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TYPE_METABUILDER_HPP
#pragma once

#include "NamedProgramEntityMetaBuilder.hpp"
#include "TypeInferenceMode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TYPE_METABUILDER_HPP
