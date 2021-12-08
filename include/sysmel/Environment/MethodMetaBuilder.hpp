#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METHOD_METABUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METHOD_METABUILDER_HPP
#pragma once

#include "MetaBuilder.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(MethodMetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class MethodMetaBuilder : public SubtypeOf<MetaBuilder, MethodMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "MethodMetaBuilder";

    virtual ASTNodePtr analyzeMessageSendNode(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;
    
protected:
    virtual ASTNodePtr concretizeMetaBuilder() override;
    virtual ASTNodePtr analyzeMessageSendNodeWithSelector(const std::string &selectorValue, const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    bool hasAnalyzedMethodPattern = false;
    ASTNodePtr selectorNode;
    ASTNodePtrList argumentsNode;
    ASTNodePtr resultTypeNode;
    ASTNodePtr bodyNode;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METHOD_METABUILDER_HPP

