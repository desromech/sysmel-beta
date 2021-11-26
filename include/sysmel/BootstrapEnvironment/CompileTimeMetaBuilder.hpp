#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_COMPILE_TIME_METABUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_COMPILE_TIME_METABUILDER_HPP
#pragma once

#include "MetaBuilder.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(CompileTimeMetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class CompileTimeMetaBuilder : public SubtypeOf<MetaBuilder, CompileTimeMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "CompileTimeMetaBuilder";

    virtual void setMetaBuilderInstanceContext(const MetaBuilderInstanceContextPtr &context);

protected:
    virtual ASTNodePtr analyzeMessageSendNodeWithSelector(const std::string &selectorValue, const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_COMPILE_TIME_METABUILDER_HPP
