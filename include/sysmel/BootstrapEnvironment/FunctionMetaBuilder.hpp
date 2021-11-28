#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTION_METABUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTION_METABUILDER_HPP
#pragma once

#include "NamedProgramEntityMetaBuilder.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(FunctionMetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class FunctionMetaBuilder : public SubtypeOf<NamedProgramEntityMetaBuilder, FunctionMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "FunctionMetaBuilder";

    virtual ASTNodePtr analyzeCallNode(const ASTCallNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

protected:
    virtual ASTNodePtr concretizeMetaBuilder() override;
    virtual ASTNodePtr analyzeMessageSendNodeWithSelector(const std::string &selectorValue, const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    bool hasParsedCallArguments = false;
    ASTNodePtrList argumentsNode;
    ASTNodePtr resultTypeNode;
    ASTNodePtr bodyNode;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTION_METABUILDER_HPP

