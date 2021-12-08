#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_NAMED_PROGRAM_ENTITY_METABUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_NAMED_PROGRAM_ENTITY_METABUILDER_HPP
#pragma once

#include "MetaBuilder.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(NamedProgramEntityMetaBuilder);

/**
 * I am the base interface for a meta-builder.
 */
class NamedProgramEntityMetaBuilder : public SubtypeOf<MetaBuilder, NamedProgramEntityMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "NamedProgramEntityMetaBuilder";

    virtual ASTNodePtr analyzeMessageSendNode(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer);

protected:
    virtual bool isValidNameNode(const ASTNodePtr &nameNode) const;
    
    ASTNodePtr nameNode;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_NAMED_PROGRAM_ENTITY_METABUILDER_HPP
