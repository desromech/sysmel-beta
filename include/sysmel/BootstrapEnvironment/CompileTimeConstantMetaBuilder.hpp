#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_COMPILE_TIME_CONSTANT_METABUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_COMPILE_TIME_CONSTANT_METABUILDER_HPP
#pragma once

#include "NamedProgramEntityMetaBuilder.hpp"
#include "TypeInferenceMode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(CompileTimeConstantMetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class CompileTimeConstantMetaBuilder : public SubtypeOf<NamedProgramEntityMetaBuilder, CompileTimeConstantMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "CompileTimeConstantMetaBuilder";

protected:
    virtual ASTNodePtr analyzeMessageSendNodeWithSelector(const std::string &selectorValue, const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;
    virtual ASTNodePtr concretizeMetaBuilder() override;

    ASTNodePtr typeNode;
    ASTNodePtr valueNode;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_COMPILE_TIME_CONSTANT_METABUILDER_HPP
