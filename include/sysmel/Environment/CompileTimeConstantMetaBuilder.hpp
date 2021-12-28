#ifndef SYSMEL_ENVIRONMENT_COMPILE_TIME_CONSTANT_METABUILDER_HPP
#define SYSMEL_ENVIRONMENT_COMPILE_TIME_CONSTANT_METABUILDER_HPP
#pragma once

#include "NamedProgramEntityMetaBuilder.hpp"
#include "TypeInferenceMode.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(CompileTimeConstantMetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class SYSMEL_COMPILER_LIB_EXPORT CompileTimeConstantMetaBuilder : public SubtypeOf<NamedProgramEntityMetaBuilder, CompileTimeConstantMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "CompileTimeConstantMetaBuilder";

protected:
    virtual ASTNodePtr analyzeMessageSendNodeWithSelector(const std::string &selectorValue, const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;
    virtual ASTNodePtr concretizeMetaBuilder() override;

    ASTNodePtr typeNode;
    ASTNodePtr valueNode;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_COMPILE_TIME_CONSTANT_METABUILDER_HPP
