#ifndef SYSMEL_ENVIRONMENT_LET_METABUILDER_HPP
#define SYSMEL_ENVIRONMENT_LET_METABUILDER_HPP
#pragma once

#include "VariableMetaBuilder.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LetMetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class SYSMEL_COMPILER_LIB_EXPORT LetMetaBuilder : public SubtypeOf<VariableMetaBuilder, LetMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "LetMetaBuilder";

    virtual ASTNodePtr analyzeCallNode(const ASTCallNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

protected:
    virtual ASTNodePtr analyzeMessageSendNodeWithSelector(const std::string &selectorValue, const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    virtual ASTNodePtr concretizeMetaBuilder() override;

    ASTNodePtr bindingPatternNode;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_LET_METABUILDER_HPP
