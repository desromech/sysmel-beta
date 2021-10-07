#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_VARIABLE_METABUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_VARIABLE_METABUILDER_HPP
#pragma once

#include "NamedProgramEntityMetaBuilder.hpp"
#include "TypeInferenceMode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(VariableMetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class VariableMetaBuilder : public SubtypeOf<NamedProgramEntityMetaBuilder, VariableMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "VariableMetaBuilder";

protected:
    virtual ASTNodePtr analyzeMessageSendNodeWithSelector(const std::string &selectorValue, const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    ASTNodePtr typeNode;
    ASTNodePtr alignmentNode;
    ASTNodePtr initialValueNode;

    bool isMutable = false;
    TypeInferenceMode typeInferenceMode = TypeInferenceMode::Value;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_VARIABLE_METABUILDER_HPP
