#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LET_METABUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LET_METABUILDER_HPP
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
class LetMetaBuilder : public SubtypeOf<VariableMetaBuilder, LetMetaBuilder>
{
public:
    static constexpr char const __typeName__[] = "LetMetaBuilder";

protected:
    virtual ASTNodePtr concretizeMetaBuilder() override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LET_METABUILDER_HPP
