#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ENUM_METABUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ENUM_METABUILDER_HPP
#pragma once

#include "TypeMetaBuilder.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(EnumMetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class EnumMetaBuilder : public SubtypeOf<TypeMetaBuilder, EnumMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "EnumMetaBuilder";

    virtual ASTNodePtr analyzeMessageSendNodeWithSelector(const std::string &selectorValue, const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

protected:
    virtual ASTNodePtr concretizeMetaBuilder() override;

    ASTNodePtr valueTypeNode;
    ASTNodePtr valuesNode;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ENUM_METABUILDER_HPP
