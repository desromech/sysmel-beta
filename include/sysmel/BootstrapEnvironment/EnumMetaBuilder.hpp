#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ENUM_METABUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ENUM_METABUILDER_HPP
#pragma once

#include "TypeMetaBuilder.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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
    ASTNodePtr valueTypeNode;
    ASTNodePtr valuesNode;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ENUM_METABUILDER_HPP
