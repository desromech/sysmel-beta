#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CLASS_METABUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CLASS_METABUILDER_HPP
#pragma once

#include "TypeMetaBuilder.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ClassMetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class ClassMetaBuilder : public SubtypeOf<TypeMetaBuilder, ClassMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "ClassMetaBuilder";

    virtual ASTNodePtr analyzeMessageSendNodeWithSelector(const std::string &selectorValue, const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

protected:
    virtual ASTNodePtr concretizeMetaBuilder() override;

    ASTNodePtr superclassNode;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_CLASS_METABUILDER_HPP
