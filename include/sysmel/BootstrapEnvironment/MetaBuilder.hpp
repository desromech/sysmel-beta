#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METABUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METABUILDER_HPP
#pragma once

#include "CompilerObject.hpp"
#include "MacroInvocationContext.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

    virtual bool isMetaBuilder() const;

    virtual ASTNodePtr analyzeMessageSendNode(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer);
    virtual void setMetaBuilderInstanceContext(const MacroInvocationContextPtr &context);

protected:
    virtual ASTNodePtr concretizeMetaBuilderWith(const ASTSemanticAnalyzerPtr &semanticAnalyzer);
    
    MacroInvocationContextPtr instanceContext;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METABUILDER_HPP
