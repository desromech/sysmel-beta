#ifndef SYSMEL_ENVIRONMENT_STRUCT_METABUILDER_HPP
#define SYSMEL_ENVIRONMENT_STRUCT_METABUILDER_HPP
#pragma once

#include "TypeMetaBuilder.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(StructMetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class SYSMEL_COMPILER_LIB_EXPORT StructMetaBuilder : public SubtypeOf<TypeMetaBuilder, StructMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "StructMetaBuilder";

protected:
    virtual ASTNodePtr analyzeMessageSendNodeWithSelector(const std::string &selectorValue, const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    virtual ASTNodePtr concretizeMetaBuilder() override;

    bool isPacked_ = false;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_STRUCT_METABUILDER_HPP
