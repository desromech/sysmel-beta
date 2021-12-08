#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METABUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METABUILDER_HPP
#pragma once

#include "CompilerObject.hpp"
#include "MetaBuilderInstanceContext.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(MetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class MetaBuilder : public SubtypeOf<CompilerObject, MetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "MetaBuilder";

    static constexpr bool __isDynamicCompileTimeType__ = false;
    static constexpr bool __isLiteralValueMessageAnalyzer__ = true;
    static constexpr bool __isEphemeralCompileTimeObject__ = true;

    virtual bool isMetaBuilder() const override;

    virtual ASTNodePtr analyzeCallNode(const ASTCallNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;
    virtual ASTNodePtr analyzeMessageSendNode(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;
    virtual ASTNodePtr concretizeEphemeralCompileTimeObject(const ASTLiteralValueNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;
    virtual void setMetaBuilderInstanceContext(const MetaBuilderInstanceContextPtr &context);

    virtual ASTNodePtr delegateToMetaBuilderAt(const MetaBuilderPtr &delegatedMetaBuilder, const ASTSourcePositionPtr &sourcePosition);

    template<typename T>
    ASTNodePtr delegateToMetaBuilderAt(const ASTSourcePositionPtr &sourcePosition)
    {
        return delegateToMetaBuilderAt(basicMakeObject<T> (), sourcePosition);
    }

protected:
    virtual ASTNodePtr analyzeMessageSendNodeWithSelector(const std::string &selectorValue, const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer);
    virtual ASTNodePtr concretizeCallNode(const ASTCallNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer);
    virtual ASTNodePtr concretizeMessageSendNode(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer);
    virtual ASTNodePtr concretizeMetaBuilder();
    virtual ASTNodePtr concretizeMetaBuilderAndAnalyzeWith(const ASTSemanticAnalyzerPtr &semanticAnalyzer);
    
    MetaBuilderInstanceContextPtr instanceContext;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METABUILDER_HPP
